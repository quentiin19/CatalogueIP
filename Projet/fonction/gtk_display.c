#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <gtk/gtk.h>

#include "function.h"

#include "gtk_display.h"



// add
void gtk_display_add_ip(GtkWidget *widget, gpointer window){

    // Create a dialog window
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Ajouter IP",
                                                    GTK_WINDOW(window),
                                                    GTK_DIALOG_MODAL,
                                                    "Fermer",
                                                    GTK_RESPONSE_CANCEL,
                                                    "Ajouter",
                                                    GTK_RESPONSE_OK,
                                                    NULL);

    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 300);

    g_signal_connect(dialog, "destroy", G_CALLBACK(gtk_main_quit), NULL);


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


    char * result_message;
    GtkWidget *result_message_label = NULL;


    int running = 1;

    while(running) {


        gint result = gtk_dialog_run(GTK_DIALOG(dialog));


        if (result == GTK_RESPONSE_OK) {

            const char *ipv4_text = gtk_entry_get_text(GTK_ENTRY(ipv4_entry));
            const char *mask_text = gtk_entry_get_text(GTK_ENTRY(mask_entry));


            if (verify_format_ipv4(ipv4_text) && (verify_format_mask(mask_text))) {
                result_message = "IP et MASK incorrect \n";

            }else if (verify_format_ipv4(ipv4_text)) {
                result_message = "Format de l'ip INCORRECT \n";

            }else if (verify_format_mask(mask_text)) {
                result_message = "Format du MASK INCORRECT \n";

            }else{
                result_message = "IP dans la database \n";


                sqlite3 * db;

                if(open_bdd(&db)){
                    fprintf(stderr, "Can't open database.\n");
                    
                    if (create_bdd(db)){
                        fprintf(stderr, "Error while creating database.\n");
                        return;
                    }
                }

                // if(ip_exist(db, ipv4_text, mask_text)){
                //     result_message = "L'ip existe déjà dans la base de données";

                // }else{
                    add_ip_to_db(db, ipv4_text, mask_text);
                // }

                sqlite3_close(db);
            };


            if (result_message_label != NULL){
                gtk_widget_destroy(result_message_label);
            }

            result_message_label = gtk_label_new(NULL);

            gtk_label_set_text(GTK_LABEL(result_message_label), g_strdup_printf(result_message));
            gtk_container_add(GTK_CONTAINER(content_area), result_message_label);

            gtk_widget_show_all(dialog);

            // gtk_container_remove(GTK_CONTAINER(content_area), result_message_label);


        } else if (result == GTK_RESPONSE_CANCEL) {
            running = 0;
            
        }
    }
    gtk_widget_destroy(dialog);
}


// display all
int display_ip_array(void *store_widget, int count, char **data, char **column_name){
    GtkListStore *store = store_widget;
    GtkTreeIter iter;

    gtk_list_store_append(store, &iter);

    for (int i = 0; i < count; i++) {
        gtk_list_store_set(store, &iter, i, data[i] ? data[i] : "NULL", -1);
    }

    return 0;
}

