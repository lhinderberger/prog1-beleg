/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */

#include <assert.h>
#include <string.h>
#include <glib/gi18n.h>

#include "common/error.h"
#include "data/image.h"
#include "frontend/welcome.h"
#include "frontend/error.h"
#include "frontend/globals.h"
#include "frontend/list.h"

GtkBuilder * builder = NULL;
GtkWindow * mainWindow = NULL;
GtkWidget * mainWidget = NULL;
GtkBox * mainLayout = NULL;
GtkStatusbar * statusbar = NULL;
pb_database * db = NULL;
pb_material_item_buffer item_buf = NULL;

const char * widget_creation_error = "Could not create new widget (_not_ from builder)!";
const char * widget_retrieval_error = "Could not retrieve widget from builder!";

void open_database_impl(GtkDialog * fileChooserDialog, int create) {
    /* Close current database first */
    close_database();

    /* Retrieve filename */
    char * filename;
    GtkFileChooser * chooser = (GtkFileChooser*)fileChooserDialog;
    if (!chooser || !(filename = gtk_file_chooser_get_filename(chooser)))
        fatal_error("Could not retrieve file name!");

    /* In create mode: Warn user about pre-existing file and abort.
     * Otherwise make sure this can be a database, by checking for the SQLite magic header
     * (see: https://www.sqlite.org/fileformat.html)
     * */
    FILE * f = fopen(filename, "rb");
    if (create) {
        if (f) {
            gtk_widget_hide((GtkWidget *) fileChooserDialog);
            warning(C_("magic header checking", "Datei existiert bereits - kann existierende Datei nicht überschreiben!"), filename);
            return;
        }
    }
    else {
        if (!f) {
            gtk_widget_hide((GtkWidget *) fileChooserDialog);
            warning(C_("magic header checking", "Konnte Datei nicht öffnen!"), filename);
            return;
        }

        char magic_header[17];
        int n_bytes_read = (int) fread(magic_header, sizeof(char), 16, f);
        magic_header[16] = 0;
        fclose(f);
        if (n_bytes_read != 16) {
            gtk_widget_hide((GtkWidget *) fileChooserDialog);
            warning(C_("magic header checking", "Konnte Dateikopf nicht lesen!"), filename);
            return;
        }
        if (strcmp(magic_header, "SQLite format 3")) {
            gtk_widget_hide((GtkWidget *) fileChooserDialog);
            warning(C_("magic header checking", "Falsches Dateiformat!"), filename);
            return;
        }
    }

    /* Open database */
    db = pb_open_database(filename, create, 1);
    g_free(filename);
    if (!db)
        fatal_pb_error();

    /* Open material list */
    set_db_controls_sensitive(TRUE);
    show_material_list();
}

GtkWidget * checked_retrieve_widget(const char * identifier) {
    GtkWidget * widget = (GtkWidget*)gtk_builder_get_object(builder, identifier);
    if (!widget) {
        warning("Retrieval error:", identifier);
        fatal_error(widget_retrieval_error);
    }
    return widget;
}

void chooser_file_activated(GtkFileChooser * chooser) {
    gtk_dialog_response((GtkDialog*)chooser, GTK_RESPONSE_ACCEPT);
}

void close_database() {
    if (db) {
        pb_close_database(db);
        db = NULL;
    }
    set_db_controls_sensitive(FALSE);
    show_welcome_screen();
}

void new_database() {
    /* Prepare dialog */
    GtkFileChooserDialog * newDbFileChooser = (GtkFileChooserDialog*)checked_retrieve_widget("newDbFileChooser");
    gtk_window_set_transient_for((GtkWindow*)newDbFileChooser, mainWindow);

    /* Execute dialog */
    if (gtk_dialog_run((GtkDialog*)newDbFileChooser) == GTK_RESPONSE_ACCEPT)
        open_database_impl((GtkDialog*)newDbFileChooser, 1);

    /* Cleanup */
    gtk_widget_hide((GtkWidget*)newDbFileChooser);
}

void open_database() {
    /* Prepare dialog */
    GtkFileChooserDialog * obenDbFileChooser = (GtkFileChooserDialog*)checked_retrieve_widget("openDbFileChooser");
    gtk_window_set_transient_for((GtkWindow*)obenDbFileChooser, mainWindow);

    /* Execute dialog */
    if (gtk_dialog_run((GtkDialog*)obenDbFileChooser) == GTK_RESPONSE_ACCEPT)
        open_database_impl((GtkDialog*)obenDbFileChooser, 0);

    /* Cleanup */
    gtk_widget_hide((GtkWidget*)obenDbFileChooser);
}

void remove_material_item(GtkButton * button, pb_material_item * item) {
    if (!item) {
        warning("Remove callback triggered without item!", "");
        return;
    }

    /* Ask user for confirmation */
    GtkDialog * dialog = (GtkDialog*)gtk_message_dialog_new(mainWindow, GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, C_("remove material item", "Möchten Sie den Artikel wirklich löschen?"));
    if (!dialog)
        fatal_error(widget_creation_error);
    int response = gtk_dialog_run(dialog);
    gtk_widget_destroy((GtkWidget*)dialog);

    /* Abort, if user decided not to delete */
    if (response == GTK_RESPONSE_NO)
        return;

    /* Perform delete otherwise */
    if (pb_mat_item_delete(db, item->id) != 0)
        fatal_pb_error();
    if (item->image_id) {
        if (pb_image_delete(db, item->image_id) != 0)
            warning("Could not delete image!", pb_error_str(pb_errno()));
    }

    /* And reload material item list */
    show_material_list();

    /* Inform user in status bar about our great success */
    gtk_statusbar_push(statusbar, 0, C_("material item deleted", "Artikel erfolgreich gelöscht. Änderungen wurden gespeichert."));
}

void set_db_controls_sensitive(int sensitive) {
    GtkAction * actionCloseDatabase = (GtkAction*)checked_retrieve_widget("actionCloseDatabase");
    gtk_action_set_sensitive (actionCloseDatabase, sensitive);
}

void swap_main_widget(GtkWidget * widget) {
    if (mainWidget) {
        gtk_container_remove((GtkContainer *)mainLayout, mainWidget);
        mainWidget = NULL;
    }

    if (widget)
        gtk_box_pack_start(mainLayout, widget, 1, 1, 0);
    mainWidget = widget;
}

GdkPixbuf * scale_pixbuf_proportionally(GdkPixbuf * pixbuf, int max_height, int max_width) {
    assert(pixbuf);

    int src_width = gdk_pixbuf_get_width(pixbuf);
    int src_height = gdk_pixbuf_get_height(pixbuf);
    if (src_width == 0 || src_height == 0) {
        warning("Image dimensions error!", "src_width or src_height is zero!");
        return NULL;
    }
    int dest_width = src_width > src_height ? max_width : max_height * src_width / src_height;
    int dest_height = src_width <= src_height ? max_height : max_width * src_height / src_width;
    return gdk_pixbuf_scale_simple(pixbuf, dest_width, dest_height, GDK_INTERP_BILINEAR);
}