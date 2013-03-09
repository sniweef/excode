#ifndef SYSTEM_H
#define SYSTEM_H

#include <glib-object.h>
#include <gtk/gtk.h>
#include <glib.h>

typedef struct _Process {
    guint   uid;
    gchar   uid_name[256];
    guint   pid;
    guint   ppid;
    gchar   name[256];
    gchar   cmdline[1024];
    gchar   state[16];
    gfloat  cpu_user;
    gfloat  cpu_system;
    guint   vsz;
    guint64 rss;
    gshort  prio;
} Process;

gboolean get_process_list(GArray *process_list);

#endif
