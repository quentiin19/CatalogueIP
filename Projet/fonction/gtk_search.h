
#ifndef GTK_TP_GTK_SEARCH_H
#define GTK_TP_GTK_SEARCH_H

#include <gtk/gtk.h>




int searchh(sqlite3 *db) {
    char *err_msg = 0;
    int rc;
    char mask[16];
    char ipv4_address[16];
    char binary_mask[36];
    char binary_result[36];
    char network[36];



        scanf("%s", mask);

    convert_ipv4_to_binary(ipv4_address, binary_result);
    convert_ipv4_to_binary(mask, binary_mask);

    for (int i = 0; i < 36; i++) {
        if (binary_result[i] == '1' && binary_mask[i] == '1') {
            network[i] = '1';
        } else {
            network[i] = '0';
        }
    }
    network[36] = '\0';

    char sql[100];

    sprintf(sql, "SELECT IPV4 FROM Address WHERE Network = '%s'", network);

    printf("\nIp dans le même réseau que %s : \n\n", ipv4_address);

    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }


    sqlite3_close(db);
    return 0;
}



int show_ip(GtkWidget *widget, gpointer user_data) {
    sqlite3 *db;

    char *err_msg = 0;
    int rc;
    char mask[16];
    char ipv4_address[16];
    char binary_mask[36];
    char binary_result[36];
    char network[36];



    // Create a dialog
    GtkWidget *dialog = gtk_dialog_new_with_buttons("rechercher IP",
                                                    GTK_WINDOW(user_data),
                                                    GTK_DIALOG_MODAL,
                                                    "Fermer",
                                                    GTK_RESPONSE_CANCEL,
                                                    "Rechercher",
                                                    GTK_RESPONSE_OK,
                                                    NULL);

    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 300);


    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    GtkWidget *ipv4_label = gtk_label_new("Adresse IPv4:");
    GtkWidget *ipv4_entry = gtk_entry_new();
    GtkWidget *mask_label = gtk_label_new("MASK :");
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


            if (verify_format_ipv4(ipv4_text)) {

                gtk_widget_destroy(ipv4_entry);
                gtk_widget_destroy(ipv4_label);
                gtk_widget_destroy(mask_entry);
                gtk_widget_destroy(mask_label);



                //INSERT


                if (sqlite3_open("../bdd.sqlite", &db) != SQLITE_OK) {
                    return 1;
                }


                GtkWidget *treeview;
                GtkListStore *store;
                GtkCellRenderer *renderer;
                GtkTreeViewColumn *column;
                QueryData queryData;


                store = gtk_list_store_new(6, G_TYPE_STRING, G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING);  //fAUt preciser en fonction des colonnes
                queryData.store = store;

                treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
                gtk_container_add(GTK_CONTAINER(content_area), treeview);



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

                char *err_msg = NULL;
                printf("salut 1");





                convert_ipv4_to_binary(ipv4_text, binary_result);
                convert_ipv4_to_binary(mask_text, binary_mask);

                for (int i = 0; i < 36; i++) {
                    if (binary_result[i] == '1' && binary_mask[i] == '1') {
                        network[i] = '1';
                    } else {
                        network[i] = '0';
                    }
                }
                network[36] = '\0';
                printf("%s",network);

                char * buffer2 = malloc(sizeof(char)*100);

                sprintf(buffer2, "SELECT Id, IPV4, Binary_IPV4, Mask, Binary_mask, Hexadecimal FROM Address WHERE Network = '%s'", network);


                if (sqlite3_exec(db, buffer2, display, &queryData, &err_msg) != SQLITE_OK) {
                    fprintf(stderr, "Erreur SQL : %s\n", err_msg);
                    printf("salut erreur sql");
                    sqlite3_free(err_msg);
                }


                //sqlite3_close(db);

            //INSERT

            }
            gtk_widget_show_all(content_area);
            gtk_widget_show_all(dialog);



        } else if (result == GTK_RESPONSE_CANCEL) {
            running = 0;
        }
    }
    //gtk_widget_destroy(dialog);

    return 0;

}










#endif
