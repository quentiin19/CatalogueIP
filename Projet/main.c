#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

void convert_ipv4_to_binary(const char* ipv4_address, char* binary_result);
int callback();
int verify_format_ipv4(const char* ipv4_address);
int verify_format_mask(const char* mask);


void convert_IPV4(const char* ipv4_address, char* binary_result, char* mask, char* binary_mask, char* hex_result) {
    int a, b, c, d;
   
    // Analyser l'adresse IPv4 et extraire les octets
    if (sscanf(ipv4_address, "%d.%d.%d.%d", &a, &b, &c, &d) != 4) {
        fprintf(stderr, "Invalid IPv4 address format.\n");
        return;
    }

    convert_ipv4_to_binary(ipv4_address, binary_result);
    convert_ipv4_to_binary(mask, binary_mask);
    sprintf(hex_result, "%02X.%02X.%02X.%02X", a, b, c, d);
}

void convert_ipv4_to_binary(const char* ipv4_address, char* binary_result) {
    int a, b, c, d;

    // Analyser l'adresse IPv4 et extraire les octets à nouveau
    sscanf(ipv4_address, "%d.%d.%d.%d", &a, &b, &c, &d);

    // Convertir chaque octet en binaire
    for (int i = 0; i < 8; i++) {
        binary_result[7 - i] = (a & (1 << i)) ? '1' : '0';
        binary_result[16 - i] = (b & (1 << i)) ? '1' : '0';
        binary_result[25 - i] = (c & (1 << i)) ? '1' : '0';
        binary_result[34 - i] = (d & (1 << i)) ? '1' : '0';
    }

    // Ajouter les points séparateurs
    binary_result[8] = binary_result[17] = binary_result[26] = '.';
    binary_result[35] = '\0'; // Caractère de fin de chaîne
}


int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    NotUsed = 0;

    if (argc == 0) {
        printf("Aucun résultat trouvé.\n");
        return 0;
    }

    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void search_by_mask(sqlite3 *db) {
    char ipv4_address[16];
    char mask[16];
    unsigned int a, b, c, d;
    unsigned int mask_a, mask_b, mask_c, mask_d;
    unsigned long int network, mask_int;
    char sql[1024];
    char *err_msg = 0;
    int rc;

    printf("\nSaisir une adresse IPv4: ");
    scanf("%15s", ipv4_address);
    printf("Saisir un masque: ");
    scanf("%15s", mask);

    sscanf(ipv4_address, "%u.%u.%u.%u", &a, &b, &c, &d);
    sscanf(mask, "%u.%u.%u.%u", &mask_a, &mask_b, &mask_c, &mask_d);

    network = (a & mask_a) << 24 | (b & mask_b) << 16 | (c & mask_c) << 8 | (d & mask_d);
    mask_int = mask_a << 24 | mask_b << 16 | mask_c << 8 | mask_d;

    sprintf(sql, "SELECT IPV4 FROM Address WHERE (Binary_IPV4 & %lu) = (Binary_IPV4 & %lu)", network, mask_int);


    // Ouvrir la base de données
    rc = sqlite3_open("bdd.sqlite", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Exécuter la requête
    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
    if (rc != SQLITE_OK && rc != SQLITE_EMPTY ) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("Recherche effectuée avec succès\n");
    }

    // Fermer la base de données
    sqlite3_close(db);
}

void create_bdd(sqlite3 *db) {
    char *err_msg = 0;
    int rc;

    char *sql = "CREATE TABLE Address(Id INTEGER PRIMARY KEY AUTOINCREMENT, IPV4 TEXT, Binary_IPV4 TEXT, Mask TEXT, Binary_mask TEXT, Hexadecimal TEXT);";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("Table created successfully\n");
    }
}


void insert_ip_data(sqlite3 *db, const char* ipv4_address, const char* mask, const char* binary_result, const char* binary_mask, const char* hex_result) {
    char *err_msg = 0;
    char sql[1024];
    int rc;

    sprintf(sql, "INSERT INTO Address (IPV4, Binary_IPV4, Mask, Binary_mask, Hexadecimal) VALUES ('%s', '%s', '%s', '%s', '%s');", ipv4_address, binary_result, mask, binary_mask, hex_result);
    
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("IP address data inserted successfully\n");
    }
}

int verify_format_ipv4(const char* ipv4_address) {
    int a, b, c, d;

    if (sscanf(ipv4_address, "%d.%d.%d.%d", &a, &b, &c, &d) != 4 || a < 0 || a > 255 || b < 0 || b > 255 || c < 0 || c > 255 || d < 0 || d > 255){
        fprintf(stderr, "Invalid IPv4 address format.\n");
        return 0;
    }

    return 1;
}

