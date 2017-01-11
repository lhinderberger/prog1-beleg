/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#ifndef PB_FRONTEND_GLOBALS
#define PB_FRONTEND_GLOBALS

#include <gtk/gtk.h>

#include "data/database.h"

extern GtkBuilder * builder;
extern GtkWindow * mainWindow;
extern GtkWidget * mainWidget;
extern GtkBox * mainLayout;

extern const char * widget_creation_error;
extern const char * widget_retrieval_error;

extern pb_database * db;

/* Closes database and resets application. Suitable for cleanup. */
void close_database();

/* Action hook for creating a database */
void new_database();

/* Action hook for opening another database */
void open_database();

/* Action hook for saving the database under a different name */
void save_database_as();

void swap_main_widget(GtkWidget * widget);

#endif