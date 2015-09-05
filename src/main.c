/*
 * main.c
 * Copyright (C) 2015 José A. Navarro Ramón <josea.navarro@murciaeduca.es>
 *
 */

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include <glib/gi18n.h>
#include <sqlite3.h>

#include "main.h"
#include "iesdb.h"
#include "iesstr.h"
#include "iesplus_i18.h"

static void activate(GtkApplication* app, gpointer user_data)
{
	GList *list;
	GtkWidget *window;

	GString* mainwindow_title;

	/* The application 'app' must be unique:
	   Test wether 'app' is running or not.
	   If running (visible, minimized or hidden), then bring it to top.
	   If not running, then execute it. */
	list = gtk_application_get_windows(app);

	if (list) {
		gtk_window_present(GTK_WINDOW(list->data));
	} else {
		window = gtk_application_window_new(app);
		mainwindow_title = g_string_new(PROGRAM_NAME);
		g_string_append(mainwindow_title, " - ");
		g_string_append(mainwindow_title, VERSION);
		gtk_window_set_title(GTK_WINDOW(window), mainwindow_title->str);
		/* Set the window icon */
		gtk_window_set_icon(GTK_WINDOW(window),
		       		main_create_pixbuf(IMAGEDIR"/"ICON_SMALL_FILE));
		gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
		gtk_widget_show_all(window);
	}
}

int main(int argc, char *argv[])
{
	GtkApplication *app;
	int status;
	char* application_id = "es.murciaeduca.navarro1.josea.iesplus";

	/*
	   Internationalization startup:
	   - GETTEXT_PACKAGE is the domain defined in "iesplus_i18.h"
	                     as the "iesplus" domain.
	   - LOCALEDIR is a constant defined in "Makefile".
	              It represents the base directory of the i18n files.
	   - Associate the message domain ("iesplus") to the base directory.
	   - i18n files will be in UTF-8.
	 */
	bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");

	/*	
	   Set the active domain for future gettext() calls
	   This means that i18n files will be found in the base directory
	   associated with this domain.
	   (Here, i18n files will be found when needeed in LOCALEDIR, which
	   is the directory associated above with the "iesplus" domain).
	 */
	textdomain (GETTEXT_PACKAGE);

	/* Application */
	app = gtk_application_new(application_id, G_APPLICATION_FLAGS_NONE);

	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);



//	gpointer datapack[] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};
//
//
//
//	/* Packing window and database pointers into 'datapack' */
//	datapack[MAIN_PACK_MAIN_WINDOW] = (gpointer) window;
//	
//	/* Create the application's main window. */
//	main_create_main_window(datapack);
//
//	gtk_widget_show_all(window);
//	
//	gtk_main ();
//
//	return 0;
	return status;
}

/* FUNCTION: main_create_main_window
 * Fill the application's main window.
 * Parameters:
 * datapack -> Packed data needed by some functions.
 *             Here, it is used to pack new_item, open_item and close_item
 *             together.
 */
void main_create_main_window(gpointer datapack[])
{
	GtkWidget *window;
	GtkWidget *vbox;
	GtkWidget *hmenubar_box;
	GtkWidget *menubar;

	window = (GtkWidget *) datapack[MAIN_PACK_MAIN_WINDOW];

	/* -----------------------------------------------------------
	   ------------------ ELEMENTS -------------------------------
	   -----------------------------------------------------------*/

	/* vbox */
	/* It will be just inside window container */
	/* Insert no pixels between elements */
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	/* vbox children will not have the same space in the box */
	gtk_box_set_homogeneous(GTK_BOX(vbox), FALSE);
	/* vbox will be placed inside the main window widget */
	gtk_container_add(GTK_CONTAINER(window), vbox);

	/* hmenubar_box */
	/* It will be a horizontal menubar */
	/* Insert no pixels between elements */
	hmenubar_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	/* hmenubar_box children will not have the same space in the box */
	gtk_box_set_homogeneous(GTK_BOX(hmenubar_box), FALSE);
	/* hmenubar_box will be placed inside vbox */
	/* Children will not have to receive extra space when parent grows */
	/* Children will not have to receive extra space when parent grows */
	/* No space put between the child and its neighbors */
	gtk_box_pack_start(GTK_BOX(vbox),hmenubar_box, FALSE, FALSE, 0);

	/*-- menubar --*/	
	menubar = gtk_menu_bar_new();
	/* Place "menubar" inside "hmenubar_box" */
	gtk_box_pack_start(GTK_BOX(hmenubar_box), menubar, TRUE, TRUE, 0);

	main_create_main_menu_and_signals(datapack, menubar);
}

