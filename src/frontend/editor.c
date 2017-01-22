/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */

#include <assert.h>
#include <gtk/gtk.h>
#include <memory.h>
#include <malloc.h>
#include <glib/gi18n.h>

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

void cleanup_editor() {
    if (createMode && editorItem) {
        free(editorItem);
        editorItem = NULL;
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

    /* Try to save item to database */
    if (!pb_mat_item_save(db, editorItem, !createMode))
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

    GtkButton * btnAbort = (GtkButton*)checked_retrieve_widget("btnAbort");
    GtkButton * btnSaveChanges = (GtkButton*)checked_retrieve_widget("btnSaveChanges");

    /* Wire up controls */
    if (!editorControlsWired) {
        g_signal_connect(editorWidget, "destroy", G_CALLBACK(cleanup_editor), NULL);
        g_signal_connect(btnAbort, "clicked", G_CALLBACK(leave_editor), NULL);
        g_signal_connect(btnSaveChanges, "clicked", G_CALLBACK(save_changes), NULL);
        editorControlsWired = 1;
    }

    /* Fill UI */
    fill_ui();

    /* Show editor */
    swap_main_widget((GtkWidget*)editorWidget);
}