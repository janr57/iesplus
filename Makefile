# Makefile for the IesPlus project
# 
# Copyright (C) 2015 José A. Navarro Ramón <josea.navarro@murciaeduca.es>
#

# *****************************************************************************
# ************ HOW TO USE *****************************************************
# *****************************************************************************
# Steps: 
# -------------------
# MAKE
# -------------------
#  1) 'make'
#     When no prefix is specified, it refers to the '/usr/local' directory.
#     So 'make' is equivalent to 'make prefix=/usr/local'
#  2) 'make prefix=<path>'
# -------------------
# MAKE INSTALL
# -------------------
# 'make install' actually installs the program.

# *****************************************************************************
# ************ IESPLUS ********************************************************
# *****************************************************************************
# -----------------------------------------------------------------------------
# PROGRAM
# -------------------
PROGRAM = iesplus
PROGRAM_NAME = IesPlus
VERSION = 0.1

# -----------------------------------------------------------------------------
# FILES
# -------------------
# Source images
LOGO_SOURCE_FILE = $(PROGRAM)-logo-source.svg
ICON_SOURCE_FILE = $(PROGRAM)-icon-source.svg
# Final images
LOGO_FILE = $(PROGRAM)-logo.png
ICON_FILE = $(PROGRAM)-icon.svg
ICON_SMALL_FILE = $(PROGRAM)-icon_small.png
# Database files
SQL_STRUCT_FILE = $(PROGRAM)_db_structure.sql

# *****************************************************************************
# ************ DIRECTORY STRUCTURE SECTION ************************************
# *****************************************************************************
# -----------------------------------------------------------------------------
# SOURCE DIRECTORIES
# -------------------
# Source code directories are UPPERCASE
# and immediately ended by the DIR suffix.
#
# SRCDIR      -> Source code directory.
# PODIR       -> Internationalization directory,
#                (i.e. where 'i18n' files in source form will be found).
# MODIR       -> Internationalization directory,
#                (i.e. where  'i18n' files in binary form will be found).
# ESESDIR     -> Spanish_Spain binary internationalization directory.
# IMAGEDIR    -> Images directory.
# INKSCAPEDIR -> Inkscape source images directory.
# DOCSDIR     -> Documentation directory.
# DBDIR       -> Database directory which holds basic sql commands.
#
SRCDIR = src
PODIR = po
MODIR = mo
ESESDIR = $(MODIR)/es_ES/LC_MESSAGES
IMAGEDIR = images
INKSCAPEDIR = $(IMAGEDIR)/inkscape
DOCSDIR = docs
DBDIR = database

# -----------------------------------------------------------------------------
# INSTALLATION DIRECTORIES
# -------------------
# These directories are lowercase and they refer to runtime.
# They also have the 'dir¡' suffix
# 
# bindir         -> Binaries directory,
#                   (i.e. where the binaries will be installed).
# datadir        -> ...
#
# ----------------------
# ***datadir***
# ----------------------
# localedir      -> Internationalization directory,
#                   (where the 'mo' files will go).
# esesdir        -> Directory where the spanish_spain i18n stuff will be found.
#                   (usually inside 'localedir').
#
# dataprogramdir -> Reserved directory for the program.
# imagedir       -> Directory where images will be installed.
#

ifndef prefix
	prefix = /usr/local
endif

bindir = $(prefix)/bin
datadir = $(prefix)/share
localedir = $(prefix)/locale
esesdir = $(localedir)/es_ES/LC_MESSAGES
dataprogramdir = $(datadir)/$(PROGRAM)
imagedir = $(dataprogramdir)/images
databasedir = $(dataprogramdir)/database
# I don't know of any other directory to put the .desktop file
# so that gnome could find it:
desktopdir = /usr/share/applications

# *****************************************************************************
# ************ COMPILE SECTION ************************************************
# *****************************************************************************
# -----------------------------------------------------------------------------
# COMPILE FLAGS
# -------------------
# If not defined DEV, or DEV=n, or DEV=no, then it is a definitive installation
#    CFLAGS := -Wall -O2 -march-native
# else
#    CFLAGS := -g -Wall
# end_DEV
# otherwise it is a 'developer' build and CFLAGS make a debuggable program.

