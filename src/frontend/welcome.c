/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */

#include "frontend/error.h"
#include "frontend/globals.h"

void show_welcome_screen() {
    GtkWidget * welcomeScreen = checked_retrieve_widget("welcomeScreen");
    swap_main_widget(welcomeScreen);
}