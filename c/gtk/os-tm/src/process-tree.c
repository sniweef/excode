#include <gtk/gtk.h>
#include <glib-object.h>
#include "system.h"
#include "process-tree.h"

enum {
    COLUMN_COMMAND = 0,
    COLUMN_PID,
    COLUMN_PPID,
    COLUMN_STATE,
    COLUMN_VSZ,
    COLUMN_RSS,
    COLUMN_UID,
    COLUMN_CPU,
    COLUMN_PRIO,
    N_COLUMNS,
};

static GtkTreeStore *pro_tree_model = NULL;
static GArray *process_list = NULL;

static void 
update_model()
{
    GtkTreeIter iter;
    gint i;
    gint list_len; 
    Process *process;
    
    if (process_list != NULL)
        g_array_free(process_list, FALSE);
    process_list = g_array_new(FALSE, FALSE, sizeof(Process));

    get_process_list(process_list);
    if (process_list == NULL)
        return ;
    list_len = process_list->len;

    for (i = 0; i < list_len; i++) {
        process = &g_array_index(process_list, Process, i);
        gtk_tree_store_append(pro_tree_model, &iter, NULL);
        gtk_tree_store_set(pro_tree_model, &iter, 
                COLUMN_COMMAND, process->name, 
                COLUMN_PID, process->pid, 
                COLUMN_PPID, process->ppid,
                COLUMN_STATE, process->state,
                COLUMN_VSZ, process->vsz,
                COLUMN_RSS, process->rss,
                COLUMN_UID, process->uid,
                COLUMN_CPU, process->cpu_user+process->cpu_system,
                COLUMN_PRIO, process->prio,
                -1);
    }
}
static void
init_column(GtkWidget *pro_tree_view)
{
    GtkCellRenderer *cell_text, *cell_right_aligned;
    GtkTreeViewColumn *column;

    cell_text = gtk_cell_renderer_text_new();

    cell_right_aligned = gtk_cell_renderer_text_new();
    g_object_set(cell_right_aligned, "xalign", 1.0, NULL);

#define COLUMN_PROPERTIES "expand", TRUE, "clickable", TRUE,\
    "reorderable", TRUE, "resizable", TRUE, "visible", TRUE
    column = gtk_tree_view_column_new_with_attributes("Process", 
            cell_text, "text", COLUMN_COMMAND, NULL);
    g_object_set(column, COLUMN_PROPERTIES, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(pro_tree_view), column);

#undef COLUMN_PROPERTIES
#define COLUMN_PROPERTIES "expand", FALSE, "clickable", TRUE, \
    "reorderable", TRUE, "resizable", TRUE, "visible", TRUE
    column = gtk_tree_view_column_new_with_attributes("PID", 
            cell_right_aligned, "text", COLUMN_PID, NULL);
    g_object_set(column, COLUMN_PROPERTIES, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(pro_tree_view), column);


    column = gtk_tree_view_column_new_with_attributes("PPID", 
            cell_right_aligned, "text", COLUMN_PPID, NULL);
    g_object_set(column, COLUMN_PROPERTIES, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(pro_tree_view), column);

    column = gtk_tree_view_column_new_with_attributes("State", 
            cell_text, "text", COLUMN_STATE, NULL);
    g_object_set(column, COLUMN_PROPERTIES, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(pro_tree_view), column);

    column = gtk_tree_view_column_new_with_attributes("VSZ", 
            cell_right_aligned, "text", COLUMN_VSZ, NULL);
    g_object_set(column, COLUMN_PROPERTIES, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(pro_tree_view), column);

    column = gtk_tree_view_column_new_with_attributes("RSS", 
            cell_right_aligned, "text", COLUMN_RSS, NULL);
    g_object_set(column, COLUMN_PROPERTIES, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(pro_tree_view), column);

    column = gtk_tree_view_column_new_with_attributes("RSS", 
            cell_right_aligned, "text", COLUMN_RSS, NULL);
    g_object_set(column, COLUMN_PROPERTIES, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(pro_tree_view), column);

    column = gtk_tree_view_column_new_with_attributes("UID", 
            cell_right_aligned, "text", COLUMN_UID, NULL);
    g_object_set(column, COLUMN_PROPERTIES, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(pro_tree_view), column);

    column = gtk_tree_view_column_new_with_attributes("Prio", 
            cell_right_aligned, "text", COLUMN_PRIO, NULL);
    g_object_set(column, COLUMN_PROPERTIES, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(pro_tree_view), column);

}

void 
init_process_page(GtkWidget *process_win)
{
    GtkWidget *pro_tree_view;


    pro_tree_model = gtk_tree_store_new(N_COLUMNS, 
           G_TYPE_STRING, G_TYPE_UINT, G_TYPE_UINT,
           G_TYPE_STRING, G_TYPE_UINT, G_TYPE_UINT64,
           G_TYPE_UINT, G_TYPE_FLOAT, G_TYPE_UINT);
    update_model();
    
    pro_tree_view = gtk_tree_view_new_with_model(
            GTK_TREE_MODEL(pro_tree_model));

    init_column(pro_tree_view);

    gtk_container_add(GTK_CONTAINER(process_win), pro_tree_view);
}
