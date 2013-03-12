#include <gtk/gtk.h>
//#include <glib.h>
#include "system.h"

#define PEAK_NUM 10

static GArray *cpu_usage_array = NULL;
static GArray *mem_usage_array = NULL;
static GtkWidget *cpu_drawing_area = NULL;
static GtkWidget *mem_drawing_area = NULL;

static void
add_to_array(GArray *array, gfloat value)
{
    g_array_prepend_val(array, value);
    if (array->len > PEAK_NUM)
        g_array_remove_index(array, PEAK_NUM);
}
static void 
update_cpu_mem_usage(gfloat *cpu_usage, gfloat *mem_usage)
{
    if (cpu_usage_array == NULL) 
        cpu_usage_array  = g_array_new(FALSE, FALSE, sizeof(float));
    if (mem_usage_array == NULL) 
        mem_usage_array  = g_array_new(FALSE, FALSE, sizeof(float));
    *cpu_usage = get_cpu_usage();
    *mem_usage = get_memory_usage();
    //g_print("cpu,mem=%f,%f\n", *cpu_usage, *mem_usage);

    add_to_array(cpu_usage_array, *cpu_usage);
    add_to_array(mem_usage_array, *mem_usage);
}
gboolean
draw_cpu_usage(GtkWidget *cpu_da, cairo_t *cr, gpointer data)
{
    gint width, height;
    GdkRGBA color;
    gint step_size;
    gint i;
    gfloat *peak;

    cpu_drawing_area = cpu_da;
    width = gtk_widget_get_allocated_width(cpu_da);
    height = gtk_widget_get_allocated_height(cpu_da);
    step_size = width / (PEAK_NUM - 1);

    gtk_style_context_get_color(gtk_widget_get_style_context(cpu_da),
            0, &color);
    gdk_cairo_set_source_rgba(cr, &color);
    cairo_fill(cr);
    
    cairo_set_source_rgb(cr, 0.42, 0.65, 0.80);

    cairo_save(cr);
    cairo_move_to (cr, width, height);
    cairo_translate (cr, step_size, 0);
    for (i = 0; i < cpu_usage_array->len; i++) {
        peak = &g_array_index(cpu_usage_array, gfloat, i);
        //g_print("i=%d\npeak=%f\n", i, *peak/100);
        cairo_translate(cr, -step_size, 0);
        cairo_line_to(cr, width, (1.0-*peak/100)*height);
    }
    cairo_stroke(cr);

    cairo_restore(cr);
    cairo_set_line_width (cr, 1.0);
    for (i = 25; i <= 75; i += 25) {
        cairo_move_to (cr, 0.0, i * height / 100);
        cairo_line_to (cr, 4.0, i * height / 100);
        cairo_move_to (cr, width, i * height / 100);
        cairo_line_to (cr, width - 4.0, i * height / 100);
    }
    cairo_stroke(cr);
    return FALSE;
}

gboolean
draw_memory_usage(GtkWidget *mem_da, cairo_t *cr, gpointer data)
{
    gint width, height;
    GdkRGBA color;
    gint step_size;
    gint i;
    gfloat *peak;

    mem_drawing_area = mem_da;

    width = gtk_widget_get_allocated_width(mem_da);
    height = gtk_widget_get_allocated_height(mem_da);
    step_size = width / (PEAK_NUM - 1);

    gtk_style_context_get_color(gtk_widget_get_style_context(mem_da),
            0, &color);
    gdk_cairo_set_source_rgba(cr, &color);
    cairo_fill(cr);
    
    cairo_set_source_rgb(cr, 0.42, 0.65, 0.80);

    cairo_save(cr);
    cairo_move_to (cr, width, height);
    cairo_translate (cr, step_size, 0);
    for (i = 0; i < mem_usage_array->len; i++) {
        peak = &g_array_index(mem_usage_array, gfloat, i);
        cairo_translate(cr, -step_size, 0);
        cairo_line_to(cr, width, (1.0-*peak/100)*height);
    }
    cairo_stroke(cr);

    cairo_restore(cr);
    cairo_set_line_width (cr, 1.0);
    for (i = 25; i <= 75; i += 25) {
        cairo_move_to (cr, 0.0, i * height / 100);
        cairo_line_to (cr, 4.0, i * height / 100);
        cairo_move_to (cr, width, i * height / 100);
        cairo_line_to (cr, width - 4.0, i * height / 100);
    }
    cairo_stroke(cr);
    return FALSE;
}

void
update_monitor(gfloat *cpu_usage, gfloat *mem_usage)
{
    update_cpu_mem_usage(cpu_usage, mem_usage);
    if (GTK_IS_WIDGET(cpu_drawing_area))
        gtk_widget_queue_draw(cpu_drawing_area);
    if (GTK_IS_WIDGET(mem_drawing_area))
        gtk_widget_queue_draw(mem_drawing_area);
}
