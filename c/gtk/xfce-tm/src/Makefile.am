NULL = 

INCLUDES =								\
	-I$(top_srcdir)							\
	-DG_LOG_DOMAIN=\"xfce4-taskmanager\"				\
	-DPACKAGE_LOCALE_DIR=\"$(localedir)\"

bin_PROGRAMS =								\
	xfce4-taskmanager

xfce4_taskmanager_CFLAGS =						\
	$(GTK_CFLAGS)							\
	$(CAIRO_CFLAGS)							\
	$(WNCK_CFLAGS)							\
	$(GKSU_CFLAGS)							\
	$(NULL)

xfce4_taskmanager_LDADD = 						\
	$(GTK_LIBS)							\
	$(CAIRO_LIBS)							\
	$(WNCK_LIBS)							\
	$(GKSU_LIBS)							\
	$(NULL)

xfce4_taskmanager_SOURCES =						\
	main.c								\
	process-window.c		process-window.h		\
	process-monitor.c		process-monitor.h		\
	process-tree-view.c		process-tree-view.h		\
	process-statusbar.c		process-statusbar.h		\
	task-manager.c			task-manager.h			\
	settings.c			settings.h			\
	$(NULL)

if HAVE_WNCK
xfce4_taskmanager_SOURCES += app-manager.c app-manager.h
endif

if OS_FREEBSD
xfce4_taskmanager_SOURCES += task-manager-freebsd.c
endif
if OS_BSD
xfce4_taskmanager_SOURCES += task-manager-bsd.c
endif
if OS_SOLARIS
xfce4_taskmanager_SOURCES += task-manager-solaris.c
endif
if OS_LINUX
xfce4_taskmanager_SOURCES += task-manager-linux.c
endif
if OS_SKEL
xfce4_taskmanager_SOURCES += task-manager-skel.c \
	process-window.ui
endif

if MAINTAINER_MODE
BUILT_SOURCES = process-window_ui.h
process-window_ui.h: process-window.ui
	$(AM_V_GEN) exo-csource --static --strip-comments --strip-content --name=process_window_ui $< >$@
endif

EXTRA_DIST = process-window.ui

