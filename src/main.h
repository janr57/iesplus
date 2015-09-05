/*
 * main.h
 * Definitions needed for main.c.
 *
 * Copyright (C) 2015 José A. Navarro Ramón <josea.navarro@murciaeduca.es>
 */

#ifndef MAIN_H__

#define MAIN_H__

//#define MAIN_PROGRAM_VERSION PROGRAM_NAME " v" VERSION

/* datapack item order */
#define	MAIN_PACK_MAIN_WINDOW 0
#define	MAIN_PACK_MAIN_WINDOW_TITLE 1
#define	MAIN_PACK_NEW_ITEM 2
#define	MAIN_PACK_OPEN_ITEM 3
#define	MAIN_PACK_CLOSE_ITEM 4
#define	MAIN_PACK_DB_HANDLE 5

void main_create_main_window(gpointer datapack[]);
void main_create_main_menu_and_signals(gpointer datapack[], GtkWidget *menubar);
GdkPixbuf *main_create_pixbuf(const gchar *filename);
gint main_program_exit_question(gpointer data);

void main_grayout_close_menu_item(gpointer datapack[]);
void main_grayout_new_open_menu_items(gpointer datapack[]);

/* Managing signals */
gint main_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data);
void main_cb_destroy(GtkWidget *widget, gpointer datapack[]);

/* Callbacks */
void main_cb_quit(GtkWidget *widget, gpointer data);
void main_cb_new(GtkWidget *widget, gpointer datapack[]);
GtkFileChooserConfirmation main_cb_overwrite_file(GtkFileChooser *chooser, gpointer data);
void main_cb_open(GtkWidget *widget, gpointer datapack[]);
void main_cb_close(GtkWidget *widget, gpointer datapack[]);
void main_cb_aboutdialog(GtkWidget *widget, gpointer data);

#endif /* MAIN_H__ */