/* FUNCTION: main_create_main_menu_and_signals
 * Define the main menu and put it into the menubar.
 * Parameters:
 * datapack -> Packed data needed by some functions.
 * menubar  -> A menubar widget previously created.
 *             Here we add the menu item structure to it.
 */
void main_create_main_menu_and_signals(gpointer datapack[], GtkWidget *menubar)
{
	GtkWidget *window;

	window = (GtkWidget *) datapack[MAIN_PACK_MAIN_WINDOW];

	// DECLARATION:
	//-----------------------------------------------------------
	/* Menu bar */
	//-----------------------------------------------------------
	/* iesplus_menu declaration */
	GtkWidget *iesplus_menu;
	GtkWidget *iesplus_item;

	/* database_menu declaration */
	GtkWidget *database_menu;
	GtkWidget *database_item;

	/* help_menu declaration */
	GtkWidget *help_menu;
	GtkWidget *help_item;
	//-----------------------------------------------------------

	//-----------------------------------------------------------
	/* Menu Items */
	//-----------------------------------------------------------
	/* item declarations */
	GtkWidget *quit_item;
	GtkWidget *new_item;
	GtkWidget *open_item;
	GtkWidget *close_item;
	GtkWidget *about_item;
	//-----------------------------------------------------------
	// CREATION
	//
	/* IesPlus, Database and Help menu.
	 * A menu is a container in which we can place menu items. */
	iesplus_menu = gtk_menu_new();
	database_menu = gtk_menu_new();
	help_menu = gtk_menu_new();

	/* Iesplus, Connection and Help items.
	 * Menu items are meant to be placed inside menus.
	 * These items will go inside menubar. */	
	iesplus_item = gtk_menu_item_new_with_mnemonic(_("_IesPlus"));
	database_item = gtk_menu_item_new_with_mnemonic(_("_Database"));
	help_item = gtk_menu_item_new_with_mnemonic(_("_Help"));

	// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	// ...MENU ITEMS
	//
	/* IesPlus menu items */
	/*   - quit item */	
	quit_item = gtk_menu_item_new_with_mnemonic(_("_Quit"));

	/* Database menu items */
	/*   - new, open and close items */	
	new_item = gtk_menu_item_new_with_mnemonic(_("_New"));
	open_item = gtk_menu_item_new_with_mnemonic(_("_Open"));
	close_item = gtk_menu_item_new_with_mnemonic(_("_Close"));

	/* Help menu items */
	/* about item */	
	about_item = gtk_menu_item_new_with_mnemonic(_("_About"));
	// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	// PLACING:
	/* -------- PLACING MENU ITEMS INSIDE MENUS -------- */
	/* menu items inside menubar */
	// 'iesplus' 'database' and 'help' items
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), iesplus_item);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), database_item);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), help_item);

	/* -------- Menu assignment -------- */
	/* menu items inside submenus */
	/* iesplus_item has associated a iesplus_menu */
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(iesplus_item), iesplus_menu);
	/* database_item has associated a database_menu */
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(database_item), database_menu);
	/* help_item has associated a helpmenu */
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(help_item), help_menu);

	/* menu items inside iesplus_menu */
	// quit
	gtk_menu_shell_append(GTK_MENU_SHELL(iesplus_menu), quit_item);
	/* menu items inside database_menu */
	// new
	gtk_menu_shell_append(GTK_MENU_SHELL(database_menu), new_item);
	// open
	gtk_menu_shell_append(GTK_MENU_SHELL(database_menu), open_item);
	// close
	gtk_menu_shell_append(GTK_MENU_SHELL(database_menu), close_item);
	/* menu items inside helpmenu */
	// about
	gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), about_item);
	/* -------- END PLACING MENU ITEMS INSIDE MENUS -------- */

	datapack[MAIN_PACK_NEW_ITEM] = (gpointer) new_item;
	datapack[MAIN_PACK_OPEN_ITEM] = (gpointer) open_item;
	datapack[MAIN_PACK_CLOSE_ITEM] = (gpointer) close_item;

	/* GREYING OUT MENU ITEMS */
	main_grayout_close_menu_item(datapack);

	/* -------- SIGNALS -------- */
	g_signal_connect(G_OBJECT(window), "delete_event",
			 G_CALLBACK(main_delete_event), (gpointer) window);

	g_signal_connect(G_OBJECT(window), "destroy",
			 G_CALLBACK(main_cb_destroy), datapack);


	g_signal_connect(G_OBJECT(quit_item), "activate",
			G_CALLBACK(main_cb_quit), (gpointer) window);

	g_signal_connect(G_OBJECT(new_item), "activate",
			G_CALLBACK(main_cb_new), datapack);

	g_signal_connect(G_OBJECT(open_item), "activate",
			G_CALLBACK(main_cb_open), datapack);

	g_signal_connect(G_OBJECT(close_item), "activate",
			G_CALLBACK(main_cb_close), datapack);

	g_signal_connect(G_OBJECT(about_item), "activate",
			G_CALLBACK(main_cb_aboutdialog), (gpointer) window);

}

