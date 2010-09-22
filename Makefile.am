## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src \
	data\
	include\
	examples

statevaldocdir = ${prefix}/doc/stateval
statevaldoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = $(statevaldoc_DATA)


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(statevaldocdir)
