#include <gtk/gtk.h>
#include <glib.h>
#include "process-tree.h"

gboolean
init_timeout()
//init_timeout(gpointer data)
{
    init_process_page();
    return TRUE;
}
int
main(int argc, char *argv[])
{
    GtkBuilder *builder;
    GObject *window;
    GObject *process_win;

    gtk_init(&argc, &argv);

    /* Construct a GtkBuilder instance and load our UI description */
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "ui", NULL);
    gtk_builder_connect_signals(builder, NULL);

    /* Connect signal handlers to the constructed widgets. */
    window = gtk_builder_get_object(builder, "window");

    process_win = gtk_builder_get_object(builder, "process-win");
    //init_process_page(GTK_WIDGET(process_win));
    GtkWidget *tree_view = init_process_page();
    gtk_container_add(GTK_CONTAINER(process_win), tree_view);
    //gtk_container_set_reallocate_redraws(GTK_CONTAINER(process_win),
            //TRUE);

    gtk_widget_show_all(GTK_WIDGET(window));
    g_timeout_add(750, (GSourceFunc)init_timeout, NULL);

    gtk_main();
    return 0;
}
