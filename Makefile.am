pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libDataStruct.pc

SUBDIRS = src \
	include

libdatastructdocdir = ${prefix}/doc/libdatastruct
libdatastructdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS

EXTRA_DIST = $(libdatastructdoc_DATA)

# Copy all the spec files. Of cource, only one is actually used.
dist-hook:
	for specfile in *.spec; do \
		if test -f $$specfile; then \
			cp -p $$specfile $(distdir); \
		fi \
	done

