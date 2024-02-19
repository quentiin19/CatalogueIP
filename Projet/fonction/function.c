#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#include "function.h"



int verify_format_ipv4(const char* ipv4_address){
    int a, b, c, d;

    if (sscanf(ipv4_address, "%d.%d.%d.%d", &a, &b, &c, &d) != 4 || a < 0 || a > 255 || b < 0 || b > 255 || c < 0 || c > 255 || d < 0 || d > 255){
        fprintf(stderr, "Invalid IPv4 address format.\n");
        return 1;
    }

    return 0;
}

int verify_format_mask(const char* mask){
    int a, b, c, d;

    if (sscanf(mask, "%d.%d.%d.%d", &a, &b, &c, &d) != 4 || 
        ((a != 0 && a != 255) || a < b) || 
        ((b != 0 && b != 255) || b < c) || 
        ((c != 0 && c != 255) || c < d) || 
        ((d != 0 && d != 255))){
        fprintf(stderr, "Invalid mask format.\n");
        return 1;
    }

    return 0;
}


int decimal_to_binary(int num, char* binary){

    if(num > 255 || num < 0){
        fprintf(stderr, "Invalid number.\n");
        return 1;
    }
    
    for (int i = 7 ; i >= 0; i--){
        if(num > 0){
            binary[i] = (num % 2) + 48;
            num /= 2;
        }else{
            binary[i] = '0';
        }
    }


    return 0;
}

int convert_address_to_binary(const char* address, char* binary_address){
    int a, b, c, d;

    sscanf(address, "%d.%d.%d.%d", &a, &b, &c, &d);

    if(decimal_to_binary(a, binary_address) || decimal_to_binary(b, binary_address + 8) || decimal_to_binary(c, binary_address + 16) || decimal_to_binary(d, binary_address + 24)){
        fprintf(stderr, "Error while converting address to binary.\n");
        return 1;
    }

    binary_address[32] = '\0';

    return 0;
}


int decimal_to_hex(int num, char* hex){

    if(num > 255 || num < 0){
        fprintf(stderr, "Invalid number.\n");
        return 1;
    }

    int quotient, remainder;
 
    quotient = num;


    for (int i = 1 ; i >= 0; i--){
        if(quotient > 0){
            remainder = quotient % 16;
            if (remainder < 10){
                hex[i] = 48 + remainder;
            
            }else{
                hex[i] = 55 + remainder;
            }
            
            quotient = quotient / 16;

        }else{
            hex[i] = '0';
        }
    }

    return 0;
}

int convert_address_to_hex(const char* address, char* hex_address){
    int a, b, c, d;

    sscanf(address, "%d.%d.%d.%d", &a, &b, &c, &d);

    if(decimal_to_hex(a, hex_address) || decimal_to_hex(b, hex_address + 2) || decimal_to_hex(c, hex_address + 4) || decimal_to_hex(d, hex_address + 6)){
        return 1;
    }

    hex_address[8] = '\0';

    return 0;
}


int get_network(const char* ip_address, const char* mask, char* network){

    char * ip_binary = malloc(sizeof(char) * 33);
    char * mask_binary = malloc(sizeof(char) * 33);

    if(convert_address_to_binary(ip_address, ip_binary) || convert_address_to_binary(mask, mask_binary)){
        fprintf(stderr, "Error while converting address and mask to binary.\n");
        return 1;
    }

    for (int i = 0; i < 32; i++){
        network[i] = (ip_binary[i] == '1' && mask_binary[i] == '1') ? '1' : '0';
    }
    
    network[32] = '\0';

    free(ip_binary);
    free(mask_binary);

    return 0;
}


int create_bdd(sqlite3 *db) {

    char *sql = "CREATE TABLE Address(Id INTEGER PRIMARY KEY AUTOINCREMENT, IPV4 TEXT, Binary_IPV4 TEXT, Mask TEXT, Binary_mask TEXT, Hexadecimal TEXT, Network TEXT);";

    if (sqlite3_exec(db, sql, 0, 0, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));

        return 1;
        
    } else {
        printf("Table created successfully\n");
        return 0;
    }
}


int open_bdd(sqlite3 **db){
    if (sqlite3_open("bdd.sqlite", db) != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(*db));
        sqlite3_close(*db);
        return 1;
    }

    return 0;
}


int callback_ip_exist(void * ip_exist, int count, char **data, char **column_name){

    printf("test");

    *((int *)ip_exist) = atoi(data[0]);

    return 0;
}


int ip_exist(sqlite3 *db, const char* ipv4_address, const char* mask){
    char query[1024];

    sprintf(query, "SELECT COUNT(*) FROM Address WHERE IPV4 = '%s' AND Mask = '%s';", ipv4_address, mask);

    int * ip_exist;

    if (sqlite3_exec(db, query, callback_ip_exist, ip_exist, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    printf("aaaaa");
    printf("ip_exist : %d\n", ip_exist);

    // if(ip_exist){
    //     return 1;
    // }else{
    //     return 0;
    // }
    return 0;
}


int add_ip_to_db(sqlite3 *db, const char* ipv4_address, const char* mask){
    char query[1024];

    char * binary_ipv4_address = malloc(sizeof(char) * 33); // 32 bit + 1 pour le caractère d'arrêt
    if(convert_address_to_binary(ipv4_address, binary_ipv4_address)){
        fprintf(stderr, "Error while getting binary address in the add_ip_to_db function.\n");
        return 1;
    }

    char * binary_mask = malloc(sizeof(char) * 33); // 32 bit + 1 pour le caractère d'arrêt
    if(convert_address_to_binary(mask, binary_mask)){
        fprintf(stderr, "Error while getting binary mask in the add_ip_to_db function.\n");
        return 1;
    }

    char * hex_ipv4_address = malloc(sizeof(char) * 9); // 8 bit + 1 pour le caractère d'arrêt
    if(convert_address_to_hex(ipv4_address, hex_ipv4_address)){
        fprintf(stderr, "Error while getting hex address in the add_ip_to_db function.\n");
        return 1;
    }

    char * binary_network = malloc(sizeof(char) * 33); // 32 bit + 1 pour le caractère d'arrêt
    if(get_network(ipv4_address, mask, binary_network)){
        fprintf(stderr, "Error while getting network in the add_ip_to_db function.\n");
        return 1;
    }


    sprintf(query, "INSERT INTO Address (IPV4, Binary_IPV4, Mask, Binary_mask, Hexadecimal, Network) VALUES ('%s', '%s', '%s', '%s', '%s', '%s');", ipv4_address, binary_ipv4_address, mask, binary_mask, hex_ipv4_address, binary_network);


    if (sqlite3_exec(db, query, 0, 0, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return 1;

    } else {
        printf("IP address data inserted successfully\n");
        return 0;
    }
}


int delete_ip_from_db(sqlite3 *db, const char* ipv4_address, const char* mask){
    char query[1024];


    sprintf(query, "DELETE FROM Address WHERE IPV4 = '%s' AND Mask = '%s';", ipv4_address, mask);


    if (sqlite3_exec(db, query, 0, 0, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return 1;

    } else {
        printf("IP address data inserted successfully\n");
        return 0;
    }
}