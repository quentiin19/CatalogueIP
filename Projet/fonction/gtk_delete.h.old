#ifndef GTK_TP_GTK_DELETE_H
#define GTK_TP_GTK_DELETE_H

#include <gtk/gtk.h>




void delete_ip(GtkWidget *widget, gpointer user_data) {


    // Create a dialog
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Ajouter IP",
                                                    GTK_WINDOW(user_data),
                                                    GTK_DIALOG_MODAL,
                                                    "Fermer",
                                                    GTK_RESPONSE_CANCEL,
                                                    "supprimer",
                                                    GTK_RESPONSE_OK,
                                                    NULL);

    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 300);


    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    GtkWidget *id_label = gtk_label_new("ID de l'ip a supprimer");
    GtkWidget *id_entry = gtk_entry_new();


    gtk_container_add(GTK_CONTAINER(content_area), id_label);
    gtk_container_add(GTK_CONTAINER(content_area), id_entry);




    gtk_widget_show_all(dialog);


    int running = 1;

    GtkWidget *message = gtk_label_new(NULL);

    while(running) {


        gint result = gtk_dialog_run(GTK_DIALOG(dialog));


        if (result == GTK_RESPONSE_OK) {

            const char *id_text = gtk_entry_get_text(GTK_ENTRY(id_entry));


            char *printing = malloc(sizeof(char) * 100);

            sqlite3 *db;

            if (sqlite3_open("../bdd.sqlite", &db) != SQLITE_OK) {
                printing = "Cannot open database \n";
                sqlite3_close(db);
            }

            char * buffer = malloc(sizeof(char)*100);

            sprintf(buffer,"DELETE FROM Address WHERE id=%s;", id_text);

            if (sqlite3_exec(db, buffer, NULL, NULL, NULL) != SQLITE_OK) {
                create_bdd(db);
                printing = "Il faut que vous rentriez un ID \n";
            }else{
                printing = "IP a bien été supprimé\n";
            }




            gtk_label_set_text(GTK_LABEL(message), g_strdup_printf(printing));
            gtk_container_add(GTK_CONTAINER(content_area), message);



            gtk_widget_show_all(dialog);



        } else if (result == GTK_RESPONSE_CANCEL) {
            running = 0;
        }

    }
    gtk_widget_destroy(dialog);


}

#endif


