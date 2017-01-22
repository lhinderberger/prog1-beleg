/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */

#include <assert.h>
#include <gtk/gtk.h>
#include <memory.h>
#include <malloc.h>
#include <glib/gi18n.h>
#include <data/image.h>

#include "common/error.h"
#include "frontend/error.h"
#include "frontend/globals.h"
#include "frontend/list.h"
#include "frontend/editor.h"

int createMode = 0;
int editorControlsWired = 0;
pb_material_item * editorItem = NULL;
GtkGrid * editorWidget = NULL;
GtkEntry * entryName = NULL;
GtkEntry * entryArticleNo = NULL;
GtkSpinButton * spinBtnStock = NULL;
GtkImage * articleImage = NULL;
GdkPixbuf * articleImagePixbuf = NULL;
GtkFileChooserButton * articleImageFileChooserBtn = NULL;

void cleanup_ai_pixbuf() {
    if (!articleImagePixbuf)
        return;

    gdk_pixbuf_unref(articleImagePixbuf);
    articleImagePixbuf = NULL;
}

void cleanup_editor() {
    if (createMode && editorItem) {
        free(editorItem);
        editorItem = NULL;
    }

    cleanup_ai_pixbuf();
}

void reset_preview_image() {
    if (articleImagePixbuf) {
        /* Scale image */
        GdkPixbuf *scaled_pixbuf = scale_pixbuf_proportionally(articleImagePixbuf, 200, 200);

        /* Display image */
        gtk_image_set_from_pixbuf(articleImage, scaled_pixbuf);
        gdk_pixbuf_unref(scaled_pixbuf);
    }
    else {
        gtk_image_set_from_icon_name(articleImage, "gtk-missing-image", GTK_ICON_SIZE_BUTTON);
    }
}

void fill_ui() {
    /* Fill text entries */
    gtk_entry_set_text(entryName, editorItem->name);
    gtk_entry_set_text(entryArticleNo, editorItem->article_no);

    /* Fill spin button */
    GtkAdjustment * n_stock_adjustment = gtk_adjustment_new(editorItem->n_stock, 0.0, 9999999999.0, 1.0, 5.0, 0.0);
    if (!n_stock_adjustment)
        fatal_error("Could not create adjustment for n_stock");
    gtk_spin_button_set_adjustment(spinBtnStock, n_stock_adjustment);

    /* Fill article image */
    cleanup_ai_pixbuf();
    if (editorItem->image_id != 0) {
        /* Load image from database via input stream to pixbuf */
        int image_len;
        void *image = pb_image_retrieve(db, editorItem->image_id, &image_len);
        if (!image)
            fatal_pb_error();

        articleImagePixbuf = gdk_pixbuf_new_from_stream(g_memory_input_stream_new_from_data(image, image_len, NULL), NULL, NULL);
        if (!articleImagePixbuf)
            warning("Could not load article image!", "");
    }

    reset_preview_image();
}

void image_changed(GtkFileChooserButton * chooser) {
    /* Retrieve image */
    cleanup_ai_pixbuf();
    articleImagePixbuf = gdk_pixbuf_new_from_file(gtk_file_chooser_get_filename((GtkFileChooser*)chooser), NULL);
    if (!articleImagePixbuf) {
        warning("Could not load image!", "GDK Error");
        return;
    }

    reset_preview_image();
}

void image_removed() {
    cleanup_ai_pixbuf();
    reset_preview_image();
}

void leave_editor() {
    cleanup_editor();
    show_material_list();
}

