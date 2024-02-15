
#ifndef GTK_TP_GTK_ADD_IP_H
#define GTK_TP_GTK_ADD_IP_H

#include <gtk/gtk.h>
#include "../process.h"





void show_add_ip_dialog(GtkWidget *widget, gpointer user_data) {


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

    GtkWidget *ipv4_label = gtk_label_new("Adresse IPv4:");
    GtkWidget *ipv4_entry = gtk_entry_new();
    GtkWidget *mask_label = gtk_label_new("Masque:");
    GtkWidget *mask_entry = gtk_entry_new();


    gtk_container_add(GTK_CONTAINER(content_area), ipv4_label);
    gtk_container_add(GTK_CONTAINER(content_area), ipv4_entry);
    gtk_container_add(GTK_CONTAINER(content_area), mask_label);
    gtk_container_add(GTK_CONTAINER(content_area), mask_entry);


    gtk_widget_show_all(dialog);


    int running = 1;

    while(running) {


        gint result = gtk_dialog_run(GTK_DIALOG(dialog));


        if (result == GTK_RESPONSE_OK) {

            const char *ipv4_text = gtk_entry_get_text(GTK_ENTRY(ipv4_entry));
            const char *mask_text = gtk_entry_get_text(GTK_ENTRY(mask_entry));

            char *printing = malloc(sizeof(char) * 100);


            char ipv4_address[16];
            char mask[16];
            char binary_mask[36];
            char binary_result[36];
            char hex_result[12];
            char network[36];
            sqlite3 *db;

            if (sqlite3_open("../bdd.sqlite", &db) != SQLITE_OK) {
                printing = "Cannot open database \n";
                sqlite3_close(db);
            }

            if (sqlite3_exec(db, "SELECT * FROM Address", NULL, NULL, NULL) != SQLITE_OK) {
                create_bdd(db);
            }


            if (!verify_format_ipv4(ipv4_text) && (!verify_format_mask(mask_text))) {
                printing = "IP et MASK incorrect \n";
            };

            if (!verify_format_ipv4(ipv4_text)) {
                printing = "Format de l'ip INCORRECT \n";
            };

            if (!verify_format_mask(mask_text)) {
                printing = "Format du MASK INCORRECT \n";
            };

            if (verify_format_ipv4(ipv4_text) && (verify_format_mask(mask_text))) {
                printing = "IP dans la database \n";


                convert_IPV4(ipv4_text, binary_result, mask_text, binary_mask, hex_result, network);

                //char * ip_buff = malloc(sizeof(char)*25);

                insert_ip_data(db, ipv4_text, mask_text, binary_result, binary_mask, hex_result, network);
                sqlite3_close(db);

                gtk_widget_destroy(ipv4_entry);
                gtk_widget_destroy(mask_entry);
                gtk_widget_destroy(mask_label);
                gtk_widget_destroy(ipv4_label);
            };

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
