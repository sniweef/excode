#include <gtk/gtk.h>
#include "system.h"
#include "process-tree.h"

static GtkWidget *label_num_processes;
static GtkWidget *label_cpu;
static GtkWidget *label_memory;

void
init_statusbar(GtkStatusbar *statusbar)
{
    GtkWidget *area, *box;
    
    area = gtk_statusbar_get_message_area(statusbar);
    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
    gtk_box_pack_start(GTK_BOX(area), box, TRUE, TRUE, 20);

    label_num_processes = gtk_label_new("Process:");
    gtk_box_pack_start(GTK_BOX(box), label_num_processes, FALSE, FALSE, 0);

    label_cpu = gtk_label_new("CPU:");
    gtk_box_pack_start(GTK_BOX(box), label_cpu, FALSE, FALSE, 0);

    label_memory = gtk_label_new("Memory:");
    gtk_box_pack_start(GTK_BOX(box), label_memory, FALSE, FALSE, 0);

    gtk_widget_show_all(box);
}

void 
//update_statusbar(GObject *object, guint property_id, const GValue *value)
update_statusbar()
{
    gchar text[15];
    guint num_process = get_process_num();
    gfloat cpu_usage = get_cpu_usage();
    gfloat memory_usage = get_memory_usage();


    g_snprintf(text, sizeof(text), "Processes: %u%%", num_process);
    gtk_label_set_text(GTK_LABEL(label_num_processes), text);

    g_snprintf(text, sizeof(text), "CPU: %.2f%%", cpu_usage);
    gtk_label_set_text(GTK_LABEL(label_cpu), text);

    g_snprintf(text, sizeof(text), "Memory: %.2f%%", memory_usage);
    gtk_label_set_text(GTK_LABEL(label_memory), text);

}
