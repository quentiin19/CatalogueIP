#include <gtk/gtk.h>

// #include "fonction/gtk_add_ip.h"
// #include "fonction/gtk_search_all.h"
// #include "fonction/gtk_search.h"
// #include "fonction/gtk_delete.h"
#include "fonction/function.h"
#include "fonction/cli_display.h"
#include "fonction/gtk_display.h"



int main(int argc, char *argv[]) {
    
    int mode = 3;

    if (mode == 0){
        display_menu();
    }

    else if (mode == 1){


        // Création de l'app GTK
        GtkApplication *app = gtk_application_new("ESGI.app.exam.langagec", G_APPLICATION_DEFAULT_FLAGS);

        // Lancement de la fonction principale
        g_signal_connect(app, "activate", G_CALLBACK(gtk_display_menu), NULL);


        return g_application_run(G_APPLICATION(app), argc, argv);
    }

    else if (mode == 2){
        
        sqlite3 *db;
        open_bdd(&db);

        char ip[] = "10.0.0.1";
        char mask[] = "255.0.0.0";


        printf("%d\n", ip_exist(db, ip, mask));
    }














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
