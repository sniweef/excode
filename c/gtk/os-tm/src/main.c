#include <gtk/gtk.h>
#include <glib.h>
#include "process-tree.h"
#include "statusbar.h"

#define UI_PATH "ui"

gboolean
init_timeout()
{
    init_process_page();
    update_statusbar();
    return TRUE;
}
int
main(int argc, char *argv[])
{
    GtkBuilder *builder;
    GObject *window;
    GObject *process_win, *statusbar;

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
    gtk_widget_show_all(GTK_WIDGET(window));
    g_timeout_add(750, (GSourceFunc)init_timeout, NULL);

    gtk_main();
    return 0;
}
