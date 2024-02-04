
#ifndef GTK_TP_GTK_ADD_IP_H
#define GTK_TP_GTK_ADD_IP_H

#include <gtk/gtk.h>
#include "../process.h"
#include <gtk/gtk.h>


void show_add_ip_dialog(GtkWidget *widget, gpointer user_data) {
    // Create a dialog
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Ajouter IP",
                                                    GTK_WINDOW(user_data),
                                                    GTK_DIALOG_MODAL,
                                                    "Annuler",
                                                    GTK_RESPONSE_CANCEL,
                                                    "Ajouter",
                                                    GTK_RESPONSE_OK,
                                                    NULL);

    // Create content for the dialog
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    GtkWidget *ipv4_label = gtk_label_new("Adresse IPv4:");
    GtkWidget *ipv4_entry = gtk_entry_new();
    GtkWidget *mask_label = gtk_label_new("Masque:");
    GtkWidget *mask_entry = gtk_entry_new();

    // Add content to the dialog
    gtk_container_add(GTK_CONTAINER(content_area), ipv4_label);
    gtk_container_add(GTK_CONTAINER(content_area), ipv4_entry);
    gtk_container_add(GTK_CONTAINER(content_area), mask_label);
    gtk_container_add(GTK_CONTAINER(content_area), mask_entry);

    // Show all widgets
    gtk_widget_show_all(dialog);

    // Run the dialog
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_OK) {
        // Handle user input
        const gchar *ipv4_address = gtk_entry_get_text(GTK_ENTRY(ipv4_entry));
        const gchar *mask = gtk_entry_get_text(GTK_ENTRY(mask_entry));

        // Do something with the input, for example, call your existing function
        add_ip_address(ipv4_address, mask);

        // Close the dialog
        gtk_widget_destroy(dialog);
    } else {
        // User canceled
        gtk_widget_destroy(dialog);
    }
}



int add_ip_address_tmp() {
    char ipv4_address[16]; // 15 caractères + 1 pour le caractère nul
    char mask[16];        // 15 caractères + 1 pour le caractère nul
    char binary_mask[36]; // 35 caractères + 1 pour le caractère nul
    char binary_result[36]; // 35 caractères + 1 pour le caractère nul
    char hex_result[12];    // 11 caractères + 1 pour le caractère nul
    char network[36];       // 35 caractères + 1 pour le caractère nul
    sqlite3 *db;

    // Ouvrir la base de données et créer la table
    if (sqlite3_open("bdd.sqlite", &db) != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    if (sqlite3_exec(db, "SELECT * FROM Address", NULL, NULL, NULL) != SQLITE_OK) {
        create_bdd(db);
    }

    do {
        printf("\nSaisir une adresse IPv4: ");
        scanf("%s", &ipv4_address);
    } while (!verify_format_ipv4(ipv4_address));

    do {
        printf("Saisir un masque: ");
        scanf("%s", &mask);
    } while (!verify_format_mask(mask));

    // Convertir l'adresse IPv4 et obtenir les résultats
    convert_IPV4(ipv4_address, binary_result, mask, binary_mask, hex_result, network);

    // Insérer les données dans la base de données
    insert_ip_data(db, ipv4_address, mask, binary_result, binary_mask , hex_result, network);

    sqlite3_close(db);
    return 0;
}






#endif //GTK_TP_GTK_ADD_IP_H