/*
 * FUNCTION: main_create_pixbuf
 * Create a GdkPixbuf pointer to some image given its filename.
 * Parameters:
 * filename -> Some image file.
 */
GdkPixbuf *main_create_pixbuf(const gchar *filename)
{
	GdkPixbuf *pixbuf;
	GError *error = NULL;

	pixbuf = gdk_pixbuf_new_from_file(filename, &error);
	if(!pixbuf) {
		fprintf(stderr, "%s\n", error->message);
		g_error_free(error);
	}

	return pixbuf;
}

/* 
 * FUNCTION: main_program_exit_question
 * ****** NOT USED YET! *******
 * This function asks the user if he/she want to quit IesPlus because
 * there is some action required in order to exit cleanly.
 * Parameters:
 * datapack -> Packed data needed by some functions.
 *             Here, it is used to pack new_item, open_item and close_item
 *             together.
 *
 * FIXME: This function must tell the user why might not be a good idea to
 * exit (what action may be required before exiting).
 */
gint main_program_exit_question(gpointer data)
{
	GtkWidget *dialog = NULL;

	dialog = gtk_message_dialog_new(GTK_WINDOW(data),
		       		GTK_DIALOG_MODAL,
				GTK_MESSAGE_QUESTION,
				GTK_BUTTONS_OK_CANCEL,
				_("Are you sure you want to quit IesPlus?"));

	gtk_window_set_title(GTK_WINDOW(dialog), _("Question"));
	gint result = gtk_dialog_run(GTK_DIALOG(dialog));	
	gtk_widget_destroy(dialog);

	return(result);
}

/* 
 * FUNCTION: main_grayout_close_menu_item 
 * This function grays out the 'close' menu item and, at the same time,
 * leave the 'new' and 'open' items available to the user.
 * Parameters:
 * datapack -> Packed data needed by some functions.
 *             Here, it is used to change the availability state of these
 *             items: 'new_item', 'open_item' and 'close_item'.
 */
