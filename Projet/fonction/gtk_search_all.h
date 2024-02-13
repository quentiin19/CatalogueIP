
#ifndef GTK_TP_GTK_SEARCH_ALL_H
#define GTK_TP_GTK_SEARCH_ALL_H

#include <gtk/gtk.h>

int display_all();

#include <gtk/gtk.h>
#include "../process.h"
#include <gtk/gtk.h>




void show_all_ip(GtkWidget *widget, gpointer user_data) {


    // Create a dialog
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Ajouter IP",
                                                    GTK_WINDOW(user_data),
                                                    GTK_DIALOG_MODAL,
                                                    "Fermer",
                                                    GTK_RESPONSE_CANCEL,
                                                    "Ajouter",
                                                    GTK_RESPONSE_OK,
                                                    NULL);

    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 300);


    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));


    gtk_widget_show_all(dialog);


    int running = 1;

    while(running) {

        gint result = gtk_dialog_run(GTK_DIALOG(dialog));

        if (result == GTK_RESPONSE_OK) {


            char *printing = malloc(sizeof(char) * 100);


            sqlite3 *db;

            if (sqlite3_open("../bdd.sqlite", &db) != SQLITE_OK) {
                printing = "Cannot open database \n";
                sqlite3_close(db);
            }

            if (sqlite3_exec(db, "SELECT * FROM Address", NULL, NULL, NULL) != SQLITE_OK) {
                create_bdd(db);
            }

                sqlite3_close(db);


            printing = (char *) display_all();

            GtkWidget *message = gtk_label_new(NULL);

            gtk_label_set_text(GTK_LABEL(message), g_strdup_printf(printing));
            gtk_container_add(GTK_CONTAINER(content_area), message);

            gtk_widget_show_all(dialog);


        } else if (result == GTK_RESPONSE_CANCEL) {
            running = 0;
        }
    }
    gtk_widget_destroy(dialog);


}

#endif //GTK_TP_GTK_ADD_IP_H


int display_all(){
    sqlite3 *db;
    char *err_msg = 0;
    int rc;

    rc = sqlite3_open("../bdd.sqlite", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    char *sql = "SELECT * FROM Address";

    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    sqlite3_close(db);
    return 0;
}




#endif //GTK_TP_GTK_SEARCH_ALL_H
