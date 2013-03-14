#ifndef PROCESS_TREE_H
#define PROCESS_TREE_H
enum {
    PT_COLUMN_COMMAND,
    PT_COLUMN_PID,
    PT_COLUMN_PPID,
    PT_COLUMN_STATE,
    PT_COLUMN_VSZ,
    PT_COLUMN_VSZ_STR,
    PT_COLUMN_RSS,
    PT_COLUMN_RSS_STR,
    PT_COLUMN_UID,
    PT_COLUMN_UID_STR,
    PT_COLUMN_CPU,
    PT_COLUMN_CPU_STR,
    PT_COLUMN_PRIO,
    PT_COLUMN_BACKGROUND,
    PT_COLUMN_FOREGROUND,
    PT_COLUMN_TIMESTAMP,
    PT_N_COLUMNS,
};

GtkWidget *init_process_page();
void update_model();
guint get_process_num();

//void init_process_page(GtkWidget *process_win);

#endif
