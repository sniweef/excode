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

static void column_clicked(GtkTreeViewColumn *column);
static gboolean treeview_clicked (GtkTreeView *treeview, 
        GdkEventButton *event);
//static void selected(GtkTreeView *tree_view, gboolean arg, gpointer data);

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
get_tree_iter(GtkTreeIter *iter, guint pid)
{
    gboolean valid;
    guint pid_iter;

    valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(pro_tree_model),
            iter);
    while (valid) {
        gtk_tree_model_get(GTK_TREE_MODEL(pro_tree_model), iter,
                PT_COLUMN_PID, &pid_iter, -1);
        if (pid == pid_iter)
            break;
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(pro_tree_model),
                iter);
    }

    if (!valid)
        gtk_tree_store_append(pro_tree_model, iter, NULL);
}
static void
rm_terminated_process(GArray *old_process_list)
{
    gint i, j;
    Process *old_process, *new_process;
    gboolean found;

    for (i = 0; i < old_process_list->len; i++) {
        found = FALSE;
        old_process = &g_array_index(old_process_list, Process, i);
        
        for (j = 0; j < process_list->len; j++) {
            new_process = &g_array_index(process_list, Process, j);
            if (old_process->pid == new_process->pid) {
                //g_print("found.pid=%u\n", old_process->pid);
                found = TRUE;
                break;
            }
        }

        if (!found) {
            GtkTreeIter iter;
            //g_print("remove process %d\n", old_process->pid);
            get_tree_iter(&iter, old_process->pid);
            gtk_tree_store_remove(pro_tree_model, &iter);
        }
    }
}
void 
update_model()
{
    GtkTreeIter iter;
    gint i;
    gint list_len; 
    gchar vsz[64], rss[64], cpu[16];
    Process *process;
    GArray *old_process_list = process_list;

    /*new_process_list = g_array_new(FALSE, FALSE, sizeof(Process));
    get_process_list(new_process_list);

    if (process_list != NULL) {
        rm_terminated_process(new_process_list);
        g_array_remove_range(process_list, 0, process_list->len);
    }
    process_list = new_process_list;*/
    
    process_list = g_array_new(FALSE, FALSE, sizeof(Process));
    get_process_list(process_list);

    if (old_process_list != NULL) {
        rm_terminated_process(old_process_list);
        g_array_free(old_process_list, TRUE);
    }

    list_len = process_list->len;

    for (i = 0; i < list_len; i++) {
        process = &g_array_index(process_list, Process, i);
        memory_human_size(process->vsz, vsz);
        memory_human_size(process->rss, rss);
        g_snprintf(cpu, 16, "%.2f%%", process->cpu_user 
                + process->cpu_system);
        get_tree_iter(&iter, process->pid);
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
    
    //if (pro_tree_model == NULL) {
        pro_tree_model = gtk_tree_store_new(PT_N_COLUMNS-3, 
                G_TYPE_STRING, G_TYPE_UINT, G_TYPE_UINT,
                G_TYPE_STRING, G_TYPE_UINT, G_TYPE_STRING,
                G_TYPE_UINT64, G_TYPE_STRING, G_TYPE_UINT,
                G_TYPE_STRING, G_TYPE_FLOAT, G_TYPE_STRING,
                G_TYPE_INT);
    //} else {
        //gtk_tree_store_clear(pro_tree_model);
    //}
    update_model();
    //if (pro_tree_view == NULL) {
        pro_tree_view = gtk_tree_view_new_with_model(
            GTK_TREE_MODEL(pro_tree_model));
        init_column(pro_tree_view);
    //}
    g_signal_connect(pro_tree_view, "button-press-event", 
            G_CALLBACK(treeview_clicked), NULL);

/*    if (pro_tree_view == NULL) {
        pro_tree_view = gtk_tree_view_new_with_model(
                GTK_TREE_MODEL(pro_tree_model));
    } else {
        gtk_tree_view_set_model(GTK_TREE_VIEW(pro_tree_view),
                GTK_TREE_MODEL(pro_tree_model));
    }*/
    
    /*GList *children = gtk_container_get_children(
            GTK_CONTAINER(process_win));
    if (children != NULL) {
        g_print("remove child\n");
        gtk_container_remove(GTK_CONTAINER(process_win), 
                GTK_WIDGET(children->data));
    }*/
    //gtk_container_add(GTK_CONTAINER(process_win), pro_tree_view);
    //gtk_widget_show(process_win);
    //g_signal_connect(pro_tree_view, "select-cursor-row", G_CALLBACK (selected), NULL);
    return pro_tree_view;
}

static void
column_clicked(GtkTreeViewColumn *column)
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

static void
send_signal(GtkMenuItem *mi, gpointer user_data)
{
        GtkWidget *dialog;
        guint pid = GPOINTER_TO_UINT(g_object_get_data(G_OBJECT(mi),
                    "pid"));
        gint tm_signal = GPOINTER_TO_INT(user_data);

        if (tm_signal == SIGNAL_KILL) {
            gint res;
            dialog = gtk_message_dialog_new(NULL, 0,
                    GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
                    "kill process");
            gtk_message_dialog_format_secondary_text(
                    GTK_MESSAGE_DIALOG(dialog),
                    "Are you sure you want to kill the PID %d?", pid);
            gtk_window_set_title(GTK_WINDOW (dialog), "os-tm");
            gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_MOUSE);
            res = gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy (dialog);
            if (res != GTK_RESPONSE_YES)
                return;
        }

        if (!send_signal_to_pid(pid, tm_signal)) {
            dialog = gtk_message_dialog_new(NULL, 0, GTK_MESSAGE_ERROR,
                    GTK_BUTTONS_CLOSE, "Error sending signal");
            gtk_message_dialog_format_secondary_text(
                    GTK_MESSAGE_DIALOG (dialog),
                    "An error was encountered by sending a signal to the PID %d.\nIt is likely you don't have the required privileges.", pid);
            gtk_window_set_title(GTK_WINDOW (dialog), "os-tm");
            gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_MOUSE);
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        }
}

