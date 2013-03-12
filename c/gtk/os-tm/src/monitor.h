#ifndef MONITOR_H
#define MONITOR_H

//void init_drawing_area(GtkDrawingArea *cpu_da, GtkDrawingArea *mem_da);
#include <gtk/gtk.h>

gboolean draw_cpu_usage(GtkWidget *widget, cairo_t *cr, gpointer data);
gboolean draw_memory_usage(GtkWidget *widget, cairo_t *cr, gpointer data);
void update_monitor(gfloat *cpu_usage, gfloat *mem_usage);

#endif