int verify_format_mask(const char* mask) {
    int a, b, c, d;

    if (sscanf(mask, "%d.%d.%d.%d", &a, &b, &c, &d) != 4 || a < 0 || a > 255 || b < 0 || b > 255 || c < 0 || c > 255 || d < 0 || d > 255){
        fprintf(stderr, "Invalid mask format.\n");
        return 0;
    }

    return 1;
}

int design() {
    printf(" _____ _____           /\\  /\\   \n");
    printf("|_   _|  __ \\         /   \\/  \\  \n");
    printf("  | | | |__) |       |        |  \n");
    printf("  | | |  ___/        | (o)(o) | \n");
    printf(" _| |_| |          c/| ,____, |\\\n");
    printf("|_____|_|            | `----' | \n");
    printf("                      `------'    \n");
    printf("    IP Finder - Find and Analyze IP Addresses\n");

    return 0;
}


int bye() {
    printf("  ____   __   __  _____     ____    _____   _____    __   __   ___    _   _     ____     ___     ___    _   _ \n");
    printf(" | __ )  \\ \\ / / | ____|   / ___|  | ____| | ____|   \\ \\ / /  / _ \\  | | | |   / ___|   / _ \\   / _ \\  | \\ | |\n");
    printf(" |  _ \\   \\ V /  |  _|     \\___ \\  |  _|   |  _|      \\ V /  | | | | | | | |   \\___ \\  | | | | | | | | |  \\| |\n");
    printf(" | |_) |   | |   | |___     ___) | | |___  | |___      | |   | |_| | | |_| |    ___) | | |_| | | |_| | | |\\  |\n");
    printf(" |____/    |_|   |_____|   |____/  |_____| |_____|     |_|    \\___/   \\___/    |____/   \\___/   \\___/  |_| \\_|\n");
    printf("                                                                                                               \n");

    return 0;
}


int display_menu(){
    int choice;

    printf("[+] 1 - Add a new IP address\n");
    printf("[+] 2 - Display all IP addresses\n");
    printf("[+] 3 - Search similar by mask\n");
    printf("[+] 4 - Delete an IP\n");
    printf("[+] 5 - Exit\n");

    printf("Enter your choice: ");
    scanf("%d", &choice);


    return choice;
}

int add_ip_address() {
    char ipv4_address[16]; // 15 caractères + 1 pour le caractère nul
    char mask[16];        // 15 caractères + 1 pour le caractère nul
    char binary_mask[36]; // 35 caractères + 1 pour le caractère nul
    char binary_result[36]; // 35 caractères + 1 pour le caractère nul
    char hex_result[12];    // 11 caractères + 1 pour le caractère nul
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
    convert_IPV4(ipv4_address, binary_result, mask, binary_mask, hex_result);

    // Insérer les données dans la base de données
    insert_ip_data(db, ipv4_address, mask, binary_result, binary_mask , hex_result);

    sqlite3_close(db);
    return 0;
}

int display_all(){
    sqlite3 *db;
    char *err_msg = 0;
    int rc;

    rc = sqlite3_open("bdd.sqlite", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    char *sql = "SELECT * FROM Address";

    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    sqlite3_close(db);
    return 0;
}

int delete_ip_address(){
    sqlite3 *db;
    char *err_msg = 0;
    int rc;

    rc = sqlite3_open("bdd.sqlite", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
     //lister toutes les Ips de ma table 
    char *sql = "SELECT Id, IPV4 FROM Address";

    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } 

    //saisir l'Id de l'ip à supprimer
    int id;
    printf("Entrez l'id de l'adresse Ip à supprimer :");
    scanf("%d", &id);

    //supprimer l'ip
    char sql2[1024];
    sprintf(sql2, "DELETE FROM Address WHERE Id = %d", id);

    rc = sqlite3_exec(db, sql2, callback, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }else{
        printf("IP address deleted successfully\n");
    }

    sqlite3_close(db);
    return 0;
}


int main() {
    char ipv4_address[16]; // 15 caractères + 1 pour le caractère nul
    char mask[16];        // 15 caractères + 1 pour le caractère nul
    char binary_mask[36]; // 35 caractères + 1 pour le caractère nul
    char binary_result[36]; // 35 caractères + 1 pour le caractère nul
    char hex_result[12];    // 11 caractères + 1 pour le caractère nul
    sqlite3 *db;
    int choice;

    create_bdd(db);

    design();
    choice=display_menu();

    switch (choice) {
        case 1:
            add_ip_address();
            break;
        case 2:
            display_all();
            break;
        case 3:
            search_by_mask(db);
            break;
        case 4:
            delete_ip_address();
            break;
        case 5:
            bye();
            // Exit
            quick_exit(0);
            break;
        default:
            printf("Invalid choice\n");
            break;
    }

    return 0;
}