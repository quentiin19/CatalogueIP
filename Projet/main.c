#include <gtk/gtk.h>

#include "fonction/gtk_add_ip.h"
#include "fonction/gtk_search_all.h"
#include "fonction/gtk_search.h"
#include "fonction/gtk_delete.h"


static void on_activate(GtkApplication *app) {
    // Create a new window
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
    //choix
    GtkWidget *button_add_ip = gtk_button_new_with_label("Ajouter IP");
    g_signal_connect_swapped(button_add_ip, "clicked", G_CALLBACK(show_add_ip_dialog), window);
    gtk_box_pack_start(GTK_BOX(main_box), button_add_ip, FALSE, FALSE, 10);
    //choix
    GtkWidget *button_print_all = gtk_button_new_with_label("Afficher tout");
    g_signal_connect_swapped(button_print_all, "clicked", G_CALLBACK(show_all_ip), window);
    gtk_box_pack_start(GTK_BOX(main_box), button_print_all, FALSE, FALSE, 10);
    //choix
    GtkWidget *button_search_ip = gtk_button_new_with_label("Rechercher IP");
    g_signal_connect_swapped(button_search_ip, "clicked", G_CALLBACK(show_ip), window);
    gtk_box_pack_start(GTK_BOX(main_box), button_search_ip, FALSE, FALSE, 10);
    //choix
    GtkWidget *button_delete_ip = gtk_button_new_with_label("Effacer IP");
    g_signal_connect_swapped(button_delete_ip, "clicked", G_CALLBACK(delete_ip), window);
    gtk_box_pack_start(GTK_BOX(main_box), button_delete_ip, FALSE, FALSE, 10);

    GtkWidget *button_quit = gtk_button_new_with_label("QUITTER");
    g_signal_connect_swapped(button_quit, "clicked", G_CALLBACK(gtk_window_close), window);
    gtk_box_pack_start(GTK_BOX(main_box), button_quit, FALSE, FALSE, 10);


    // Ajouter les elements

    gtk_container_add(GTK_CONTAINER(window), main_box);


    // Montrer tout
    gtk_widget_show_all(window);
}

int main(int argc, char *argv[]) {
    GtkApplication *app = gtk_application_new("ESGI.app.exam.langagec", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    return g_application_run(G_APPLICATION(app), argc, argv);
}
