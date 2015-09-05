/*
 * iesdb.c
 * Database management code.
 *
 * Copyright (C) 2015 José A. Navarro Ramón <josea.navarro@murciaeduca.es>
 */

#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iesdb.h"

/* FIXME: This is a mess. I am just beginning to code. Please, be patient. */
int iesdb_create_structure(sqlite3 *db)
{
	int retval;

	if ((retval = iesdb_process_sql_file(db, DBDIR"/"SQL_STRUCT_FILE)) != 0)
		return retval;

	return 0;
}

int iesdb_process_sql_file(sqlite3 *db, char *filename)
{
	FILE *sql_handle;
	char *sql_buff;
	int retval = 0, i;

	sql_handle = fopen(filename, "r");
	if (sql_handle == NULL) {
		fprintf(stderr, "Cannot open sql file: %s\n", filename);
		return 1;
	}

	/* Allocate space for line and sql buffers */
	sql_buff = (char *) malloc(IESDB_SQL_MAXLEN + 1);
	for (i = 0; i < IESDB_SQL_MAXLEN + 1; i++)
		sql_buff[i] = '\0';

	i = 0;

	while ((retval = iesdb_get_sql_string_from_handle(sql_buff,
				       	                  IESDB_SQL_MAXLEN,
							  sql_handle)) == 0) {
		//if (*sql_buff != '\0') {
			retval = sqlite3_exec(db, sql_buff, 0, 0, 0);
			if (retval) {
				fprintf(stderr, "\nFAILED:\n%s\n", sql_buff);
				return 4;
			}

			i++;
			printf("SQL: %d\n", i);
			printf("%s", sql_buff);
		//}
		printf("\n");
	}

	if (retval == 3)
		retval = 0;

	free(sql_buff);
	fclose(sql_handle);



	return 0;
}

int iesdb_get_sql_string_from_handle(char *buff, int max, FILE *handle)
{
	enum states {
		STATE_NORMAL,
		STATE_SPACE,
		STATE_COMMENT
	};

	enum phases {
		PHASE_INSIDE_LINE,
		PHASE_START_OF_LINE,
		PHASE_COMMENT
	};

	int state, phase, count, c, retval;
	
	count = 0;
	state = STATE_SPACE;
	phase = PHASE_START_OF_LINE;

	retval = 3;
	while ((c = fgetc(handle)) != EOF) {
		if (phase == PHASE_START_OF_LINE) {
			if (c == '\n') {
				retval = 0;
				break;
			} else if (c == '#') {
				state = STATE_COMMENT;
				phase = PHASE_COMMENT;
				continue;
			} else if (c != ' ' && c != '\t') {
				if (++count < max) {
					*(buff++) = c;
					state = STATE_NORMAL;
					phase = PHASE_INSIDE_LINE;
					continue;
				} else {
					fprintf(stderr, "Sql overflow\n");
					retval = 2;
					break;
				}
			}
		} else if (phase == PHASE_INSIDE_LINE) {
			if ((c == '\n' || c == ' ' || c == '\t')
		            && state == STATE_NORMAL) {
				//c = ' ';
				state = STATE_SPACE;
				if (c == '\n')
					phase = PHASE_START_OF_LINE;
			} else {
				state = STATE_NORMAL;
			}
			if (++count < max) {
				*(buff++) = c;
				continue;
			} else {
				fprintf(stderr, "Sql overflow\n");
				retval = 2;
				break;
			}
		} else if (phase == PHASE_COMMENT) {
			if (c == '\n') {
				state = STATE_SPACE;
				phase = PHASE_START_OF_LINE;
				continue;
			}
		}
	}
	
	/* Write an end of string */
	*buff = '\0';

	return retval;
}

int iesdb_create_table_system(sqlite3 *db)
{
	int result;

	sqlite3_stmt *stmt;

	result = sqlite3_prepare_v2(db,
		                    "CREATE TABLE system ("
		                     "version INTEGER,"
                                     "subversion INTEGER"
			             ");",
			             IESDB_SQL_MAXLEN, &stmt, NULL);
	if (result != SQLITE_OK) {
		fprintf(stderr, "'System' table prepare error: %d\n", result);
		return 1;
	}

	sqlite3_step(stmt);
	sqlite3_finalize(stmt);

	return 0;
}

int iesdb_populate_table_system(sqlite3 *db)
{
	int result;
	sqlite3_stmt *stmt;

	result = sqlite3_prepare_v2(db,
		                    "INSERT INTO system VALUES ("
		                     "0,1"
			             ");",
			             IESDB_SQL_MAXLEN, &stmt, NULL);
	if (result != SQLITE_OK) {
		fprintf(stderr, "'System' table query error: %d\n", result);
		return 1;
	}

	sqlite3_step(stmt);
	sqlite3_finalize(stmt);

	return 0;
}

int iesdb_query_table_system(sqlite3 **pdb)
{
	int result;
	sqlite3_stmt *stmt;
	int i;

	result = sqlite3_prepare_v2(*pdb,
		                    "SELECT * FROM system;",
			             IESDB_SQL_MAXLEN, &stmt, 0);
	if (result) {
		fprintf(stderr, "'System' table query error: %d\n", result);
		return 1;
	}

	int cols = sqlite3_column_count(stmt);
	printf("Number of cols: %d\n", cols);

	i = 0;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		i++;
		for(int col = 0; col < cols; col++){
			const char *val = (const char *) sqlite3_column_text(stmt,col);
			printf("%s = %s\t", sqlite3_column_name(stmt,col),val);
		}
	//	printf("\n");

	}
	if (i != 1) {
		fprintf(stderr, "'System' table has more than one register\n");
		exit(2);
	}

	printf("All %d rows fetched\n", i);
	
	sqlite3_finalize(stmt);

	return 0;
}

int iesdb_open_iesplus_file(char *filename, sqlite3 **pdb)
{
	int error = 0;

	error = sqlite3_open(filename, pdb);
	if (error) 
	{
		fprintf(stderr, "Cannot open sqlite3 file\n");
	} else {

		error = iesdb_query_table_system(pdb);
		if (error) {
			fprintf(stderr, "Error in query\n");
		}
	}

	return error;
}
