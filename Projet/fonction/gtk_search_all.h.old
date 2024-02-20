#ifndef GTK_TP_GTK_SEARCH_ALL_H
#define GTK_TP_GTK_SEARCH_ALL_H

#include <gtk/gtk.h>



//#include "../process.h"

typedef struct {
    GtkListStore *store;
    int num_cols;
} QueryData;


int display(void *data, int argc, char **argv, char **azColName) {
    QueryData *queryData = (QueryData *)data;
    GtkListStore *store = queryData->store;
    GtkTreeIter iter;

    gtk_list_store_append(store, &iter);



    for (int i = 0; i < argc; i++) {
        gtk_list_store_set(store, &iter, i, argv[i] ? argv[i] : "NULL", -1);
        printf("%s\n",argv[i]);
    }

    return 0;
}


int show_all_ip() {
    sqlite3 *db;

    if (sqlite3_open("../bdd.sqlite", &db) != SQLITE_OK) {
        return 1;
    }

    GtkWidget *window, *treeview;
    GtkListStore *store;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    QueryData queryData;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Résultats de la requête");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    store = gtk_list_store_new(6, G_TYPE_STRING, G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING);  //fAUt preciser en fonction des colonnes
    queryData.store = store;

    treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    gtk_container_add(GTK_CONTAINER(window), treeview);



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
    if (sqlite3_exec(db, "SELECT Id, IPV4, Binary_IPV4, Mask, Binary_mask, Hexadecimal FROM Address;", display, &queryData, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "Erreur SQL : %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    gtk_widget_show_all(window);

    sqlite3_close(db);
    return 0;
}




#endif //GTK_TP_GTK_SEARCH_ALL_H
