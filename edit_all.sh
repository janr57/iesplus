# edit_all.sh
# Usage: $ sh edit_all.sh <editor>
#
# Copyright (C) 2015 José A. Navarro Ramón <josea.navarro@murciaeduca.es>
# 

files="Makefile\
	.gitignore\
	src/iesplus_i18.h\
	src/iesdb.h src/iesdb.c\
	src/main.h src/main.c"

if [ $# -ne 1 ]
then
	echo "USAGE: $0 <editor>"
	exit 1
elif [ $1 = 'gvim' ]
then
	$1 -geom 80x61 $files
elif [ $1 = 'vim' ]
then
	$1 $files
else
	$1 $files &
fi
