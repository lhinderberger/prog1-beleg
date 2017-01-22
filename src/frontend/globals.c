/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */

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
    GtkFileChooser * chooser = GTK_FILE_CHOOSER(fileChooserDialog);
    if (!chooser || !(filename = gtk_file_chooser_get_filename(chooser)))
        fatal_error("Could not retrieve file name!");

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
    if (!widget)
        fatal_error(widget_retrieval_error);
    return widget;
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
    if (gtk_dialog_run((GtkDialog*)newDbFileChooser) == 1)
        open_database_impl((GtkDialog*)newDbFileChooser, 1);

    /* Cleanup */
    gtk_widget_hide((GtkWidget*)newDbFileChooser);
}

void open_database() {
    /* Prepare dialog */
    GtkFileChooserDialog * obenDbFileChooser = (GtkFileChooserDialog*)checked_retrieve_widget("openDbFileChooser");
    gtk_window_set_transient_for((GtkWindow*)obenDbFileChooser, mainWindow);

    /* Execute dialog */
    //TODO: intercept directory opening!
    if (gtk_dialog_run((GtkDialog*)obenDbFileChooser) == 1)
        open_database_impl((GtkDialog*)obenDbFileChooser, 0);

    /* Cleanup */
    gtk_widget_hide((GtkWidget*)obenDbFileChooser);
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