int gtk_display_all_ip(GtkWidget *widget, gpointer window){

    // Create a dialog window
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Affichage de toutes les IP",
                                                    GTK_WINDOW(window),
                                                    GTK_DIALOG_MODAL,
                                                    "Fermer",
                                                    GTK_RESPONSE_CANCEL,
                                                    NULL);

    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 300);

    g_signal_connect(dialog, "destroy", G_CALLBACK(gtk_main_quit), NULL);


    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));


    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    gtk_widget_set_size_request(scrolled_window, 400, 300);


    GtkWidget *treeview;
    GtkListStore *store;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;


    store = gtk_list_store_new(6, G_TYPE_STRING, G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING);  //fAUt preciser en fonction des colonnes

    treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    gtk_container_add(GTK_CONTAINER(scrolled_window), treeview);


    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("IPV4", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Mask", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Binaire IP", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Binaire mask", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Hexadecimal", renderer, "text", 5, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);


    sqlite3 * db;

    if(open_bdd(&db)){
        fprintf(stderr, "Can't open database.\n");
        
        if (create_bdd(db)){
            fprintf(stderr, "Error while creating database.\n");
            return 1;
        }
    }


    char *err_msg = NULL;
    if (sqlite3_exec(db, "SELECT Id, IPV4, Binary_IPV4, Mask, Binary_mask, Hexadecimal FROM Address;", display_ip_array, store, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "Erreur SQL : %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    gtk_container_add(GTK_CONTAINER(content_area), scrolled_window);

    gtk_widget_show_all(dialog);

    gtk_dialog_run(GTK_DIALOG(dialog));            
    gtk_widget_destroy(dialog);
}


// display search
int gtk_display_search_ip(GtkWidget *widget, gpointer window){

    // Create a dialog window
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Ajouter IP",
                                                    GTK_WINDOW(window),
                                                    GTK_DIALOG_MODAL,
                                                    "Fermer",
                                                    GTK_RESPONSE_CANCEL,
                                                    "Rechercher",
                                                    GTK_RESPONSE_OK,
                                                    NULL);

    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 300);

    g_signal_connect(dialog, "destroy", G_CALLBACK(gtk_main_quit), NULL);


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


    char * result_message = NULL;
    GtkWidget *scrolled_window = NULL;


    int running = 1;

    while(running) {


        gint result = gtk_dialog_run(GTK_DIALOG(dialog));


        if (result == GTK_RESPONSE_OK) {


            const char *ipv4_text = gtk_entry_get_text(GTK_ENTRY(ipv4_entry));
            const char *mask_text = gtk_entry_get_text(GTK_ENTRY(mask_entry));


            if (verify_format_ipv4(ipv4_text) && (verify_format_mask(mask_text))) {
                result_message = "IP et MASK incorrect \n";

            }else if (verify_format_ipv4(ipv4_text)) {
                result_message = "Format de l'ip INCORRECT \n";

            }else if (verify_format_mask(mask_text)) {
                result_message = "Format du MASK INCORRECT \n";

            }else{
                result_message = "IP dans la database \n";

                if(scrolled_window != NULL){
                    gtk_widget_destroy(scrolled_window);
                }

                scrolled_window = gtk_scrolled_window_new(NULL, NULL);
                gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                            GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

                gtk_widget_set_size_request(scrolled_window, 400, 300);


                GtkWidget *treeview;
                GtkListStore *store;
                GtkCellRenderer *renderer;
                GtkTreeViewColumn *column;


                store = gtk_list_store_new(6, G_TYPE_STRING, G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING);  //fAUt preciser en fonction des colonnes

                treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
                gtk_container_add(GTK_CONTAINER(scrolled_window), treeview);


                renderer = gtk_cell_renderer_text_new();
                column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
                gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

                renderer = gtk_cell_renderer_text_new();
                column = gtk_tree_view_column_new_with_attributes("IPV4", renderer, "text", 1, NULL);
                gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

                renderer = gtk_cell_renderer_text_new();
                column = gtk_tree_view_column_new_with_attributes("Mask", renderer, "text", 3, NULL);
                gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

                renderer = gtk_cell_renderer_text_new();
                column = gtk_tree_view_column_new_with_attributes("Binaire IP", renderer, "text", 2, NULL);
                gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

                renderer = gtk_cell_renderer_text_new();
                column = gtk_tree_view_column_new_with_attributes("Binaire mask", renderer, "text", 4, NULL);
                gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

                renderer = gtk_cell_renderer_text_new();
                column = gtk_tree_view_column_new_with_attributes("Hexadecimal", renderer, "text", 5, NULL);
                gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);


                sqlite3 * db;

                if(open_bdd(&db)){
                    fprintf(stderr, "Can't open database.\n");
                    
                    if (create_bdd(db)){
                        fprintf(stderr, "Error while creating database.\n");
                        return 1;
                    }
                }

                char * network = malloc(sizeof(char) * 33);
                get_network(ipv4_text, mask_text, network);

                char query[1024];
                sprintf(query, "SELECT Id, IPV4, Binary_IPV4, Mask, Binary_mask, Hexadecimal FROM Address WHERE Network = '%s';", network);


                if (sqlite3_exec(db, query, display_ip_array, store, NULL) != SQLITE_OK) {
                    fprintf(stderr, "Erreur SQL : %s\n", sqlite3_errmsg(db));
                }

                free(network);

                gtk_container_add(GTK_CONTAINER(content_area), scrolled_window);

                gtk_widget_show_all(dialog);
            }

        } else if (result == GTK_RESPONSE_CANCEL) {
            running = 0;
        }
    }

    gtk_widget_destroy(dialog);
}



