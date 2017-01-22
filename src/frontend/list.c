/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */

#include "common/error.h"
#include "common/list.h"
#include "frontend/globals.h"
#include "frontend/editor.h"
#include "frontend/error.h"
#include "frontend/list.h"

#include <math.h>
#include <glib/gi18n.h>

GtkGrid * listGrid = NULL;
GtkLabel * pageLabel = NULL;
GtkWidget * listScreen = NULL;
GtkContainer * listDummyBox = NULL;
GtkButton * currentSortingArrow = NULL;

int listControlsWired = 0;
int n_items_displayed = 0;
int page = 0;
int n_pages = 0;
int asc = 1;
int sort_field = PB_MAT_ITEM_VAR_NAME;

void render_page();

void sorting_arrow_clicked(GtkButton * sorting_arrow, gpointer user_data) {
    /* Determine new sorting and paging parameters */
    int new_sort_field = (int)user_data;
    asc = new_sort_field == sort_field ? !asc : 1;
    sort_field = new_sort_field;
    page = 0;

    /* Re-render (this also deletes and re-renders all sorting arrow buttons) */
    render_page();

    /* Set current sorting arrow button icon */
    gtk_button_set_image(currentSortingArrow, gtk_image_new_from_icon_name(asc ? "gtk-sort-ascending" : "gtk-sort-descending", GTK_ICON_SIZE_SMALL_TOOLBAR));
}

