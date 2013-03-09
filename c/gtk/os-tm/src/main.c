#include <gtk/gtk.h>
#include "process-tree.h"

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
    init_process_page(GTK_WIDGET(process_win));

    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_main ();
    return 0;
}