void main_grayout_close_menu_item(gpointer datapack[])
{
	GtkWidget *new_item;
	GtkWidget *open_item;
	GtkWidget *close_item;

	new_item = (GtkWidget *) datapack[MAIN_PACK_NEW_ITEM];
	open_item = (GtkWidget *) datapack[MAIN_PACK_OPEN_ITEM];
	close_item = (GtkWidget *) datapack[MAIN_PACK_CLOSE_ITEM];

	gtk_widget_set_sensitive(new_item, TRUE);
	gtk_widget_set_sensitive(open_item, TRUE);
	gtk_widget_set_sensitive(close_item, FALSE);
}

/* 
 * FUNCTION: main_grayout_new_open_menu_items 
 * This function grays out the 'new' and 'open' menu items and,
 * at the same time, leave the 'close' item available to the user.
 * Parameters:
 * datapack -> Packed data needed by some functions.
 *             Here, it is used to change the availability state of these
 *             items: 'new_item', 'open_item' and 'close_item'.
 */
void main_grayout_new_open_menu_items(gpointer datapack[])
{
	GtkWidget *new_item;
	GtkWidget *open_item;
	GtkWidget *close_item;

	new_item = (GtkWidget *) datapack[MAIN_PACK_NEW_ITEM];
	open_item = (GtkWidget *) datapack[MAIN_PACK_OPEN_ITEM];
	close_item = (GtkWidget *) datapack[MAIN_PACK_CLOSE_ITEM];

	gtk_widget_set_sensitive(new_item, FALSE);
	gtk_widget_set_sensitive(open_item, FALSE);
	gtk_widget_set_sensitive(close_item, TRUE);
}

/*** MANAGING SIGNALS ***/

/* 
 * FUNCTION: main_delete_event 
 * This callback function is called whenever the user, via the window
 * manager whishes to quit the program (i.e. The user click at the 'close'
 * option of the window manager.
 * Parameters:
 * widget   -> Pointer to the widget responsible for this call.
 * event    -> Event that occurred.
 * data     -> Some data sent to this function.
 */
gint main_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	/* If you return FALSE in the "delete_event" signal handler,
	 * GTK will emit the "destroy" signal.
	 * Returning TRUE means you don't want the window to be destroyed.
	 * This is useful for popping up 'Are you sure you want to quit?'
	 * type dialogs.
	 * We may ask the user if he/she really meant to quit:
	 * gint result = program_exit_question(data);
	 * and then act in consecuence:
         * if (result == GTK_RESPONSE_OK)
         *	return FALSE;
         * else
         *	return TRUE;
	 */

	return FALSE;
}

void main_cb_destroy(GtkWidget *widget, gpointer datapack[])
{
	sqlite3 *db;

	db = (sqlite3 *) datapack[MAIN_PACK_DB_HANDLE];

	fprintf(stderr, "%p\n", db);

	if (db) {
		g_print("Closing database\n");
		sqlite3_close(db);
	}

	gtk_main_quit();
}

