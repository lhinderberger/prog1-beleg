/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */

#include "frontend/error.h"
#include "frontend/globals.h"

void show_welcome_screen() {
    GtkWidget * welcomeScreen = (GtkWidget*)gtk_builder_get_object(builder, "welcomeScreen");
    if (!welcomeScreen)
        fatal_error(widget_retrieval_error);
    swap_main_widget(welcomeScreen);
}