ifndef DEV
	CFLAGS := -Wall -O2 -march=native
else
	ifeq ($(DEV),n)
		CFLAGS := -Wall -O2 -march=native
	else ifeq ($(DEV),no)
		CFLAGS := -Wall -O2 -march=native
	else ifeq ($(DEV),y)
		CFLAGS := -g -Wall
	else ifeq ($(DEV),yes)
		CFLAGS := -g -Wall
	else
		CFLAGS := -g Wall
	endif
endif

#
# if_CLANG is not defined, then gcc is used:
#    CC = gcc
#    STDC = -std=gnu11
# else
#    CC = clang
#    STDC = -std=c11
# end_DEV

CC := gcc
STDC := -std=gnu11
CCSTR := "GCC"

ifndef CLANG
	CC := gcc
	STDC := -std=gnu11
	CCSTR := "GCC"
else
	ifeq ($(CLANG),n)
		CC := gcc
		STDC := -std=gnu11
		CCSTR := "GCC"
	else ifeq ($(CLANG),no)
		CC := gcc
		STDC := -std=gnu11
		CCSTR := "GCC"
	else ifeq ($(CLANG),y)
		CC := clang
		STDC := -std=c11
		CCSTR := "CLANG"
	else ifeq ($(CLANG),yes)
		CC := clang
		STDC := -std=c11
		CCSTR := "CLANG"
	else
		CC := clang
		STDC := -std=c11
		CCSTR := "CLANG"
	endif
endif

CFLAGS += `pkg-config --cflags gtk+-3.0`
LIBS = `pkg-config --libs gtk+-3.0`
LIBS += -lsqlite3

SOURCES = main.c

OBJECTS = $(SRCDIR)/main.o $(SRCDIR)/iesdb.o $(SRCDIR)/iesstr.o

# ----------------------------------------------------------------------------
#  COMPILING
#  ---------------------------------
all: $(PROGRAM) $(IMAGEDIR)/$(LOGO_FILE) \
	$(IMAGEDIR)/$(ICON_FILE) $(IMAGEDIR)/$(ICON_SMALL_FILE) \
	$(DBDIR)/$(SQL_STRUCT_FILE) $(PROGRAM).desktop

$(SRCDIR)/main.o: $(SRCDIR)/main.c $(SRCDIR)/main.h $(SRCDIR)/$(PROGRAM)_i18.h
	@echo $(CCSTR) $(notdir $<)
	@$(CC) $(CFLAGS) $(STDC) \
		-DIMAGEDIR=\"$(imagedir)\" \
		-DPROGRAM_NAME=\"$(PROGRAM_NAME)\" \
		-DVERSION=\"$(VERSION)\" \
		-DLOGO_FILE=\"$(LOGO_FILE)\" \
		-DICON_FILE=\"$(ICON_FILE)\" \
		-DICON_SMALL_FILE=\"$(ICON_SMALL_FILE)\" \
		-DLOCALEDIR=\"$(localedir)\" \
		-c $< -o $@

$(SRCDIR)/iesdb.o: $(SRCDIR)/iesdb.c $(SRCDIR)/iesdb.h \
		$(SRCDIR)/$(PROGRAM)_i18.h
	@echo $(CCSTR) $(notdir $<)
	@$(CC) $(CFLAGS) $(STDC) \
		-DDBDIR=\"$(databasedir)\" \
		-DSQL_STRUCT_FILE=\"$(SQL_STRUCT_FILE)\" \
		-c $< -o $@

$(SRCDIR)/iesstr.o: $(SRCDIR)/iesstr.c $(SRCDIR)/iesstr.h \
       		$(SRCDIR)/$(PROGRAM)_i18.h
	@echo $(CCSTR) $(notdir $<)
	@$(CC) $(CFLAGS) $(STDC) \
		-c $< -o $@

