#include <gtk/gtk.h>
#include <glib.h>
#include <string.h>
#include "process-tree.h"
#include "statusbar.h"
#include "monitor.h"

#define UI_PATH "main.ui"

static void show_sysinfo_dialog(GtkMenuItem *menuitem, gpointer data);
static void show_about_dialog(GtkMenuItem *menuitem, gpointer data);

static gboolean
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

    GObject *sysinfo_mi, *about_mi;
    sysinfo_mi = gtk_builder_get_object(builder, "sysinfo-mi");
    g_signal_connect(sysinfo_mi, "activate",
            G_CALLBACK(show_sysinfo_dialog), NULL);

    about_mi = gtk_builder_get_object(builder, "about-mi");
    g_signal_connect(about_mi, "activate",
            G_CALLBACK(show_about_dialog), NULL);

    gtk_widget_show_all(GTK_WIDGET(window));
    g_timeout_add(750, (GSourceFunc)init_timeout, NULL);

    gtk_main();
    return 0;
}
static gchar *
get_distribution(gchar *txt, size_t len)
{
    FILE *file;
    char buffer[1024];
    gchar *end, *cur;
    int i = 0;

    if ((file = fopen("/etc/issue", "r")) == NULL) {
        g_strlcpy(txt, "", len);
        return txt;
    }

    fgets(buffer, 1024, file);
    //g_print("%s\n", buffer);
    end = g_strstr_len(buffer, 1024, "\\r");
    cur = buffer;
    while (i < len - 1 && cur < end) {
        txt[i] = buffer[i];
        cur++;
        i++;
    }
    txt[i] = '\0';

    fclose(file);
    return txt;
}
static gchar *
get_cpu_info(gchar *txt, size_t len)
{
    FILE *file;
    gchar buffer[1024];
    gchar *result = NULL;
    gchar *needle = "model name\t:";

    if ((file = fopen("/proc/cpuinfo", "r")) == NULL) {
        g_strlcpy(txt, "", len);
        return txt;
    }

    while (fgets(buffer, 1024, file) != NULL) {
        if ((result = g_strstr_len(buffer, 1024, needle)) 
                != NULL) {
            g_strlcpy(txt, result+strlen(needle), len);
            //g_print("%s\n", txt);
            break;
        }
    }
    fclose(file);

    //remove extra blanks
    gint i, j;

    for (i = 0; i < strlen(txt) && txt[i] != '\0'; i++) {
        if (txt[i] == ' ') {
            for (j = i; txt[j] == ' '; j++)
                ;
            g_strlcpy(txt+i, txt+j-1, len-j);
        }
    }
    //g_print("%s\n", txt);

    return txt;
}
static gchar *
get_memory_info(gchar *txt, size_t len)
{
    FILE *file;
    gchar buffer[1024];

    if ((file = fopen("/proc/meminfo", "r")) == NULL) {
        g_strlcpy(txt, "", len);
        return txt;
    }

    while (fgets(buffer, 1024, file) != NULL) {
        if (sscanf(buffer, "MemTotal:\t%s kB", txt) > 0)
            break;
    }
    fclose(file);

    gfloat mem_total_in_mb = (float)g_ascii_strtoll(txt, NULL, 10) / 1024;
    gfloat mem_total_in_gb = mem_total_in_mb / 1024;
    g_snprintf(txt, len, "%.2f MB(%.2f GB)", mem_total_in_mb,
            mem_total_in_gb);
    return txt;
}
static gchar *
get_os_bit_info(gchar *txt, size_t len)
{
    snprintf(txt, len, "%lu bit", sizeof(long) * 8);
    return txt; 
}
static gchar *
get_kernel_info(gchar *txt, size_t len)
{
    FILE *file;
    gchar buffer[1024];

    if ((file = fopen("/proc/version", "r")) == NULL) {
        g_strlcpy(txt, "", len);
        return txt;
    }

    fgets(buffer, 1024, file);
    sscanf(buffer, "Linux version %s", txt);
    return txt;
}
static void
show_sysinfo_dialog(GtkMenuItem *menuitem, gpointer data)
{
    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "sysinfo-dialog.ui", NULL);
    GObject *sysinfo_dialog = gtk_builder_get_object(builder, 
            "sysinfo-dialog");
    GObject *labels[5];
    gchar txt[50];

    labels[0] = gtk_builder_get_object(builder, "main-label");
    labels[1] = gtk_builder_get_object(builder, "cpu-label");
    labels[2] = gtk_builder_get_object(builder, "memory-label");
    labels[3] = gtk_builder_get_object(builder, "os-label");
    labels[4] = gtk_builder_get_object(builder, "kernel-label");

    gtk_label_set_text(GTK_LABEL(labels[0]), get_distribution(txt, 50));
    gtk_label_set_text(GTK_LABEL(labels[1]), get_cpu_info(txt, 50));
    gtk_label_set_text(GTK_LABEL(labels[2]), get_memory_info(txt, 50));
    gtk_label_set_text(GTK_LABEL(labels[3]), get_os_bit_info(txt, 50));
    gtk_label_set_text(GTK_LABEL(labels[4]), get_kernel_info(txt, 50));

    gtk_dialog_run(GTK_DIALOG(sysinfo_dialog));
    gtk_widget_destroy(GTK_WIDGET(sysinfo_dialog));
}

static void 
show_about_dialog(GtkMenuItem *menuitem, gpointer data)
{
    /*GtkAboutDialog *about_dialog = 
        GTK_ABOUT_DIALOG(gtk_about_dialog_new());

    gtk_dialog_run(GTK_DIALOG(about_dialog));
    gtk_widget_destroy(GTK_WIDGET(about_dialog));*/

    gtk_show_about_dialog(NULL, 
            "program-name", "os-tm", 
            //"authors", "hzhigeng", 
            "version", "1.0",
	    "comments", "Task mannager",
            "copyright", "Copyright © 2013–2014 hzhigeng<hzhigeng@gmail.com>",
            "license-type", GTK_LICENSE_GPL_2_0,
            NULL);
}
