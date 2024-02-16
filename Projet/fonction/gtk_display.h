#ifndef GTK_DISPLAY_H
#define GTK_DISPLAY_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <gtk/gtk.h>


// main menu
void gtk_display_main_menu(GtkApplication *app);


// display all
int display(void *data, int argc, char **argv, char **azColName);
int gtk_display_all_ip();


// display search
int searchh(sqlite3 *db);
int show_ip(GtkWidget *widget, gpointer user_data);


// delete
void gtk_display_delete_ip(GtkWidget *widget, gpointer user_data);


// add
void gtk_display_add_ip(GtkWidget *widget, gpointer user_data);

#endif