################################################################################
#	Name       : Makefile
#	Author     : Didier Barvaux <didier@barvaux.org>
#	Description: create the ROHC statistics program
################################################################################

bin_PROGRAMS = \
	rohc_stats

bin_SCRIPTS = \
	rohc_stats.sh

man_MANS = \
	rohc_stats.1


rohc_stats_CFLAGS = \
	$(configure_cflags) \
	-Wno-unused-parameter \
	-Wno-sign-compare

rohc_stats_CPPFLAGS = \
	-I$(top_srcdir)/test \
	-I$(top_srcdir)/src/common \
	-I$(top_srcdir)/src/comp \
	-I$(top_srcdir)/src/decomp \
	$(libpcap_includes)

rohc_stats_LDFLAGS = \
	$(configure_ldflags)

rohc_stats_SOURCES = \
	rohc_stats.c

rohc_stats_LDADD = \
	-l$(pcap_lib_name) \
	$(top_builddir)/src/librohc.la \
	$(additional_platform_libs)


if BUILD_DOC_MAN
rohc_stats.1: $(rohc_stats_SOURCES) $(builddir)/rohc_stats
	$(AM_V_GEN)help2man --output=$@ -s 1 --no-info \
		-m "$(PACKAGE_NAME)'s tools" -S "$(PACKAGE_NAME)" \
		-n "The ROHC statistics tool" \
		$(builddir)/rohc_stats
endif


# extra files for releases
EXTRA_DIST = \
	$(bin_SCRIPTS) \
	$(man_MANS)

