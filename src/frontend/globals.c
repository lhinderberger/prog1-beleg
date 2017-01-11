/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */

#include "frontend/error.h"
#include "frontend/globals.h"
#include "frontend/list.h"

GtkBuilder * builder = NULL;
GtkWindow * mainWindow = NULL;
GtkWidget * mainWidget = NULL;
GtkBox * mainLayout = NULL;
pb_database * db = NULL;

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
    show_material_list();
}

void close_database() {
    if (db) {
        pb_close_database(db);
        db = NULL;
    }
}

void new_database() {
    /* Prepare dialog */
    GtkFileChooserDialog * newDbFileChooser = (GtkFileChooserDialog*)gtk_builder_get_object(builder, "newDbFileChooser");
    if (!newDbFileChooser)
        fatal_error(widget_retrieval_error);
    gtk_window_set_transient_for((GtkWindow*)newDbFileChooser, mainWindow);

    /* Execute dialog */
    if (gtk_dialog_run((GtkDialog*)newDbFileChooser) == 1)
        open_database_impl((GtkDialog*)newDbFileChooser, 1);

    /* Cleanup */
    gtk_widget_hide((GtkWidget*)newDbFileChooser);
}

void open_database() {
    /* Prepare dialog */
    GtkFileChooserDialog * obenDbFileChooser = (GtkFileChooserDialog*)gtk_builder_get_object(builder, "openDbFileChooser");
    if (!obenDbFileChooser)
        fatal_error(widget_retrieval_error);
    gtk_window_set_transient_for((GtkWindow*)obenDbFileChooser, mainWindow);

    /* Execute dialog */
    //TODO: intercept directory opening!
    if (gtk_dialog_run((GtkDialog*)obenDbFileChooser) == 1)
        open_database_impl((GtkDialog*)obenDbFileChooser, 0);

    /* Cleanup */
    gtk_widget_hide((GtkWidget*)obenDbFileChooser);
}

void save_database_as() {
    fatal_error("Not Implemented!");
}

void swap_main_widget(GtkWidget * widget) {
    if (mainWidget) {
        gtk_container_remove((GtkContainer *)mainLayout, mainWidget);
        mainWidget = NULL;
    }

    gtk_box_pack_start((GtkBox*)mainLayout, widget, 1, 1, 0);
    mainWidget = widget;
}
