#include <gtk/gtk.h>
#include "create_window.h"

gint main(gint argc, gchar *argv[])
{
		gtk_init(&argc, &argv);
		CreateWindow();
		gtk_main();
		return 0;
}
