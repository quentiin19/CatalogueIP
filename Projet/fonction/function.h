#ifndef FUNCTION_H
#define FUNCTION_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>



int create_bdd(sqlite3 *db);
int open_bdd(sqlite3 **db);

int verify_format_ipv4(const char* ipv4_address);
int verify_format_mask(const char* mask);

int decimal_to_binary(int num, char* binary);
int decimal_to_hex(int num, char* hex);

int convert_address_to_binary(const char* address, char* binary_address);
int convert_address_to_hex(const char* address, char* hex_address);
int get_network(const char* ip_address, const char* mask, char* network);

int callback_ip_exist(void * ip_exist, int count, char **data, char **column_name);
int ip_exist(sqlite3 *db, const char* ipv4_address, const char* mask);
int add_ip_to_db(sqlite3 *db, const char* ipv4_address, const char* mask);
int delete_ip_from_db(sqlite3 *db, const char* ipv4_address, const char* mask);


#endif