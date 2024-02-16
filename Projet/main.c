// #include <gtk/gtk.h>

// #include "fonction/gtk_add_ip.h"
// #include "fonction/gtk_search_all.h"
// #include "fonction/gtk_search.h"
// #include "fonction/gtk_delete.h"
#include "fonction/function.h"
#include "fonction/cli_display.h"


// static void on_activate(GtkApplication *app) {
//     // Create a new window
//     // Create a new window
//     GtkWidget *window = gtk_application_window_new(app);
//     gtk_window_set_title(GTK_WINDOW(window), "IP catalogue - ESGI");
//     gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
//     gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);

//     // Create a new label with the desired text
//     GtkWidget *label = gtk_label_new("IP catalogue");
//     //configuration de la fenêtre

//     g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
//     gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

//     //configuration des box
//     GtkWidget * main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
//     gtk_container_add(GTK_CONTAINER(window), main_box);


//     GtkWidget *grid = gtk_grid_new();
//     gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
//     gtk_grid_set_column_spacing(GTK_GRID(grid), 10);


//     // Menu
//     //choix
//     GtkWidget *button_add_ip = gtk_button_new_with_label("Ajouter IP");
//     g_signal_connect_swapped(button_add_ip, "clicked", G_CALLBACK(show_add_ip_dialog), window);
//     gtk_box_pack_start(GTK_BOX(main_box), button_add_ip, FALSE, FALSE, 10);
//     //choix
//     GtkWidget *button_print_all = gtk_button_new_with_label("Afficher tout");
//     g_signal_connect_swapped(button_print_all, "clicked", G_CALLBACK(show_all_ip), window);
//     gtk_box_pack_start(GTK_BOX(main_box), button_print_all, FALSE, FALSE, 10);
//     //choix
//     GtkWidget *button_search_ip = gtk_button_new_with_label("Rechercher IP");
//     g_signal_connect_swapped(button_search_ip, "clicked", G_CALLBACK(show_ip), window);
//     gtk_box_pack_start(GTK_BOX(main_box), button_search_ip, FALSE, FALSE, 10);
//     //choix
//     GtkWidget *button_delete_ip = gtk_button_new_with_label("Effacer IP");
//     g_signal_connect_swapped(button_delete_ip, "clicked", G_CALLBACK(delete_ip), window);
//     gtk_box_pack_start(GTK_BOX(main_box), button_delete_ip, FALSE, FALSE, 10);

//     GtkWidget *button_quit = gtk_button_new_with_label("QUITTER");
//     g_signal_connect_swapped(button_quit, "clicked", G_CALLBACK(gtk_window_close), window);
//     gtk_box_pack_start(GTK_BOX(main_box), button_quit, FALSE, FALSE, 10);


//     // Ajouter les elements

//     gtk_container_add(GTK_CONTAINER(window), main_box);


//     // Montrer tout
//     gtk_widget_show_all(window);
// }

int main(int argc, char *argv[]) {

    display_menu();
    // GtkApplication *app = gtk_application_new("ESGI.app.exam.langagec", G_APPLICATION_DEFAULT_FLAGS);
    // g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    // return g_application_run(G_APPLICATION(app), argc, argv);

    // char * ip = "192.168.126.126";
    // char * mask = "255.255.255.0";
    // char * bad_ip = "192.268.0.1";
    // char * bad_mask = "255.254.255.0";

    // printf("Test de la fonction verify_format_ipv4\n");
    // printf("Test avec une adresse valide : %s / %d\n", ip, verify_format_ipv4(ip));
    // printf("Test avec une adresse valide : %s / %d\n", mask, verify_format_mask(mask));
    // printf("Test avec une adresse valide : %s / %d\n", bad_ip, verify_format_ipv4(bad_ip));
    // printf("Test avec une adresse valide : %s / %d\n", bad_mask, verify_format_mask(bad_mask));

    // char * binary = malloc(sizeof(char) * 33);

    // printf("Test de la fonction decimal_to_binary\n");
    
    // convert_address_to_binary(ip, binary);

    // for(int i = 0; i < 32; i++){
    //     if (i % 8 == 0 && i != 0)
    //     {
    //         printf(".");
    //     }
    //     printf("%c", binary[i]);
    // }


    // printf("\n");

    // char * network = malloc(sizeof(char) * 33);

    // printf("Test de la fonction get_network\n");
    // get_network(ip, mask, network);

    
    // for(int i = 0; i < 32; i++){
    //     if (i % 8 == 0 && i != 0)
    //     {
    //         printf(".");
    //     }
    //     printf("%c", network[i]);
    // }
    // printf("\n");
}
