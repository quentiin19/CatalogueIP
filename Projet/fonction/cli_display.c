#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#include "cli_display.h"
#include "function.h"


int display_design(int pattern){

    switch (pattern){
        case 1:
            printf(" _____ _____           /\\  /\\   \n");
            printf("|_   _|  __ \\         /   \\/  \\  \n");
            printf("  | | | |__) |       |        |  \n");
            printf("  | | |  ___/        | (o)(o) | \n");
            printf(" _| |_| |          c/| ,____, |\\\n");
            printf("|_____|_|            | `----' | \n");
            printf("                      `------'    \n");
            printf("    IP Finder - Find and Analyze IP Addresses\n");

            return 0;
        
        case 2:
            printf("  ____   __   __  _____     ____    _____   _____    __   __   ___    _   _     ____     ___     ___    _   _ \n");
            printf(" | __ )  \\ \\ / / | ____|   / ___|  | ____| | ____|   \\ \\ / /  / _ \\  | | | |   / ___|   / _ \\   / _ \\  | \\ | |\n");
            printf(" |  _ \\   \\ V /  |  _|     \\___ \\  |  _|   |  _|      \\ V /  | | | | | | | |   \\___ \\  | | | | | | | | |  \\| |\n");
            printf(" | |_) |   | |   | |___     ___) | | |___  | |___      | |   | |_| | | |_| |    ___) | | |_| | | |_| | | |\\  |\n");
            printf(" |____/    |_|   |_____|   |____/  |_____| |_____|     |_|    \\___/   \\___/    |____/   \\___/   \\___/  |_| \\_|\n");
            printf("                                                                                                               \n");

            return 0;
        
        default:
            return 1;
    }
}


int display_add_ip(sqlite3 *db){

    printf("\n\nAjout d'une nouvelle IP\n");
    printf("Entrez 'q' à n'importe quel moment pour revenir au menu !\n");

    int running = 1;

    char ip[17];
    char mask[17];

    while (running){

        printf("Entrez l'adresse IP : ");
        scanf("%s", ip);

        if (strcmp(ip, "q") == 0){
            running = 0;
            break;
        }


        printf("Entrez le masque : ");
        scanf("%s", mask);

        if (strcmp(ip, "q") == 0){
            running = 0;
            break;
        }


        if (verify_format_ipv4(ip)){
            printf("Format de l'adresse IP incorrect.\n");

        }else if (verify_format_mask(mask)){
            printf("Format du masque incorrect.\n");

        }else{
            if (add_ip_to_db(db, ip, mask)){
                printf("Erreur lors de l'ajout de l'adresse IP en base de données.\n");
                return 1;
            }else
            {
                printf("Adresse IP ajoutée avec succès.\n");
            }
        }
    }

    return 0;
}


int display_multiple_ip(void *unused, int count, char **data, char **column_names){

    if (count == 0) {
        printf("Aucune IP trouvée.\n");
    }else{
        for (int i = 0; i < count; i++) {
            printf("%s = %s\n", column_names[i], data[i] ? data[i] : "NULL");
        }
    }

    printf("\n");

    return 0;
}


