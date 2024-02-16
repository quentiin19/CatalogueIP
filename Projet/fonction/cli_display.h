#ifndef CLI_DISPLAY_H
#define CLI_DISPLAY_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>


int display_multiple_ip(void *unused, int count, char **data, char **column_names);

int display_add_ip(sqlite3 *db);
int display_all_ip(sqlite3 *db);
int display_search_ip(sqlite3 *db);
int display_remove_ip(sqlite3 *db);

int display_design(int pattern);
int display_menu();




#endif