/*** Callbacks ***/
void main_cb_new(GtkWidget *widget, gpointer datapack[])
{
	GtkWidget *window;
	GtkFileFilter *iesplus_file_filter;
	gchar *main_window_title;
	sqlite3 *db;
	int error;

	window = (GtkWidget *) datapack[MAIN_PACK_MAIN_WINDOW];
	main_window_title = (gchar *) datapack[MAIN_PACK_MAIN_WINDOW_TITLE];
	db = (sqlite3 *) datapack[MAIN_PACK_DB_HANDLE];

	/* Filter definition */
	iesplus_file_filter = gtk_file_filter_new();
	gtk_file_filter_set_name(iesplus_file_filter, "'iesplus' files");
	gtk_file_filter_add_pattern(iesplus_file_filter, "*.ies");

	GtkWidget *dialog;

	dialog = gtk_file_chooser_dialog_new("New Database",
		       	                 GTK_WINDOW(window),
		                         GTK_FILE_CHOOSER_ACTION_SAVE,
			       	         _("_Cancel"), GTK_RESPONSE_CANCEL,
			       	         _("_Open"), GTK_RESPONSE_ACCEPT,
			       	         NULL);

	//gtk_file_chooser_set_show_hidden(GTK_FILE_CHOOSER(dialog), TRUE);

	/* If the user chooses an already existing file, then the system 
	 * must send confirmation message */
	gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dialog),
		                    iesplus_file_filter);

	gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog),
		                                       TRUE);
	/* Here it is defined a callback just in case the user selects
	 * an already existing file */
	g_signal_connect(dialog, "confirm-overwrite",
		       	 G_CALLBACK(main_cb_overwrite_file),
			 NULL);

	/* The system suggests a default name to the user */
	gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog),
		                          IESDB_NEW_FILENAME);

	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
	{
		gchar *filename;

		filename = gtk_file_chooser_get_filename(
				                      GTK_FILE_CHOOSER(dialog));

		if (g_file_test(filename, G_FILE_TEST_EXISTS)) {
			g_print("BORRANDO FICHERO\n");
			if ((unlink(filename)) != 0) {
				fprintf(stderr, "No se puede borrar %s\n", filename);
			} else
				fprintf(stderr, "Borrado %s\n", filename);
		}


		/* Opening the new database file */
		error = sqlite3_open(filename, &db);
		if (error) {
			fprintf(stderr, "Cannot open database");
		}

		error = iesdb_create_structure(db);
		if (error) {
			fprintf(stderr, "Cannot open create tables");
		}

		error = iesstr_concat3files(&main_window_title,
			       	            PROGRAM_NAME,
					    " - ",
					    sqlite3_db_filename(db, "main"));
		if (error)
			fprintf(stderr, "FIXME: ERROR LONGITUD\n");
		gtk_window_set_title(GTK_WINDOW(window), main_window_title);

		datapack[MAIN_PACK_DB_HANDLE] = db;
		datapack[MAIN_PACK_MAIN_WINDOW_TITLE] = main_window_title;

		main_grayout_new_open_menu_items(datapack);

		g_free(filename);
	}

	gtk_widget_destroy(dialog);

}

/*
 * FUNCTION: main_cb_overwrite_file
 * Callback called by gtk_file_chooser in main_cb_new callback in case
 * the users selected an already existing file.
 * This function must delete it if it is not read-only file and accept
 * the selection.
 * If the file is a read-only file, then it must tell this fact to the
 * user and return to the gtk_file_chooser.
 */
GtkFileChooserConfirmation main_cb_overwrite_file(GtkFileChooser *chooser,
	                                          gpointer data)
{
	GtkWidget *message_dialog; 
	gchar *symlink_message = "File '%s' is a symlink. Try again!";
	gchar *directory_message = "File '%s' is a directory. Try again!";
	gchar *filename;

	/* We can find different cases:
	 * 1) It is a symlink.
	 * 2) We have no write permission on the file.
	 * 3) It is a ... whatever.
	 * 4) It is a regular file but the extension is not '.ies'
	 * ----> Return to the file chooser.
	 * 5) It is a regular file and the extension is '.ies'.
	 * ----> Accept it. return to the file chooser with a 
	 *       warning. If accepted, then we delete the file
	 *       and create a new, non-executable file.
	 */
	filename = gtk_file_chooser_get_filename(chooser);
	
	if (g_file_test(filename,
			G_FILE_TEST_IS_SYMLINK)) {
		message_dialog = gtk_message_dialog_new(GTK_WINDOW(chooser),
				GTK_DIALOG_DESTROY_WITH_PARENT,
				GTK_MESSAGE_ERROR,
				GTK_BUTTONS_CLOSE,
				symlink_message, filename,
				GTK_RESPONSE_ACCEPT, NULL 
				);
		g_signal_connect_swapped(message_dialog, "response",
				G_CALLBACK(gtk_widget_destroy),
				message_dialog);
		gtk_dialog_run(GTK_DIALOG(message_dialog));
		gtk_widget_destroy(message_dialog);
		return GTK_FILE_CHOOSER_CONFIRMATION_SELECT_AGAIN;
	} else if (g_file_test(filename,
			G_FILE_TEST_IS_DIR)) {
		message_dialog = gtk_message_dialog_new(GTK_WINDOW(chooser),
				GTK_DIALOG_DESTROY_WITH_PARENT,
				GTK_MESSAGE_ERROR,
				GTK_BUTTONS_CLOSE,
				directory_message, filename,
				GTK_RESPONSE_ACCEPT, NULL 
				);
		g_signal_connect_swapped(message_dialog, "response",
				G_CALLBACK(gtk_widget_destroy),
				message_dialog);
		gtk_dialog_run(GTK_DIALOG(message_dialog));
		gtk_widget_destroy(message_dialog);
		return GTK_FILE_CHOOSER_CONFIRMATION_SELECT_AGAIN;
	} else if (g_file_test(filename,
			G_FILE_TEST_IS_REGULAR)) {
		return GTK_FILE_CHOOSER_CONFIRMATION_CONFIRM;
	}

	g_free(filename);
	//if (is_uri_read_only(uri)) {
	//	if (user_wants_to_replace_read_only_file(uri))
	//		return GTK_FILE_CHOOSER_CONFIRMATION_ACCEPT_FILENAME;
	//	else
	//		return GTK_FILE_CHOOSER_CONFIRMATION_SELECT_AGAIN;
	//} else
		return GTK_FILE_CHOOSER_CONFIRMATION_CONFIRM;
}