int display_all_ip(sqlite3 *db){
    
    printf("\n\nAffichage de toutes les IP\n");

    if (sqlite3_exec(db, "SELECT Id, IPV4, Mask FROM Address", display_multiple_ip, 0, NULL) != SQLITE_OK){
        fprintf(stderr, "Error while displaying all IP addresses.\n");
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    return 0;
}



int display_search_ip(sqlite3 *db){

    printf("\n\nRecherche d'IP par réseau\n");
    printf("Entrez 'q' à n'importe quel moment pour revenir au menu !\n");

    int running = 1;

    char ip[16];
    char mask[16];

    while (running){

        printf("Entrez l'adresse IP du réseau : ");
        scanf("%s", ip);

        if (strcmp(ip, "q") == 0){
            running = 0;
            break;
        }


        printf("Entrez le masque du réseau : ");
        scanf("%s", mask);

        if (strcmp(ip, "q") == 0){
            running = 0;
            break;
        }


        if (verify_format_ipv4(ip)){
            printf("Format de l'adresse IP incorrect.\n");

        }else if (verify_format_mask(mask)){
            printf("Format du masque incorrect.\n");

        }else{
            // Calcul du réseau
            char network[33];
            if (get_network(ip, mask, network)){
                fprintf(stderr, "Error while getting network in the display_search_ip function.\n");
                return 1;
            }

            // Création de la requete SQL
            char query[1024];
            sprintf(query, "SELECT Id, IPV4, Mask FROM Address WHERE Network = '%s';", network);

            // Recherche et affichage des IP en base de données
            if (sqlite3_exec(db, query, display_multiple_ip, 0, NULL) != SQLITE_OK){
                fprintf(stderr, "Error while displaying all IP addresses.\n");
                fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
                return 1;
            }
        }
    }

    return 0;
}


int display_remove_ip(sqlite3 *db){

    printf("\n\nSuppression d'une IP de la base de données\n");
    printf("Entrez 'q' à n'importe quel moment pour revenir au menu !\n");

    int running = 1;

    char ip[16];
    char mask[16];

    while (running){

        printf("Entrez l'adresse IP du réseau : ");
        scanf("%s", ip);

        if (strcmp(ip, "q") == 0){
            running = 0;
            break;
        }


        printf("Entrez le masque du réseau : ");
        scanf("%s", mask);

        if (strcmp(ip, "q") == 0){
            running = 0;
            break;
        }


        if (verify_format_ipv4(ip)){
            printf("Format de l'adresse IP incorrect.\n");

        }else if (verify_format_mask(mask)){
            printf("Format du masque incorrect.\n");

        }else{
            // Création de la requete SQL
            char query[1024];
            sprintf(query, "DELETE FROM Address WHERE IPV4 = '%s' AND Mask = '%s';", ip, mask);

            // Suppression de l'IP en base de données
            if (sqlite3_exec(db, query, display_multiple_ip, 0, NULL) != SQLITE_OK){
                fprintf(stderr, "Error while displaying all IP addresses.\n");
                fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
                return 1;
            }
        }
    }

    return 0;
}


int display_menu(){

    // Diplay du desing de bienvenue
    display_design(1);

    // Initialisation de la base de données
    sqlite3 *db;

    if(open_bdd(&db)){
        fprintf(stderr, "Can't open database.\n");
        
        if (create_bdd(db)){
            fprintf(stderr, "Error while creating database.\n");
            return 1;
        }
    }

    // Boucle principale
    int running = 1;
    int choice;

    while (running){


        do{
            printf("[+] 1 - Add a new IP address\n");
            printf("[+] 2 - Display all IP addresses\n");
            printf("[+] 3 - Search similar by mask\n");
            printf("[+] 4 - Delete an IP\n");
            printf("[+] 5 - Exit\n");

            printf("Enter your choice: ");
            scanf("%d", &choice);
        }while (choice < 0 || choice > 5);


        switch (choice){
            case 1:
                // Ajout d'une ip en bdd
                if (display_add_ip(db)){
                    fprintf(stderr, "Error while adding IP address.\n");
                    return 1;
                }
                break;
            
            case 2:
                // Affichage de toutes les ip en bdd
                if (display_all_ip(db)){
                    fprintf(stderr, "Error while displaying all IP addresses.\n");
                    return 1;
                }
                break;
            
            case 3:
                // Recherche d'une ip en bdd
                if (display_search_ip(db)){
                    fprintf(stderr, "Error while searching IP addresses.\n");
                    return 1;
                }
                break;
            
            case 4:
                // Suppression d'une ip en bdd
                if (display_remove_ip(db)){
                    fprintf(stderr, "Error while removing IP addresses.\n");
                    return 1;
                }
                break;
            
            case 5:
                // Exit du programme
                running = 0;

                // Diplay du desing de fin
                display_design(2);

                // Fermeture de la base de données
                if (sqlite3_close(db) != SQLITE_OK){
                    fprintf(stderr, "Error while closing database.\n");
                    return 1;
                }
                break;
            
            default:
                printf("Le choix n'est pas valide, merci de recommencer !");
                break;
        }   
    }

    sqlite3_close(db);
    return 0;
}