# We have two options here regarding image creation:
# 1) We may produce the image using 'inkscape':
#    inkscape -C -w 350 -h 97 -f $< -e $@
# 2) We may use 'imagemagick':
#    convert -depth 16 -background transparent -scale 500 $< $@
$(IMAGEDIR)/$(LOGO_FILE): $(INKSCAPEDIR)/$(LOGO_SOURCE_FILE)
	@echo IMAGE $(notdir $@)
	@convert -depth 16 -background transparent -scale 500 $< $@

$(IMAGEDIR)/$(ICON_FILE): $(INKSCAPEDIR)/$(ICON_SOURCE_FILE)
	@echo IMAGE $(notdir $@)
	@inkscape $< -l $@

$(IMAGEDIR)/$(ICON_SMALL_FILE): $(INKSCAPEDIR)/$(ICON_SOURCE_FILE)
	@echo IMAGE $(notdir $@)
	@convert -depth 16 -background transparent -scale 128 $< $@

$(PROGRAM).desktop: $(PROGRAM)_desktop.template
	@echo DESKTOP FILE $@
	@sed -e 's/(__PROGRAM_NAME)/$(PROGRAM_NAME)/g' < $< > $@
	@sed -i -e 's/(__PROGRAM)/$(PROGRAM)/g' $@
	@sed -i -e 's/(__VERSION)/$(VERSION)/g' $@
	@sed -i -e 's/(__ICON)/$(ICON_FILE)/g' $@
	@sed -i -e 's#(__IMAGEDIR)#$(imagedir)#g' $@


# ----------------------------------------------------------------------------
#  LINKING
# ------------------------
$(PROGRAM): $(OBJECTS)
	@echo LINK $(notdir $^)
	@gcc $(OPTIONS) -o $@ $^ $(LIBS)


# *****************************************************************************
# ************ INTERNATIONALIZATION SECTION ***********************************
# *****************************************************************************
# -----------------------------------------------------------------------------
# I18N
# ------------------------
TEXT_SOURCES = $(SRCDIR)/main.i $(SRCDIR)/main.h \
	       $(SRCDIR)/iesstr.i $(SRCDIR)/iesstr.h \
	       $(SRCDIR)/iesdb.i $(SRCDIR)/iesdb.h

$(SRCDIR)/%.i: $(SRCDIR)/%.c
	@echo CPP $<
	@$(CC) $(CFLAGS) -E -o $@ $<

# The '.pot' file has all translatable strings
# 'xgettext' gets these strings from the specified source
# This file is in utf-8 format
$(PODIR)/$(PROGRAM).pot: $(TEXT_SOURCES)
	@xgettext --package-name=$(PROGRAM) --package-version=0.1 \
		-d $(PROGRAM) --from-code=utf-8 -s -o $@ $<
	@sed -i 's/charset=CHARSET/charset=UTF-8/g' $@

# ----------------------------------------------------------------------------
#  SPANISH / ESPAÑOL
# ------------------------
.PHONY: esespofile esesmofile

# El fichero es_ES.po es el que contiene cadenas anteriores traducidas y las
# nuevas a falta de traducir.
# Este fichero debe ser editado y traducir las cadenas nuevas o modificar las
# que se quieran cambiar.
#  - Si no existe el fichero es_ES.po hay que crearlo con 'msginit'.
#  - Si ya existe, 'msgmerge' se encarga de mezclar el fichero '.pot' con
#    es_ES.po para respetar las traducciones ya realizadas anteriormente.
esespofile: $(PODIR)/es_ES.po

$(PODIR)/es_ES.po: $(PODIR)/$(PROGRAM).pot
	@if [ -f $(PODIR)/es_ES.po ]; then msgmerge -s -U $@ $<; \
		else msginit -l es_ES.utf8 -o $@ -i $<; fi

# El fichero es_ES.po se codifica en binario y se almacena en el directorio
# adecuado para que tenga lugar la internacionalización en tiempo de
# ejecución.
esesmofile: $(ESESDIR)/$(PROGRAM).mo

$(ESESDIR)/$(PROGRAM).mo: $(PODIR)/es_ES.po
	msgfmt -c -v -o $@ $<

