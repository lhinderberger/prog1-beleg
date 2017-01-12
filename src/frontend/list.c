#include "frontend/globals.h"
#include "frontend/error.h"
#include "frontend/list.h"
#include "logic/items_query.h"

#include <glib/gi18n.h>

GtkGrid * listGrid = NULL;
GtkWidget * listScreen = NULL;
GtkScrolledWindow * scrolledWindow = NULL;

#define ITEM_BUF_SIZE 10
pb_material_item * item_buf[ITEM_BUF_SIZE];
int n_items_displayed = 0;

void add_list_heading(int column, const char * label) {
    /* Create label */
    GtkWidget * lbl_widget = gtk_label_new(label);
    if (!lbl_widget)
        fatal_error(widget_creation_error);

    /* Add to grid */
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
    n_items_displayed = 0;
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
    g_assert(item);

    /*GtkWidget * dummybutton = gtk_button_new();
    char buf[512];
    sprintf(buf, "Dummy Button %d", (int)item);
    gtk_button_set_label((GtkButton*)dummybutton, buf);
    gtk_grid_attach(listGrid, dummybutton, 0, 1 + n_items, 1, 1);
    gtk_widget_show(dummybutton);
    n_items++;*/
    //TODO: Render actual material item

    //TODO: Add image column

    /* Add article name column */
    GtkLabel * art_name_label = gtk_label_new(item->name);
    if (!art_name_label)
        fatal_error(widget_creation_error);
    gtk_grid_attach(listGrid, art_name_label, 1, n_items_displayed, 1, 1);
    gtk_widget_show(art_name_label);

    /* Add article id column */
    GtkLabel * art_id_label = gtk_label_new(item->article_no);
    if (!art_id_label)
        fatal_error(widget_creation_error);
    gtk_grid_attach(listGrid, art_id_label, 2, n_items_displayed, 1, 1);
    gtk_widget_show(art_id_label);

    /* Add article id column */
    char n_stock_str[32];
    if (sprintf(n_stock_str, "%d", item->n_stock) < 0)
        fatal_error("sprintf failed for n_stock");

    GtkLabel * n_stock_label = gtk_label_new(n_stock_str);
    if (!n_stock_label)
        fatal_error(widget_creation_error);
    gtk_grid_attach(listGrid, n_stock_label, 3, n_items_displayed, 1, 1);
    gtk_widget_show(n_stock_label);

    /* Finalize */
    n_items_displayed++;
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

    int total_items = pb_list_mat_items(db, item_buf, PB_MAT_ITEM_VAR_NAME, 1, 0, ITEM_BUF_SIZE); //TODO: Check return value
    for (int i = 0; i < total_items; i++)
        render_material_item(item_buf[i]);
}