// remove
void gtk_display_remove_ip(GtkWidget *widget, gpointer window){

    // Create a dialog window
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Supprimer une IP",
                                                    GTK_WINDOW(window),
                                                    GTK_DIALOG_MODAL,
                                                    "Fermer",
                                                    GTK_RESPONSE_CANCEL,
                                                    "Supprimer",
                                                    GTK_RESPONSE_OK,
                                                    NULL);

    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 300);

    g_signal_connect(dialog, "destroy", G_CALLBACK(gtk_main_quit), NULL);


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


    char * result_message = NULL;
    GtkWidget *result_message_label = NULL;


    int running = 1;

    while(running) {


        gint result = gtk_dialog_run(GTK_DIALOG(dialog));


        if (result == GTK_RESPONSE_OK) {

            const char *ipv4_text = gtk_entry_get_text(GTK_ENTRY(ipv4_entry));
            const char *mask_text = gtk_entry_get_text(GTK_ENTRY(mask_entry));


            if (verify_format_ipv4(ipv4_text) && (verify_format_mask(mask_text))) {
                result_message = "IP et MASK incorrect \n";

            }else if (verify_format_ipv4(ipv4_text)) {
                result_message = "Format de l'ip INCORRECT \n";

            }else if (verify_format_mask(mask_text)) {
                result_message = "Format du MASK INCORRECT \n";

            }else{
                // result_message = "IP dans la database \n";


                sqlite3 * db;

                if(open_bdd(&db)){
                    fprintf(stderr, "Can't open database.\n");
                    
                    if (create_bdd(db)){
                        fprintf(stderr, "Error while creating database.\n");
                        return;
                    }
                }

                if(delete_ip_from_db(db, ipv4_text, mask_text)){
                    fprintf(stderr, "Error while deleting IP from database.\n");
                }

                result_message = "l'IP a bien été supprimé de la base de données";

                sqlite3_close(db);
            };

            if (result_message_label != NULL){
                gtk_widget_destroy(result_message_label);
            }

            result_message_label = gtk_label_new(NULL);
            
            gtk_label_set_text(GTK_LABEL(result_message_label), g_strdup_printf(result_message));
            gtk_container_add(GTK_CONTAINER(content_area), result_message_label);

            gtk_widget_show_all(dialog);

            // gtk_container_remove(GTK_CONTAINER(content_area), result_message_label);


        } else if (result == GTK_RESPONSE_CANCEL) {
            running = 0;
            
        }
    }
    gtk_widget_destroy(dialog);
}




// main menu
void gtk_display_menu(GtkApplication *app) {

    // Create a new window
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "IP catalogue - ESGI");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);

    // Create a new label with the desired text
    GtkWidget *label = gtk_label_new("IP catalogue");

    //configuration de la fenêtre
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);


    //configuration des box
    GtkWidget * main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_box);


    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);


    // Menu
    //choix ajouter
    GtkWidget *button_add_ip = gtk_button_new_with_label("Ajouter IP");
    g_signal_connect_swapped(button_add_ip, "clicked", G_CALLBACK(gtk_display_add_ip), window);
    gtk_box_pack_start(GTK_BOX(main_box), button_add_ip, FALSE, FALSE, 10);

    //choix afficher tout
    GtkWidget *button_all_ip = gtk_button_new_with_label("Afficher tout");
    g_signal_connect_swapped(button_all_ip, "clicked", G_CALLBACK(gtk_display_all_ip), window);
    gtk_box_pack_start(GTK_BOX(main_box), button_all_ip, FALSE, FALSE, 10);

    //choix rechercher
    GtkWidget *button_search_ip = gtk_button_new_with_label("Rechercher IP");
    g_signal_connect_swapped(button_search_ip, "clicked", G_CALLBACK(gtk_display_search_ip), window);
    gtk_box_pack_start(GTK_BOX(main_box), button_search_ip, FALSE, FALSE, 10);

    //choix effacer
    GtkWidget *button_remove_ip = gtk_button_new_with_label("Effacer IP");
    g_signal_connect_swapped(button_remove_ip, "clicked", G_CALLBACK(gtk_display_remove_ip), window);
    gtk_box_pack_start(GTK_BOX(main_box), button_remove_ip, FALSE, FALSE, 10);

    //choix quitter
    GtkWidget *button_quit = gtk_button_new_with_label("QUITTER");
    g_signal_connect_swapped(button_quit, "clicked", G_CALLBACK(gtk_window_close), window);
    gtk_box_pack_start(GTK_BOX(main_box), button_quit, FALSE, FALSE, 10);


    // Ajouter les elements
    gtk_container_add(GTK_CONTAINER(window), main_box);

    // Montrer tout
    gtk_widget_show_all(window);
}
