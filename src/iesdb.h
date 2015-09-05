/*
 * iesdb.h
 * Definitions needed for iesdatabase.c.
 *
 * Copyright (C) 2015 José A. Navarro Ramón <josea.navarro@murciaeduca.es>
 */

#ifndef IESDB_H__

#define IESDB_H__

#define IESDB_NEW_FILENAME _("New Database")
#define IESDB_SQL_MAXLEN 2048
#define IESDB_LINE_MAXLEN 2048

int iesdb_create_structure(sqlite3 *db);
int iesdb_process_sql_file(sqlite3 *db, char *filename);
int iesdb_get_sql_string_from_handle(char *buff, int max, FILE *handle);
int iesdb_create_table_system(sqlite3 *db);
int iesdb_populate_table_system(sqlite3 *db);
int iesdb_query_table_system(sqlite3 **pdb);
int iesdb_open_iesplus_file(char *filename, sqlite3 **pdb);

#endif /* IESDB_H__ */