# *****************************************************************************
# ************ INSTALL SECTION ************************************************
# *****************************************************************************
install: $(PROGRAM)
	test -d $(prefix) || mkdir $(prefix)
	test -d $(bindir) || mkdir $(bindir)
	install -m 0755 $(PROGRAM) $(bindir)
	test -d $(datadir) || mkdir $(datadir)
	test -d $(dataprogramdir) || mkdir -p $(dataprogramdir)
	test -d $(imagedir) || mkdir -p $(imagedir)
	install -m 0644 $(IMAGEDIR)/*.png $(imagedir)
	install -m 0644 $(IMAGEDIR)/*.svg $(imagedir)
	test -d $(databasedir) || mkdir -p $(databasedir)
	install -m 0644 $(DBDIR)/*.sql $(databasedir)
	test -d $(localedir) || mkdir -p $(localedir)
	test -d $(esesdir) || mkdir -p $(esesdir)
	install -m 0644 $(ESESDIR)/$(PROGRAM).mo $(esesdir)
	install -m 0644 $(PROGRAM).desktop $(desktopdir)
	echo 'last_installed = $(prefix)' > ./LAST_INSTALLED.mk

# *****************************************************************************
# ************ UNINSTALL SECTION **********************************************
# *****************************************************************************
-include LAST_INSTALLED.mk

ifdef $(last_installed)
	prefix= $(last_installed)
else
	prefix = /usr/local
endif

bindir = $(prefix)/bin
datadir = $(prefix)/share
localedir = $(datadir)/locale
dataprogramdir = $(datadir)/$(PROGRAM)
esesdir = $(localedir)/es_ES/LC_MESSAGES
imagedir = $(dataprogramdir)/images
desktopdir = /usr/share/applications

uninstall: $(bindir)/$(PROGRAM)
		test -f $(bindir)/$(PROGRAM) && rm $(bindir)/$(PROGRAM)
		test -f $(esesdir)/$(PROGRAM).mo && rm $(esesdir)/$(PROGRAM).mo
		test -f $(imagedir)/$(LOGO_FILE) && rm $(imagedir)/$(LOGO_FILE)
		test -f $(imagedir)/$(ICON_FILE) && rm $(imagedir)/$(ICON_FILE)
		test -f $(imagedir)/$(ICON_SMALL_FILE) && rm $(imagedir)/$(ICON_SMALL_FILE)
		test -f $(desktopdir)/$(PROGRAM).desktop && rm $(desktopdir)/$(PROGRAM).desktop
		test -d $(databasedir) && rm -rf $(databasedir)
		test -d $(dataprogramdir) && rm -rf $(dataprogramdir)
		test -f ./LAST_INSTALLED.mk && rm ./LAST_INSTALLED.mk

# *****************************************************************************
# ************ UTILITIES SECTION **********************************************
# *****************************************************************************
.PHONY: clean help
# -----------------------------------------------------------------------------
# CLEAN
# -------------------
clean:
	@echo DELETE $(PROGRAM) $(PROGRAM)-g $(PROGRAM)-c *.i *.o\
		*.png *.svg *.desktop *~ #*#
	@$(RM) $(SRCDIR)/*.o $(SRCDIR)/*.i $(SRCDIR)/*~ $(PROGRAM) $(PROGRAM)g\
		$(PROGRAM)-c *~ $(PODIR)/*~ $(PROGRAM).desktop\
		$(IMAGEDIR)/*.png $(IMAGEDIR)/*.svg $(SRCDIR)/#*# #*#

help:
	@echo "****************** NORMAL USE **********************"
	@echo "1) 'make'            -> Compile (see documentation for options)."
	@echo "2) 'make esespofile' -> Search source code for new "\
	      "translatable strings that"
	@echo "                        will go to the spanish language 'po' file."
	@echo "                        The user may then translate these"\
	      "strings using"
	@echo "                        'gtranslator' or 'poedit'."
	@echo "3) 'make esesmofile' -> Compile translated strings in 'po'"\
	      "file into 'mo' file."
	@echo "4) 'make install'    -> Install IesPlus as root."
	@echo "5) 'make uninstall'  -> In case you want to uninstall."
	@echo "****************** OTHER ***************************"
	@echo "'make clean'         -> Clean source directory."
	@echo "'make help'          -> This help screen."
