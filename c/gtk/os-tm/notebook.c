#include <gtk/gtk.h>

enum {
    TITLE_COLUMN,
    AUTHOR_COLUMN,
    CHECKED_COLUMN,
    N_COLUMNS
};

    void 
init_process_page(GtkBuilder *builder)
{
    GtkTreeStore *store;
    GtkWidget *tree;
    GtkTreeViewColumn *column;
    GtkCellRenderer *renderer;

    /* Create a model.  We are using the store model for now, though we
     *     * could use any other GtkTreeModel */
    store = gtk_tree_store_new (N_COLUMNS,
            G_TYPE_STRING,
            G_TYPE_STRING,
            G_TYPE_BOOLEAN);

    GtkTreeIter iter1;  /* Parent iter */
    GtkTreeIter iter2;  /* Child iter  */

    gtk_tree_store_append (store, &iter1, NULL);  /* Acquire a top-level iterator */
    gtk_tree_store_set (store, &iter1,
            TITLE_COLUMN, "The Art of Computer Programming",
            AUTHOR_COLUMN, "Donald E. Knuth",
            CHECKED_COLUMN, FALSE,
            -1);

    gtk_tree_store_append (store, &iter2, &iter1);  /* Acquire a child iterator */
    gtk_tree_store_set (store, &iter2,
            TITLE_COLUMN, "Volume 1: Fundamental Algorithms",
            -1);

    gtk_tree_store_append (store, &iter2, &iter1);
    gtk_tree_store_set (store, &iter2,
            TITLE_COLUMN, "Volume 2: Seminumerical Algorithms",
            -1);

    gtk_tree_store_append (store, &iter2, &iter1);
    gtk_tree_store_set (store, &iter2,
            TITLE_COLUMN, "Volume 3: Sorting and Searching",
            -1);

    /* Create a view */
    tree = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));

    /* The view now holds a reference.  We can get rid of our own
     *     * reference */
    g_object_unref (G_OBJECT (store));

    /* Create a cell render and arbitrarily make it red for demonstration
     *     * purposes */
    renderer = gtk_cell_renderer_text_new ();
    g_object_set (G_OBJECT (renderer),
            "foreground", "red",
            NULL);

    /* Create a column, associating the "text" attribute of the
     *     * cell_renderer to the first column of the model */
    column = gtk_tree_view_column_new_with_attributes ("Author", renderer,
            "text", AUTHOR_COLUMN,
            NULL);

    gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);

    /* Second column.. title of the book. */
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes ("Title",
            renderer,
            "text", TITLE_COLUMN,
            NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);

    /* Last column.. whether a book is checked out. */
    renderer = gtk_cell_renderer_toggle_new ();
    column = gtk_tree_view_column_new_with_attributes ("Checked out",
            renderer,
            "active", CHECKED_COLUMN,
            NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);

    GtkWidget *process_win = gtk_builder_get_object(builder, 
            "process-win");
    gtk_container_add(GTK_CONTAINER(process_win), tree);
}
int
main (int   argc, char *argv[])
{
    GtkBuilder *builder;
    GObject *window;

    gtk_init (&argc, &argv);

    /* Construct a GtkBuilder instance and load our UI description */
    builder = gtk_builder_new ();
    gtk_builder_add_from_file (builder, "ui.glade", NULL);

    /* Connect signal handlers to the constructed widgets. */
    window = gtk_builder_get_object (builder, "window");
    g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

    GtkWidget *area, *box;

    area = gtk_builder_get_object(builder, "statusbar");
    //gtk_statusbar_push(statusbar, 1, "statusbar");
    box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 15);
    gtk_box_pack_start (GTK_BOX (area), box, TRUE, TRUE, 6);
    GtkWidget *label1 = gtk_label_new("test");
    gtk_box_pack_start (GTK_BOX (box), label1, FALSE, FALSE, 0);
    /*GtkWidget *label2 = gtk_label_new("test2");
    gtk_box_pack_start (GTK_BOX (box), label2, FALSE, FALSE, 0);*/


    init_process_page(builder);

    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_main ();

    return 0;
}