void main_cb_open(GtkWidget *widget, gpointer datapack[])
{
	GtkWidget *window;
	gchar *main_window_title;
	sqlite3 *db;
	int error;

	window = (GtkWidget *) datapack[MAIN_PACK_MAIN_WINDOW];
	main_window_title = (gchar *) datapack[MAIN_PACK_MAIN_WINDOW_TITLE];
	db = (sqlite3 *) datapack[MAIN_PACK_DB_HANDLE];

	/* FIXME: We need to know if some kind of user action is recommended
	 * before opening a new file.
	 */

	GtkWidget *dialog;
	//GtkWidget *toggle;

	dialog = gtk_file_chooser_dialog_new("Open Database",
		                          GTK_WINDOW(window),
		                          GTK_FILE_CHOOSER_ACTION_OPEN,
		                          _("_Cancel"), GTK_RESPONSE_CANCEL,
		                          _("_Open"), GTK_RESPONSE_ACCEPT,
		                          NULL);

	//toggle = gtk_check_button_new_with_label("New database");
	//gtk_widget_show(toggle);
	//gtk_file_chooser_set_extra_widget(GTK_FILE_CHOOSER(dialog), toggle);

	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
	{
		gchar *filename;

		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		//error = sqlite3_open(filename, &db);
		//if (error) {
		//	fprintf(stderr, "Cannot open database\n");
		//}

		error = iesdb_open_iesplus_file(filename, &db);
		if (error) {
			fprintf(stderr, "Cannot open database\n");
		}else {
		
			error = iesstr_concat3files(&main_window_title,
			       	            PROGRAM_NAME,
					    " - ",
					    "s");
					    //sqlite3_db_filename(db, "main"));
			if (error)
				fprintf(stderr, "FIXME: ERROR LONGITUD\n");

			gtk_window_set_title(GTK_WINDOW(window), main_window_title);

			datapack[MAIN_PACK_DB_HANDLE] = db;
			datapack[MAIN_PACK_MAIN_WINDOW_TITLE] = main_window_title;

			main_grayout_new_open_menu_items(datapack);
		}

		g_free(filename);
	}

	gtk_widget_destroy(dialog);
}

void main_cb_close(GtkWidget *widget, gpointer datapack[])
{
	GtkWidget *window;
	gchar *main_window_title;
	sqlite3 *db;

	window = (GtkWidget *) datapack[MAIN_PACK_MAIN_WINDOW];
	main_window_title = (gchar *) datapack[MAIN_PACK_MAIN_WINDOW_TITLE];
	db = (sqlite3 *) datapack[MAIN_PACK_DB_HANDLE];

	/* FIXME: First we need to know if some kind of user action is needed
	 * before creating a new file. In this case we must do the action or
	 * produce a message.
	 */
	
	g_print("Closing database\n");
	sqlite3_close(db);

	g_free(main_window_title);
	datapack[MAIN_PACK_MAIN_WINDOW_TITLE] = (gpointer ) NULL;

	iesstr_concat3files(&main_window_title, PROGRAM_NAME, "", "");
	gtk_window_set_title(GTK_WINDOW(window), main_window_title);

	main_grayout_close_menu_item(datapack);
}