void save_changes() {
    /* Set new values for item */
    strcpy(editorItem->name, gtk_entry_get_text(entryName));
    editorItem->name[64] = 0;

    strcpy(editorItem->article_no, gtk_entry_get_text(entryArticleNo));
    editorItem->name[32] = 0;

    editorItem->n_stock = gtk_spin_button_get_value_as_int(spinBtnStock);

    /* Try to save image */
    int remove_image_id = 0;
    if (articleImagePixbuf) {
        /* Read entire image file */
        FILE * f = fopen(gtk_file_chooser_get_filename((GtkFileChooser*)articleImageFileChooserBtn), "rb");
        if (!f)
            fatal_error("Could not open image file!");

        fseek(f, 0, SEEK_END);
        size_t file_len = (size_t)ftell(f);
        fseek(f, 0, SEEK_SET);

        char * buffer = malloc(file_len * sizeof(char));
        if (!buffer)
            fatal_error("Could not allocate memory for file buffer!");
        if (fread(buffer, sizeof(char), file_len, f) != file_len)
            fatal_error("File read error!");

        /* Try to write to database */
        editorItem->image_id = pb_image_save(db, editorItem->image_id, editorItem->image_id != 0, buffer, (int)file_len);
        if (!editorItem->image_id)
            warning("Could not save article image!", pb_error_str(pb_errno()));

        /* Clean up */
        free(buffer);
        fclose(f);
    }
    else if (editorItem->image_id) {
        /* Mark image for removal */
        remove_image_id = editorItem->image_id;
        editorItem->image_id = 0;
    }

    /* Try to save item to database and optionally remove image */
    if (!pb_mat_item_save(db, editorItem, !createMode))
        fatal_pb_error();
    if (remove_image_id != 0 && pb_image_delete(db, editorItem->image_id) != 0)
        fatal_pb_error();

    /* Inform user in status bar about our great success */
    gtk_statusbar_push(statusbar, 0, C_("editor_save_changes", "Artikel erfolgreich geändert. Änderungen wurden gespeichert."));

    /* Return to previous screen */
    leave_editor();
}



void open_editor(pb_material_item * item, int create) {
    /* Clean up corrupted state, if any */
    cleanup_editor();

    /* Read arguments */
    assert((create && !item) || (!create && item));
    editorItem = create ? NULL : item;
    createMode = create != 0;

    /* Try to create buffer item, when in create mode */
    if (create) {
        editorItem = (pb_material_item*)malloc(sizeof(pb_material_item));
        if (!editorItem)
            fatal_error("Could not allocate memory for material item!");
        memset(editorItem, 0, sizeof(pb_material_item));
    }

    /* Retrieve editor UI components */
    editorWidget = (GtkGrid*)checked_retrieve_widget("itemEditor");
    entryName = (GtkEntry*)checked_retrieve_widget("entryName");
    entryArticleNo = (GtkEntry*)checked_retrieve_widget("entryArticleNo");
    spinBtnStock = (GtkSpinButton*)checked_retrieve_widget("spinBtnStock");
    articleImage = (GtkImage*)checked_retrieve_widget("articleImage");
    articleImageFileChooserBtn = (GtkFileChooserButton*)checked_retrieve_widget("articleImageFileChooserBtn");

    GtkButton * btnAbort = (GtkButton*)checked_retrieve_widget("btnAbort");
    GtkButton * btnSaveChanges = (GtkButton*)checked_retrieve_widget("btnSaveChanges");
    GtkButton * btnRemoveArticleImage = (GtkButton*)checked_retrieve_widget("btnRemoveArticleImage");

    /* Wire up controls */
    if (!editorControlsWired) {
        g_signal_connect(editorWidget, "destroy", G_CALLBACK(cleanup_editor), NULL);
        g_signal_connect(btnAbort, "clicked", G_CALLBACK(leave_editor), NULL);
        g_signal_connect(btnSaveChanges, "clicked", G_CALLBACK(save_changes), NULL);
        g_signal_connect(btnRemoveArticleImage, "clicked", G_CALLBACK(image_removed), NULL);
        g_signal_connect(articleImageFileChooserBtn, "file-set", G_CALLBACK(image_changed), NULL);
        editorControlsWired = 1;
    }

    /* Fill UI */
    fill_ui();

    /* Show editor */
    swap_main_widget((GtkWidget*)editorWidget);
}