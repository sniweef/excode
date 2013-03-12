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
    gint  prio;
} Process;

enum {
    SIGNAL_STOP = 0,
    SIGNAL_CONTINUE,
    SIGNAL_KILL,
};
enum {
    PRIORITY_VERY_LOW = 0,
    PRIORITY_LOW,
    PRIORITY_NORMAL,
    PRIORITY_HIGH,
    PRIORITY_VERY_HIGH,
};


gboolean get_process_list(GArray *process_list);
gboolean pid_is_sleeping (guint pid);
gboolean send_signal_to_pid(guint pid, gint tm_signal);
gboolean set_priority_to_pid(guint pid, gint priority);
gfloat   get_cpu_usage();
gfloat   get_memory_usage();

#endif
