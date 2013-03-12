#include <gtk/gtk.h>
#include <glib.h>
#include "process-tree.h"
#include "statusbar.h"
#include "monitor.h"

#define UI_PATH "ui"

gboolean
init_timeout()
{
    gfloat cpu_usage, mem_usage;
    update_model();

    update_monitor(&cpu_usage, &mem_usage);
    //g_print("%f\t%f\n", cpu_usage, mem_usage);
    
    //use the monitor value below
    update_statusbar(cpu_usage, mem_usage);
    return TRUE;
}
int
main(int argc, char *argv[])
{
    GtkBuilder *builder;
    GObject *window;
    GObject *process_win, *statusbar;
    GObject *cpu_da, *mem_da;

    gtk_init(&argc, &argv);

    /* Construct a GtkBuilder instance and load our UI description */
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, UI_PATH, NULL);
    gtk_builder_connect_signals(builder, NULL);

    /* Connect signal handlers to the constructed widgets. */
    window = gtk_builder_get_object(builder, "window");

    process_win = gtk_builder_get_object(builder, "process-win");
    //init_process_page(GTK_WIDGET(process_win));
    GtkWidget *tree_view = init_process_page();
    gtk_container_add(GTK_CONTAINER(process_win), tree_view);
    //gtk_container_set_reallocate_redraws(GTK_CONTAINER(process_win),
            //TRUE);

    statusbar = gtk_builder_get_object(builder, "statusbar");
    init_statusbar(GTK_STATUSBAR(statusbar));
    
    cpu_da = gtk_builder_get_object(builder, "cpu-drawing-area");
    mem_da = gtk_builder_get_object(builder, "memory-drawing-area");
    g_signal_connect(cpu_da, "draw", G_CALLBACK(draw_cpu_usage), NULL);
    g_signal_connect(mem_da, "draw", G_CALLBACK(draw_memory_usage), NULL);

    gtk_widget_show_all(GTK_WIDGET(window));
    g_timeout_add(750, (GSourceFunc)init_timeout, NULL);

    gtk_main();
    return 0;
}
