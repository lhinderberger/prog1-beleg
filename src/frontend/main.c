/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include "logic/items_query.h"

#include "frontend/error.h"
#include "frontend/globals.h"
#include "frontend/welcome.h"

void cleanup();
void init(int argc, char ** args);
void init_ui(int argc, char ** args);

int main(int argc, char ** args) {
    init(argc, args);

    show_welcome_screen();
    gtk_widget_show((GtkWidget*)mainWindow);
    gtk_main();

    cleanup();
    return 0;
}

/**
 * Initializes the application, including user interface and globals
 * @param argc main function argument count
 * @param args main function arguments
 */
void init(int argc, char ** args) {
    /* Allocate material item buffer */
    item_buf = pb_create_mat_item_buffer(ITEM_BUF_SIZE);
    if (!item_buf)
        fatal_error("Could not allocate material item buffer");

    /* Initialize UI */
    init_ui(argc, args);
}

/**
 * Clean up all application resources for a clean exit.
 */
void cleanup() {
    close_database();
    pb_free_mat_item_buffer(item_buf);
    item_buf = NULL;
}

/**
 * Initializes user interface
 * Loads ui definition, initializes globals, wires Actions
 * @param argc main function argument count
 * @param args main function arguments
 */
void init_ui(int argc, char ** args) {
    gtk_init(&argc, &args);

    /* Initialize builder and load UI */
    builder = gtk_builder_new();
    if (!builder)
        fatal_error("Could not initialize GTKBuilder!");

    if (!gtk_builder_add_from_file(builder, "prog1-beleg.glade", NULL))
        fatal_error("Could not load UI file");

    /* Retrieve main window and main layout */
    mainWindow = (GtkWindow*)gtk_builder_get_object(builder, "mainWindow");
    if (!mainWindow)
        fatal_error(widget_retrieval_error);
    g_signal_connect(mainWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_window_set_title(mainWindow, C_("Main Window heading", "Materialdatenbank"));

    mainLayout = (GtkBox*)gtk_builder_get_object(builder, "mainLayout");
    if (!mainLayout)
        fatal_error(widget_retrieval_error);

    statusbar = (GtkStatusbar*)gtk_builder_get_object(builder, "statusbar");
    if (!statusbar)
        fatal_error(widget_retrieval_error);

    /* Wire actions */
    GtkAction * actionCloseDatabase = (GtkAction*)gtk_builder_get_object(builder, "actionCloseDatabase");
    if (!actionCloseDatabase)
        fatal_error(widget_retrieval_error);
    g_signal_connect(actionCloseDatabase, "activate", G_CALLBACK(close_database), NULL);

    GtkAction * actionSaveDatabaseAs = (GtkAction*)gtk_builder_get_object(builder, "actionSaveDatabaseAs");
    if (!actionSaveDatabaseAs)
        fatal_error(widget_retrieval_error);
    g_signal_connect(actionSaveDatabaseAs, "activate", G_CALLBACK(save_database_as), NULL);
    set_db_controls_sensitive(FALSE);

    GtkAction * actionNewDatabase = (GtkAction*)gtk_builder_get_object(builder, "actionNewDatabase");
    if (!actionNewDatabase)
        fatal_error(widget_retrieval_error);
    g_signal_connect(actionNewDatabase, "activate", G_CALLBACK(new_database), NULL);

    GtkAction * actionOpenDatabase = (GtkAction*)gtk_builder_get_object(builder, "actionOpenDatabase");
    if (!actionOpenDatabase)
        fatal_error(widget_retrieval_error);
    g_signal_connect(actionOpenDatabase, "activate", G_CALLBACK(open_database), NULL);
}