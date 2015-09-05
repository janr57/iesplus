/*
 * iesstr.c
 * String related functions.
 *
 * Copyright (C) 2015 José A. Navarro Ramón <josea.navarro@murciaeduca.es>
 *
 */

#include <glib.h>
#include <string.h>

#include "iesstr.h"

#define IESSTR_MAX_TITLE_SIZE 250

int iesstr_concat3files(gchar **pdest, const gchar *str1, const gchar *str2,
	                const gchar *str3)
{
	int i, len;

	len = strlen(str1) + strlen(str2) + strlen(str3);

	if (len > IESSTR_MAX_TITLE_SIZE)
		return 1;

	*pdest = g_malloc(len + 1);
	for (i = 0; i < len + 1; i++)
		*(*pdest + i) = '\0';

	g_snprintf(*pdest, IESSTR_MAX_TITLE_SIZE, "%s%s%s", str1, str2, str3);

	return 0;
}