static void
set_priority(GtkMenuItem *mi, gpointer user_data)
{
    guint pid = GPOINTER_TO_UINT(g_object_get_data(G_OBJECT(mi), "pid"));
    gint priority = GPOINTER_TO_INT(user_data);

    if (!set_priority_to_pid(pid, priority)) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, 0,
                GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
                "Error setting priority");
        gtk_message_dialog_format_secondary_text(
                GTK_MESSAGE_DIALOG(dialog),
                "An error was encountered by setting a priority to the PID %d.\nIt is likely you don't have the required privileges.", pid);
        gtk_window_set_title(GTK_WINDOW(dialog), "os-tm");
        gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_MOUSE);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}
static void
show_cmdline_dialog(GtkMenuItem *mi, gpointer user_data)
{
    guint pid = GPOINTER_TO_UINT(g_object_get_data(G_OBJECT(mi), "pid"));
    Process *process;
    gint i;

    for (i = 0; i < process_list->len; i++) {
        process = &g_array_index(process_list, Process, i);
        if (process->pid == pid) 
            break;
    }

    GtkWidget *dialog = gtk_message_dialog_new(NULL, 0,
            GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Cmdline");
    gtk_message_dialog_format_secondary_text(
                GTK_MESSAGE_DIALOG(dialog), process->cmdline);
            
    gtk_window_set_title(GTK_WINDOW (dialog), "os-tm");
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_MOUSE);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy (dialog);
}
static GtkWidget *
build_context_menu(guint pid)
{
    GtkWidget *menu, *menu_priority, *mi;
    menu = gtk_menu_new();

    if (!pid_is_sleeping (pid)) {
        mi = gtk_menu_item_new_with_label("Stop");
        g_object_set_data(G_OBJECT(mi), "pid", GUINT_TO_POINTER(pid));
        gtk_container_add(GTK_CONTAINER(menu), mi);
        g_signal_connect(mi, "activate", G_CALLBACK(send_signal), 
                GINT_TO_POINTER(SIGNAL_STOP));
    } else {
        mi = gtk_menu_item_new_with_label("Continue");
        g_object_set_data(G_OBJECT(mi), "pid", GUINT_TO_POINTER(pid));
        gtk_container_add (GTK_CONTAINER (menu), mi);
        g_signal_connect(mi, "activate", G_CALLBACK(send_signal),
                GINT_TO_POINTER (SIGNAL_CONTINUE));
    }

    mi = gtk_menu_item_new_with_label("Kill");
    g_object_set_data(G_OBJECT(mi), "pid", GUINT_TO_POINTER(pid));
    gtk_container_add(GTK_CONTAINER(menu), mi);
    g_signal_connect(mi, "activate", G_CALLBACK(send_signal),
            GINT_TO_POINTER(SIGNAL_KILL));

    menu_priority = gtk_menu_new ();

    mi = gtk_menu_item_new_with_label("Very low");
    g_object_set_data(G_OBJECT(mi), "pid", GUINT_TO_POINTER(pid));
    gtk_container_add(GTK_CONTAINER(menu_priority), mi);
    g_signal_connect(mi, "activate", G_CALLBACK(set_priority),
            GINT_TO_POINTER(PRIORITY_VERY_LOW));

    mi = gtk_menu_item_new_with_label("Low");
    g_object_set_data(G_OBJECT (mi), "pid", GUINT_TO_POINTER(pid));
    gtk_container_add(GTK_CONTAINER(menu_priority), mi);
    g_signal_connect(mi, "activate", G_CALLBACK(set_priority),
            GINT_TO_POINTER(PRIORITY_LOW));

    mi = gtk_menu_item_new_with_label("Normal");
    g_object_set_data(G_OBJECT(mi), "pid", GUINT_TO_POINTER(pid));
    gtk_container_add(GTK_CONTAINER(menu_priority), mi);
    g_signal_connect(mi, "activate", G_CALLBACK(set_priority),
            GINT_TO_POINTER(PRIORITY_NORMAL));

    mi = gtk_menu_item_new_with_label("High");
    g_object_set_data(G_OBJECT(mi), "pid", GUINT_TO_POINTER(pid));
    gtk_container_add(GTK_CONTAINER(menu_priority), mi);
    g_signal_connect(mi, "activate", G_CALLBACK(set_priority),
            GINT_TO_POINTER(PRIORITY_HIGH));

    mi = gtk_menu_item_new_with_label("Very high");
    g_object_set_data(G_OBJECT(mi), "pid", GUINT_TO_POINTER(pid));
    gtk_container_add(GTK_CONTAINER(menu_priority), mi);
    g_signal_connect(mi, "activate", G_CALLBACK(set_priority),
            GINT_TO_POINTER(PRIORITY_VERY_HIGH));

    mi = gtk_menu_item_new_with_label("Priority");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(mi), menu_priority);
    gtk_container_add(GTK_CONTAINER(menu), mi);

    mi = gtk_menu_item_new_with_label("Cmdline");
    g_object_set_data(G_OBJECT(mi), "pid", GUINT_TO_POINTER(pid));
    gtk_container_add(GTK_CONTAINER(menu), mi);
    g_signal_connect(mi, "activate", G_CALLBACK(show_cmdline_dialog),
            NULL); 


    gtk_widget_show_all(menu);
    return menu;
}