void main_cb_quit(GtkWidget *widget, gpointer data)
{
	gtk_widget_destroy(GTK_WIDGET(data));
}

void main_cb_aboutdialog(GtkWidget *widget, gpointer data)
{
	const gchar *authors[] = {
	            "José A. Navarro Ramón <josea.navarro@murciaeduca.es>",
	            NULL };
	
	const gchar *artists[] = {
	            "José A. Navarro Ramón <josea.navarro@murciaeduca.es>",
	            NULL };

	const gchar *copyright = 
	            "Copyright \xc2\xa9 2013, José A. Navarro Ramón";

	const gchar *comments = _("A class notebook for teachers.");

	const gchar *license = "Copyright \xc2\xa9 2013, "
			       "José A. Navarro Ramón.\n"
			       "All rights reserved.\n\n"
			       "Redistribution and use in source and binary "
			       "forms, with or without modification, are "
			       "permitted provided\nthat the following "
			       "conditions are met:\n\n"
			       "1) Redistributions of source code must retain "
			       "the above copyright notice, this list of "
		               "conditions and the\nfollowing disclaimer.\n\n"
			       "2) Redistributions in binary form must "
			       "reproduce the above copyright notice, this "
			       "list of conditions and\nthe following "
			       "disclaimer in the documentation and/or other "
			       "materials provided with the distribution.\n\n"
			       "THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT "
			       "HOLDERS AND CONTRIBUTORS""AS IS"" AND ANY\n"
			       "EXPRESS OR IMPLIED WARRANTIES, INCLUDING, "
			       "BUT NOT LIMITED TO, THE IMPLIED WARRANTIES\n"
			       "OF MERCHANTABILITY AND FITNESS FOR A "
			       "PARTICULAR PURPOSE ARE DISCLAIMED. IN NO "
			       "EVENT\nSHALL THE COPYRIGHT HOLDER OR "
			       "CONTRIBUTORS BE LIABLE FOR ANY DIRECT, "
			       "INDIRECT,\nINCIDENTAL, SPECIAL, EXEMPLARY, "
			       "OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT "
			       "LIMITED\nTO, PROCUREMENT "
			       "OF SUBSTITUTE GOODS OR SERVICES; LOSS OF "
			       "USE, DATA, OR PROFITS; OR\nBUSINESS "
			       "INTERRUPTION) HOWEVER CAUSED AND ON ANY "
			       "THEORY OF LIABILITY, WHETHER IN\nCONTRACT, "
			       "STRICT LIABILITY, OR TORT (INCLUDING "
			       "NEGLIGENCE OR OTHERWISE) ARISING IN ANY\n"
			       "WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF "
			       "ADVISED OF THE POSSIBILITY OF SUCH\nDAMAGE.";

	GdkPixbuf *logo = NULL;
	/* El directorio de imágenes debería poderse cambiar en main.h
	 * cuando se instalara en el ordenador */
	logo = gdk_pixbuf_new_from_file(IMAGEDIR"/"LOGO_FILE, NULL);

	gtk_show_about_dialog(GTK_WINDOW(data),
			      //"program-name", "IesPlus",
			      "program-name", PROGRAM_NAME,
			      //"version", "0.1",
			      "version", VERSION,
			      "title", _("About IesPlus"),
			      "license-type", GTK_LICENSE_CUSTOM,
			      "copyright", copyright,
			      "comments", comments,
			      "authors", authors,
			      "artists", artists,
			      "logo", logo,
			      "license", license,
			      NULL);
}

