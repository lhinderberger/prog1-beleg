/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */

#include "frontend/globals.h"
#include "frontend/error.h"
#include "frontend/list.h"

#include <glib/gi18n.h>

GtkGrid * listGrid = NULL;
GtkWidget * listScreen = NULL;
GtkScrolledWindow * scrolledWindow = NULL;

int n_items_displayed = 0;

void add_list_heading(int column, const char * label, int expand) {
    /* Create formatted label markup */
    char fmt_label[128];
    if (sprintf(fmt_label, "<b>%s</b>", label) < 0)
        fatal_error("sprintf failed for list heading");

    /* Create label */
    GtkWidget * lbl_widget = gtk_label_new(NULL);
    if (!lbl_widget)
        fatal_error(widget_creation_error);
    gtk_label_set_markup((GtkLabel*)lbl_widget, fmt_label);
    gtk_label_set_xalign((GtkLabel*)lbl_widget, 0.0);
    if (expand)
        gtk_widget_set_hexpand(lbl_widget, TRUE);

    /* Add to grid */
    gtk_grid_attach(listGrid, lbl_widget, column, 0, 1, 1);
    gtk_widget_show(lbl_widget);
}

void add_label(int row, int column, const char * label) {
    /* Create label */
    GtkWidget * glabel = gtk_label_new(label);
    if (!glabel)
        fatal_error(widget_creation_error);
    gtk_label_set_xalign((GtkLabel*)glabel, 0.0);

    /* Add to grid */
    gtk_grid_attach(listGrid, glabel, column, row, 1, 1);
    gtk_widget_show(glabel);
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
    gtk_grid_set_row_spacing(listGrid, 5);
    gtk_grid_set_column_spacing(listGrid, 10);
    gtk_widget_set_hexpand((GtkWidget*)listGrid, TRUE);
    gtk_container_add((GtkContainer*)scrolledWindow, (GtkWidget*)listGrid);

    /* Add headings */
    add_list_heading(1, C_("List headings", "Artikelname"), 1);
    add_list_heading(2, C_("List headings", "Artikelnummer"), 1);
    add_list_heading(3, C_("List headings", "Lagerbestand"), 0);
    add_list_heading(4, C_("List headings", "Aktion       "), 0);

    /* Show widget */
    gtk_widget_show((GtkWidget*)listGrid);
}

void render_material_item(pb_material_item * item) {
    g_assert(item);
    int row = 1 + n_items_displayed;

    /* Add image column */
    //TODO: Read actual image from DB
    GtkWidget * art_image_frame = gtk_frame_new("");
    if (!art_image_frame)
        fatal_error(widget_creation_error);

    GtkWidget * art_image = gtk_image_new_from_icon_name("gtk-missing-image", GTK_ICON_SIZE_BUTTON);
    if (!art_image)
        fatal_error(widget_creation_error);
    gtk_widget_set_size_request(art_image, 80, 80);

    gtk_container_add((GtkContainer*)art_image_frame, art_image);
    gtk_grid_attach(listGrid, art_image_frame, 0, row, 1, 1);
    gtk_widget_show(art_image_frame);
    gtk_widget_show(art_image);


    /* Add article name column */
    add_label(row, 1, item->name);

    /* Add article id column */
    add_label(row, 2, item->article_no);

    /* Add n_stock column */
    //TODO: Make editable by attaching to signal
    GtkAdjustment * n_stock_adjustment = gtk_adjustment_new(item->n_stock, 0.0, 9999999999.0, 1.0, 5.0, 0.0);
    if (!n_stock_adjustment)
        fatal_error("Could not create adjustment for n_stock");
    GtkWidget * n_stock_editor = gtk_spin_button_new(n_stock_adjustment, 1.0, 0);
    gtk_grid_attach(listGrid, n_stock_editor, 3, row, 1, 1);
    gtk_widget_show(n_stock_editor);

    /* Add edit button */
    GtkWidget * edit_btn = gtk_button_new_from_icon_name("gtk-edit", GTK_ICON_SIZE_BUTTON);
    if (!edit_btn)
        fatal_error(widget_creation_error);
    gtk_button_set_always_show_image((GtkButton*)edit_btn, TRUE);
    gtk_widget_set_halign(edit_btn, GTK_ALIGN_START);
    gtk_widget_set_valign(edit_btn, GTK_ALIGN_CENTER);
    gtk_grid_attach(listGrid, edit_btn, 4, row, 1, 1);
    gtk_widget_show(edit_btn);

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