static void
position_menu(GtkMenu *menu, gint *x, gint *y, gboolean *push_in,
        gpointer data)
{
    gdk_window_get_origin(gtk_tree_view_get_bin_window(
                GTK_TREE_VIEW(pro_tree_view)), x, y);
    *x += 5;
    *y += 5;
    *push_in = TRUE;
}

static void 
popup_menu(guint pid, guint activate_time, gboolean at_pointer_pos)
{
    static GtkWidget *menu = NULL;
    GtkMenuPositionFunc position_func = NULL;

    if (at_pointer_pos == FALSE)
        position_func = (GtkMenuPositionFunc)position_menu;
    if (menu != NULL)
        gtk_widget_destroy(menu);
    menu = build_context_menu(pid);
    gtk_menu_popup(GTK_MENU(menu), NULL, NULL, position_func,
            pro_tree_view, 1, activate_time);
    
}
static gboolean treeview_clicked (GtkTreeView *treeview, 
        GdkEventButton *event)
{
    guint pid;

    if (event->button != 3)
        return FALSE;

    GtkTreeSelection *selection;
    GtkTreePath *path;
    GtkTreeIter iter;

    selection = gtk_tree_view_get_selection(treeview);
    gtk_tree_view_get_path_at_pos(treeview, (gint)event->x, 
            (gint)event->y, &path, NULL, NULL, NULL);

    if (path == NULL)
        return FALSE;
    gtk_tree_model_get_iter(GTK_TREE_MODEL(pro_tree_model), &iter, path);
    gtk_tree_model_get(GTK_TREE_MODEL(pro_tree_model), &iter,
            PT_COLUMN_PID, &pid, -1);
    gtk_tree_selection_select_path(selection, path);
    gtk_tree_path_free(path);
    
    popup_menu(pid, event->time, TRUE);
    return TRUE;
}
guint
get_process_num()
{
    return process_list->len;
}
