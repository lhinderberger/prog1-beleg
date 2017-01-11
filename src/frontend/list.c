#include "frontend/globals.h"
#include "frontend/error.h"
#include "frontend/list.h"

#include <glib/gi18n.h>

GtkGrid * listGrid = NULL;
GtkWidget * listScreen = NULL;
GtkScrolledWindow * scrolledWindow = NULL;
int n_items = 0;

void add_list_heading(int column, const char * label) {
    GtkWidget * lbl_widget = gtk_label_new(label);
    if (!lbl_widget)
        fatal_error(widget_creation_error);
    gtk_grid_attach(listGrid, lbl_widget, column, 0, 1, 1);
    gtk_widget_show(lbl_widget);
}

void clear_list() {
    /* Remove old list grid, if any */
    if (listGrid) {
        gtk_container_remove((GtkContainer*)listScreen, (GtkWidget*)listGrid);

        gtk_widget_destroy((GtkWidget*)listGrid);
        listGrid = NULL;
    }

    /* Create a new one */
    n_items = 0;
    listGrid = (GtkGrid*)gtk_grid_new();
    if (!listGrid)
        fatal_error(widget_creation_error);
    gtk_container_add((GtkContainer*)scrolledWindow, (GtkWidget*)listGrid);

    /* Add headings */
    add_list_heading(1, C_("List headings", "Artikelname"));
    add_list_heading(2, C_("List headings", "Artikelnummer"));
    add_list_heading(3, C_("List headings", "Lagerbestand"));

    /* Show widget */
    gtk_widget_show((GtkWidget*)listGrid);
}

void render_material_item(pb_material_item * item) {
    GtkWidget * dummybutton = gtk_button_new();
    char buf[512];
    sprintf(buf, "Dummy Button %d", (int)item);
    gtk_button_set_label((GtkButton*)dummybutton, buf);
    gtk_grid_attach(listGrid, dummybutton, 0, 1 + n_items, 1, 1);
    gtk_widget_show(dummybutton);
    n_items++;
    //TODO: Render actual material item
}

void show_material_list() {
    if (!listScreen) {
        listScreen = (GtkWidget*)gtk_builder_get_object(builder, "listScreen");
        if (!listScreen)
            fatal_error(widget_retrieval_error);
    }
    if (!scrolledWindow) {
        scrolledWindow = (GtkScrolledWindow*)gtk_builder_get_object(builder, "scrolledWindowForList");
        if (!scrolledWindow)
            fatal_error(widget_retrieval_error);
    }
    swap_main_widget(listScreen);

    //TODO: Load and render actual list
    clear_list();
    for (int i = 0; i < 100; i++)
        render_material_item((pb_material_item*)i);
}