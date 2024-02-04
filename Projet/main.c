#include <gtk/gtk.h>

#include "fonction/gtk_add_ip.h"

static void on_activate(GtkApplication *app) {
    // Create a new window
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "IP catalogue - ESGI");

    gtk_window_set_default_size(GTK_WINDOW(window), 800, 400);

    // Create a new label with the desired text
    GtkWidget *label = gtk_label_new("IP catalogue");

    // Create a box to hold the label
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);


    // Menu
    //choix
    GtkWidget *button_add_ip = gtk_button_new_with_label("Ajouter IP");
    g_signal_connect_swapped(button_add_ip, "clicked", G_CALLBACK(show_add_ip_dialog), window);
    gtk_box_pack_start(GTK_BOX(box), button_add_ip, FALSE, FALSE, 0);
    //choix
    GtkWidget *button_print_all = gtk_button_new_with_label("Afficher tout");
    g_signal_connect_swapped(button_print_all, "clicked", G_CALLBACK(gtk_window_close), window);
    gtk_box_pack_start(GTK_BOX(box), button_print_all, FALSE, FALSE, 0);
    //choix
    GtkWidget *button_search_ip = gtk_button_new_with_label("Rechercher IP");
    g_signal_connect_swapped(button_search_ip, "clicked", G_CALLBACK(gtk_window_close), window);
    gtk_box_pack_start(GTK_BOX(box), button_search_ip, FALSE, FALSE, 0);
    //choix
    GtkWidget *button_delete_ip = gtk_button_new_with_label("Effacer IP");
    g_signal_connect_swapped(button_delete_ip, "clicked", G_CALLBACK(gtk_window_close), window);
    gtk_box_pack_start(GTK_BOX(box), button_delete_ip, FALSE, FALSE, 0);



    // Ajouter les elements
    gtk_container_add(GTK_CONTAINER(window), box);


    // Montrer tout
    gtk_widget_show_all(window);
}

int main(int argc, char *argv[]) {

    GtkApplication *app = gtk_application_new("ESGI.app.exam.langagec", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    return g_application_run(G_APPLICATION(app), argc, argv);
}