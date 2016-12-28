/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#include "common/error.h"
#include "frontend/error.h"
#include "frontend/globals.h"

#include <gtk/gtk.h>
#include <stdlib.h>

void fatal_error(const char * message) {
    GtkWidget * dialog = gtk_message_dialog_new(mainWindow, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, message);
    if (!dialog)
        fprintf(stderr, "Could not create fatal error dialogue.\n%s", message);
    else
        gtk_dialog_run(GTK_DIALOG(dialog));
    exit(-1);
}

void fatal_pb_error() {
    fatal_error(pb_error_str(pb_errno()));
}