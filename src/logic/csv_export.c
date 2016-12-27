/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "common/error.private.h"
#include "logic/csv_export.h"

char * escape_string(const char * in) {
    pb_clear_error();

    /* Count quotation marks */
    size_t len = 0;
    size_t n_quotes = 0;
    for (len = 0; in[len]; len++) {
        if (in[len] == '"')
            n_quotes++;
    }

    /* Allocate buffer */
    char * out = (char*)calloc(1 + len + n_quotes, sizeof(char));
    if (!out) {
        pb_error(PB_E_MALLOC);
        return NULL;
    }

    /* Copy and escape string */
    int i = 0;
    int j = 0;
    while (in[i]) {
        if (in[i] == '"') {
            out[j] = '"';
            j++;
        }
        out[j] = in[i];
        j++;
        i++;
    }

    return out;
}

int pb_csv_export_mat_items(pb_material_item_buffer items, int n_items, const char * filename) {
    pb_clear_error();

    if (!items || !filename)
        return pb_error(PB_E_NULLPTR);

    /* Try to open file */
    FILE * f = !strcmp(filename, "stdout") ? stdout : fopen(filename, "w");
    if (!f)
        return pb_custom_error("Could not open file for CSV export");

    /* Print comment line */
    fputs("id\tname\tarticle_no\tn_stock\timage_id\n", f);

    /* Iterate over all items, print every item to file */
    char * escaped_name = NULL;
    char * escaped_article_no = NULL;
    for (int i = 0; i < n_items; i++) {
        /* Check pointer */
        if (!items[i]) {
            fclose(f);
            return pb_error(PB_E_NULLPTR);
        }
        pb_material_item * item = items[i];

        /* Escape strings */
        escaped_name = escape_string(item->name);
        if (!escaped_name) {
            fclose(f);
            return pb_errno();
        }
        escaped_article_no = escape_string(item->article_no);
        if (!escaped_article_no) {
            free(escaped_name);
            fclose(f);
            return pb_errno();
        }

        /* Write line to file */
        fprintf(f, "%d,\t\"%s\",\t\"%s\",\t%d,\t%d\n", item->id, escaped_name, escaped_article_no, item->n_stock, item->image_id);

        /* Free escaped strings again */
        free(escaped_name);
        free(escaped_article_no);
        escaped_name = escaped_article_no = NULL;
    }

    fclose(f);
    return 0;
}