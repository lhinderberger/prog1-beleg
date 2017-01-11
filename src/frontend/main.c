/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#include <gtk/gtk.h>
#include "data/database.h"

#include "frontend/error.h"
#include "frontend/globals.h"
#include "frontend/welcome.h"

void init_ui(int argc, char ** args);

int main(int argc, char ** args) {
    init_ui(argc, args);
    show_welcome_screen();
    gtk_widget_show((GtkWidget*)mainWindow);
    gtk_main();
    close_database();
    return 0;
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

    mainLayout = (GtkBox*)gtk_builder_get_object(builder, "mainLayout");
    if (!mainLayout)
        fatal_error(widget_retrieval_error);

    /* Wire actions */
    GtkAction * actionCloseDatabase = (GtkAction*)gtk_builder_get_object(builder, "actionCloseDatabase");
    if (!actionCloseDatabase)
        fatal_error(widget_retrieval_error);
    gtk_action_set_sensitive (actionCloseDatabase, FALSE);
    g_signal_connect(actionCloseDatabase, "activate", G_CALLBACK(close_database), NULL);

    GtkAction * actionSaveDatabaseAs = (GtkAction*)gtk_builder_get_object(builder, "actionSaveDatabaseAs");
    if (!actionSaveDatabaseAs)
        fatal_error(widget_retrieval_error);
    gtk_action_set_sensitive (actionSaveDatabaseAs, FALSE);
    g_signal_connect(actionSaveDatabaseAs, "activate", G_CALLBACK(save_database_as), NULL);

    GtkAction * actionNewDatabase = (GtkAction*)gtk_builder_get_object(builder, "actionNewDatabase");
    if (!actionNewDatabase)
        fatal_error(widget_retrieval_error);
    g_signal_connect(actionNewDatabase, "activate", G_CALLBACK(new_database), NULL);

    GtkAction * actionOpenDatabase = (GtkAction*)gtk_builder_get_object(builder, "actionOpenDatabase");
    if (!actionOpenDatabase)
        fatal_error(widget_retrieval_error);
    g_signal_connect(actionOpenDatabase, "activate", G_CALLBACK(open_database), NULL);
}