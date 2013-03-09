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
static GtkWidget *pro_tree_view = NULL;
static GtkTreeViewColumn *sort_column = NULL;

static void column_clicked (GtkTreeViewColumn *column);

static void
memory_human_size(guint64 mem, gchar *mem_str)
{
    guint64 mem_tmp;

    mem_tmp = mem / 1024 / 1024;
    if (mem_tmp > 3) {
        g_snprintf (mem_str, 64, "%lu MiB", (gulong)mem_tmp);
        return;
    }

    mem_tmp = mem / 1024;
    if (mem_tmp > 8) {
        g_snprintf (mem_str, 64, "%lu KiB", (gulong)mem_tmp);
        return;
    }
    g_snprintf (mem_str, 64, "%lu B", (gulong)mem);
}
static void 
update_model()
{
    GtkTreeIter iter;
    gint i;
    gint list_len; 
    gchar vsz[64], rss[64], cpu[16];
    Process *process;
    
    if (process_list != NULL) {
        g_array_remove_range(process_list, 0, process_list->len);
    } else {
        process_list = g_array_new(FALSE, FALSE, sizeof(Process));
    }

    get_process_list(process_list);
    list_len = process_list->len;

    for (i = 0; i < list_len; i++) {
        process = &g_array_index(process_list, Process, i);
        memory_human_size(process->vsz, vsz);
        memory_human_size(process->rss, rss);
        g_snprintf(cpu, 16, "%.2f%%", process->cpu_user 
                + process->cpu_system);
        gtk_tree_store_append(pro_tree_model, &iter, NULL);
        gtk_tree_store_set(pro_tree_model, &iter, 
                PT_COLUMN_COMMAND, process->name, 
                PT_COLUMN_PID, process->pid, 
                PT_COLUMN_PPID, process->ppid,
                PT_COLUMN_STATE, process->state,
                PT_COLUMN_VSZ, process->vsz,
                PT_COLUMN_VSZ_STR, vsz,
                PT_COLUMN_RSS, process->rss,
                PT_COLUMN_RSS_STR, rss,
                PT_COLUMN_UID, process->uid,
                PT_COLUMN_UID_STR, process->uid_name,
                PT_COLUMN_CPU, process->cpu_user+process->cpu_system,
                PT_COLUMN_CPU_STR, cpu,
                PT_COLUMN_PRIO, process->prio,
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
            cell_text, "text", PT_COLUMN_COMMAND, NULL);
    g_object_set(column, COLUMN_PROPERTIES, NULL);
    g_object_set_data(G_OBJECT(column), "sort-column-id", 
            GINT_TO_POINTER(PT_COLUMN_COMMAND));
    g_signal_connect(column, "clicked", G_CALLBACK (column_clicked),
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(pro_tree_view), column);

#undef COLUMN_PROPERTIES
#define COLUMN_PROPERTIES "expand", FALSE, "clickable", TRUE, \
    "reorderable", TRUE, "resizable", TRUE, "visible", TRUE
    column = gtk_tree_view_column_new_with_attributes("PID", 
            cell_right_aligned, "text", PT_COLUMN_PID, NULL);
    g_object_set(column, COLUMN_PROPERTIES, NULL);
    g_object_set_data(G_OBJECT(column), "sort-column-id", 
            GINT_TO_POINTER(PT_COLUMN_PID));
    g_signal_connect(column, "clicked", G_CALLBACK(column_clicked),
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(pro_tree_view), column);


    column = gtk_tree_view_column_new_with_attributes("PPID", 
            cell_right_aligned, "text", PT_COLUMN_PPID, NULL);
    g_object_set(column, COLUMN_PROPERTIES, NULL);
    g_object_set_data(G_OBJECT(column), "sort-column-id", 
            GINT_TO_POINTER(PT_COLUMN_PPID));
    g_signal_connect(column, "clicked", G_CALLBACK (column_clicked),
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(pro_tree_view), column);

    column = gtk_tree_view_column_new_with_attributes("State", 
            cell_text, "text", COLUMN_STATE, NULL);
    g_object_set(column, COLUMN_PROPERTIES, NULL);
    g_object_set_data(G_OBJECT(column), "sort-column-id", 
            GINT_TO_POINTER(PT_COLUMN_STATE));
    g_signal_connect(column, "clicked", G_CALLBACK (column_clicked),
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(pro_tree_view), column);

    column = gtk_tree_view_column_new_with_attributes("VSZ", 
            cell_right_aligned, "text", PT_COLUMN_VSZ_STR, NULL);
    g_object_set(column, COLUMN_PROPERTIES, NULL);
    g_object_set_data(G_OBJECT(column), "sort-column-id", 
            GINT_TO_POINTER(PT_COLUMN_VSZ));
    g_signal_connect(column, "clicked", G_CALLBACK (column_clicked),
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(pro_tree_view), column);

    column = gtk_tree_view_column_new_with_attributes("RSS", 
            cell_right_aligned, "text", PT_COLUMN_RSS_STR, NULL);
    g_object_set(column, COLUMN_PROPERTIES, NULL);
    g_object_set_data(G_OBJECT(column), "sort-column-id", 
            GINT_TO_POINTER(PT_COLUMN_RSS));
    g_signal_connect(column, "clicked", G_CALLBACK (column_clicked),
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(pro_tree_view), column);

    column = gtk_tree_view_column_new_with_attributes("UID", 
            cell_text, "text", PT_COLUMN_UID_STR, NULL);
    g_object_set(column, COLUMN_PROPERTIES, NULL);
    g_object_set_data(G_OBJECT(column), "sort-column-id", 
            GINT_TO_POINTER(PT_COLUMN_UID_STR));
    g_signal_connect(column, "clicked", G_CALLBACK (column_clicked),
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(pro_tree_view), column);

    column = gtk_tree_view_column_new_with_attributes("CPU", 
            cell_right_aligned, "text", PT_COLUMN_CPU_STR, NULL);
    g_object_set(column, COLUMN_PROPERTIES, NULL);
    g_object_set_data(G_OBJECT(column), "sort-column-id", 
            GINT_TO_POINTER(PT_COLUMN_CPU));
    g_signal_connect(column, "clicked", G_CALLBACK (column_clicked),
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(pro_tree_view), column);

    column = gtk_tree_view_column_new_with_attributes("Prio", 
            cell_right_aligned, "text", PT_COLUMN_PRIO, NULL);
    g_object_set(column, COLUMN_PROPERTIES, NULL);
    g_object_set_data(G_OBJECT(column), "sort-column-id", 
            GINT_TO_POINTER(PT_COLUMN_PRIO));
    g_signal_connect(column, "clicked", G_CALLBACK (column_clicked),
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(pro_tree_view), column);

}

GtkWidget * 
init_process_page()
{
    
    if (pro_tree_model == NULL) {
        pro_tree_model = gtk_tree_store_new(PT_N_COLUMNS-3, 
                G_TYPE_STRING, G_TYPE_UINT, G_TYPE_UINT,
                G_TYPE_STRING, G_TYPE_UINT, G_TYPE_STRING,
                G_TYPE_UINT64, G_TYPE_STRING, G_TYPE_UINT,
                G_TYPE_STRING, G_TYPE_FLOAT, G_TYPE_STRING,
                G_TYPE_INT);
    } else {
        gtk_tree_store_clear(pro_tree_model);
    }
    update_model();
    pro_tree_view = gtk_tree_view_new_with_model(
            GTK_TREE_MODEL(pro_tree_model));

/*    if (pro_tree_view == NULL) {
        pro_tree_view = gtk_tree_view_new_with_model(
                GTK_TREE_MODEL(pro_tree_model));
    } else {
        gtk_tree_view_set_model(GTK_TREE_VIEW(pro_tree_view),
                GTK_TREE_MODEL(pro_tree_model));
    }*/
    init_column(pro_tree_view);
    
    /*GList *children = gtk_container_get_children(
            GTK_CONTAINER(process_win));
    if (children != NULL) {
        g_print("remove child\n");
        gtk_container_remove(GTK_CONTAINER(process_win), 
                GTK_WIDGET(children->data));
    }*/
    //gtk_container_add(GTK_CONTAINER(process_win), pro_tree_view);
    //gtk_widget_show(process_win);
    return pro_tree_view;
}

static void
column_clicked (GtkTreeViewColumn *column)
{
    gint sort_column_id;
    GtkSortType sort_type;

    gtk_tree_sortable_get_sort_column_id(GTK_TREE_SORTABLE(pro_tree_model),
            &sort_column_id, &sort_type);

    if (sort_column != column) {
        if (sort_column != NULL)
            gtk_tree_view_column_set_sort_indicator(sort_column,
                    FALSE);

        gtk_tree_view_column_set_sort_indicator(column, TRUE);

        sort_column_id = GPOINTER_TO_INT(g_object_get_data(
                    G_OBJECT (column), "sort-column-id"));
        sort_type = GTK_SORT_DESCENDING;
    } else {
        sort_type = (sort_type == GTK_SORT_ASCENDING) ? 
            GTK_SORT_DESCENDING : GTK_SORT_ASCENDING;
    }

    gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(
                pro_tree_model), sort_column_id, sort_type);
    gtk_tree_view_column_set_sort_order (column, sort_type);

    sort_column = column;
}