void add_list_heading(int column, const char * label, int expand, int sorting_column) {
    /* Create formatted label markup */
    char fmt_label[128];
    if (sprintf(fmt_label, "<b>%s</b>", label) < 0)
        fatal_error("sprintf failed for list heading");

    /* Create heading box */
    GtkBox * heading_box = (GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    if (!heading_box)
        fatal_error(widget_creation_error);
    gtk_widget_set_hexpand((GtkWidget*)heading_box, TRUE);

    /* Create label */
    GtkWidget * lbl_widget = gtk_label_new(NULL);
    if (!lbl_widget)
        fatal_error(widget_creation_error);
    gtk_label_set_markup((GtkLabel*)lbl_widget, fmt_label);
    gtk_label_set_xalign((GtkLabel*)lbl_widget, 0.0);
    if (expand)
        gtk_widget_set_hexpand(lbl_widget, TRUE);
    gtk_container_add((GtkContainer*)heading_box, lbl_widget);
    gtk_widget_show(lbl_widget);

    /* Create sorting arrow */
    if (sorting_column != 0) {
        GtkButton * sorting_arrow = (GtkButton*)gtk_button_new_from_icon_name("gtk-sort-ascending", GTK_ICON_SIZE_SMALL_TOOLBAR);
        if (!sorting_arrow)
            fatal_error(widget_creation_error);
        gtk_container_add((GtkContainer*)heading_box, (GtkWidget*)sorting_arrow);
        g_signal_connect(sorting_arrow, "clicked", G_CALLBACK(sorting_arrow_clicked), (gpointer)sorting_column);
        gtk_widget_show((GtkWidget*)sorting_arrow);

        if (sorting_column == sort_field)
            currentSortingArrow = sorting_arrow;
    }

    /* Add to grid */
    gtk_grid_attach(listGrid, (GtkWidget*)heading_box, column, 0, 1, 1);
    gtk_widget_show((GtkWidget*)heading_box);
}

void add_label(int row, int column, const char * label) {
    /* Create label */
    GtkWidget * glabel = gtk_label_new(label);
    if (!glabel)
        fatal_error(widget_creation_error);
    gtk_label_set_xalign((GtkLabel*)glabel, 0.0);

    /* Add to grid */
    gtk_widget_set_valign(glabel, GTK_ALIGN_START);
    gtk_grid_attach(listGrid, glabel, column, row, 1, 1);
    gtk_widget_show(glabel);
}

void clear_list() {
    /* Remove old list grid, if any */
    if (listGrid) {
        gtk_widget_hide((GtkWidget*)listGrid);
        gtk_container_remove(listDummyBox, (GtkWidget*)listGrid);
        listGrid = NULL;
    }

    /* Create a new list grid */
    n_items_displayed = 0;
    listGrid = (GtkGrid*)gtk_grid_new();
    if (!listGrid)
        fatal_error(widget_creation_error);
    gtk_grid_set_row_spacing(listGrid, 5);
    gtk_grid_set_column_spacing(listGrid, 10);
    gtk_widget_set_hexpand((GtkWidget*)listGrid, TRUE);
    gtk_container_add(listDummyBox, (GtkWidget*)listGrid);

    /* Add headings */
    add_list_heading(1, C_("List headings", "Artikelname"), 1, PB_MAT_ITEM_VAR_NAME);
    add_list_heading(2, C_("List headings", "Artikelnummer"), 1, PB_MAT_ITEM_VAR_ARTICLE_NO);
    add_list_heading(3, C_("List headings", "Lagerbestand"), 0, PB_MAT_ITEM_VAR_N_STOCK);
    add_list_heading(4, C_("List headings", "Aktion       "), 0, 0);

    /* Show widget */
    gtk_widget_show((GtkWidget*)listGrid);
}

void n_stock_changed(GtkSpinButton *spin_button, gpointer user_data) {
    pb_material_item * item = (pb_material_item*)user_data;
    if (!item || !spin_button)
        return;

    /* Cache old n_stock */
    int old_n_stock = item->n_stock;

    /* Set new n_stock and try to save it */
    item->n_stock = gtk_spin_button_get_value_as_int(spin_button);
    if (!pb_mat_item_save(db, item, 1)) {
        /* Reset n_stock and display error message */
        item->n_stock = old_n_stock;
        gtk_spin_button_set_value(spin_button, old_n_stock);
        warning(C_("Warning n_stock_changed", "Der Lagerbestand konnte nicht gesetzt werden!"), pb_error_str(pb_errno()));
    }
    else {
        /* Inform user in status bar about our great success */
        gtk_statusbar_push(statusbar, 0, C_("n_stock_changed", "Lagerbestand erfolgreich geändert. Änderungen wurden gespeichert."));
    }
}

void create_btn_clicked(GtkButton *edit_btn) {
    open_editor(NULL, 1);
}

void edit_btn_clicked(GtkButton *edit_btn, gpointer user_data) {
    pb_material_item * item = (pb_material_item*)user_data;
    if (!item || !edit_btn)
        return;

    /* Open editor */
    open_editor(item, 0);
}

void render_material_item(pb_material_item * item) {
    g_assert(item);
    int row = 1 + n_items_displayed;

    /* Add image column */
    //TODO: Read actual image from DB
    //TODO: Hinzufügen zu Grid in eigene Funkton auslagern
    GtkWidget * art_image_frame = gtk_frame_new("");
    if (!art_image_frame)
        fatal_error(widget_creation_error);

    GtkWidget * art_image = gtk_image_new_from_icon_name("gtk-missing-image", GTK_ICON_SIZE_BUTTON);
    if (!art_image)
        fatal_error(widget_creation_error);
    gtk_widget_set_size_request(art_image, 80, 80);
    gtk_widget_set_valign(art_image, GTK_ALIGN_START);

    gtk_container_add((GtkContainer*)art_image_frame, art_image);
    gtk_grid_attach(listGrid, art_image_frame, 0, row, 1, 1);
    gtk_widget_show(art_image_frame);
    gtk_widget_show(art_image);


    /* Add article name column */
    add_label(row, 1, item->name);

    /* Add article id column */
    add_label(row, 2, item->article_no);

    /* Add n_stock column */
    GtkAdjustment * n_stock_adjustment = gtk_adjustment_new(item->n_stock, 0.0, 9999999999.0, 1.0, 5.0, 0.0);
    if (!n_stock_adjustment)
        fatal_error("Could not create adjustment for n_stock");
    GtkWidget * n_stock_editor = gtk_spin_button_new(n_stock_adjustment, 1.0, 0);
    gtk_widget_set_valign(n_stock_editor, GTK_ALIGN_START);
    g_signal_connect(n_stock_editor, "value-changed", G_CALLBACK(n_stock_changed), item);
    gtk_grid_attach(listGrid, n_stock_editor, 3, row, 1, 1);
    gtk_widget_show(n_stock_editor);

    /* Add edit button */
    GtkWidget * edit_btn = gtk_button_new_from_icon_name("gtk-edit", GTK_ICON_SIZE_BUTTON);
    if (!edit_btn)
        fatal_error(widget_creation_error);
    gtk_button_set_always_show_image((GtkButton*)edit_btn, TRUE);
    gtk_widget_set_valign(edit_btn, GTK_ALIGN_START);
    gtk_widget_set_halign(edit_btn, GTK_ALIGN_CENTER);
    gtk_grid_attach(listGrid, edit_btn, 4, row, 1, 1);
    gtk_widget_show(edit_btn);
    g_signal_connect(edit_btn, "clicked", G_CALLBACK(edit_btn_clicked), item);

    /* Finalize */
    n_items_displayed++;
}

void render_page() {
    clear_list();

    /* Count material items */
    //TODO: Take search into account
    n_pages = (int)ceil((double)pb_count_mat_items(db) / (double)ITEM_BUF_SIZE);
    if (n_pages < 0)
        fatal_pb_error();

    /* Adjust current page */
    if (page < 1)
        page = 1;
    else if (page > n_pages)
        page = n_pages;

    /* Set current page label */
    char buf[64];
    sprintf(buf, C_("page label format", "Seite %d/%d"), page, n_pages);
    gtk_label_set_text(pageLabel, buf);

    /* Retrieve and render current page */
    int total_items = pb_list_mat_items(db, item_buf, sort_field, asc, ITEM_BUF_SIZE * (page - 1), ITEM_BUF_SIZE);
    if (total_items < 0)
        fatal_pb_error();
    for (int i = 0; i < total_items; i++)
        render_material_item(item_buf[i]);
}

void next_page() {
    page++;
    render_page();
}

void prev_page() {
    page--;
    render_page();
}

void show_material_list() {
    /* Retrieve controls from builder */
    listScreen = checked_retrieve_widget("listScreen");
    listDummyBox = (GtkContainer*)checked_retrieve_widget("listDummyBox");
    pageLabel = (GtkLabel*)checked_retrieve_widget("pageLabel");
    GtkButton * btnNextPage = (GtkButton*)checked_retrieve_widget("nextPageButton");
    GtkButton * btnPrevPage = (GtkButton*)checked_retrieve_widget("prevPageButton");
    GtkButton * btnCreateItem = (GtkButton*)checked_retrieve_widget("btnCreateItem");

    /* Wire up controls */
    if (!listControlsWired) {
        g_signal_connect(btnCreateItem, "clicked", G_CALLBACK(create_btn_clicked), NULL);
        g_signal_connect(btnNextPage, "clicked", G_CALLBACK(next_page), NULL);
        g_signal_connect(btnPrevPage, "clicked", G_CALLBACK(prev_page), NULL);
        listControlsWired = 1;
    }

    /* Render current page of material items */
    render_page();

    /* Show list */
    swap_main_widget(listScreen);
}