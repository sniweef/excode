#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <gtk/gtk.h>

#define UI "ui"
#define BUFFER_SIZE 4096

static void build_ui(GtkBuilder **builder, GtkWidget **window,
        GtkLabel **header_label, GtkLabel **result_label) 
{
    *builder = gtk_builder_new();
    gtk_builder_add_from_file(*builder, UI, NULL);
    gtk_builder_connect_signals(*builder, NULL);

    *window = GTK_WIDGET(gtk_builder_get_object(*builder, "window"));
    *header_label = GTK_LABEL(gtk_builder_get_object(*builder, 
                "header-label"));
    *result_label = GTK_LABEL(gtk_builder_get_object(*builder, 
                "result-label"));
}
static void get_file_content(char *filename, char *buffer)
{
    int fd;
    int n;

    if ((fd = open(filename, O_RDONLY)) < 0) {
        g_print("read %s failed!\n", filename);
        exit(-1);
    }
    n = read(fd, buffer, BUFFER_SIZE);
    buffer[n] = '\0';
}

/* id: 0 - parent        *
 *     1 - first child   *
 *     2 - second child  */
static void go(int *argc, char **argv[], int id)
{
    GtkWidget *window;
    GtkBuilder *builder;
    GtkLabel *header_label, *result_label;
    char command[100];
    char buffer[BUFFER_SIZE];
    char *id_text[3][2] = {
        {"parent_in", "parent_out"},
        {"child1_in", "child1_out"},
        {"child2_in", "child2_out"}
    };

    gtk_init(argc, argv);
    build_ui(&builder, &window, &header_label, &result_label);
    g_snprintf(command, 100, "./mycp %s %s", id_text[id][0], id_text[id][1]);
    system(command);

    g_snprintf(command, 100, "Execute command: ./mycp %s %s\nFile %s contains:", id_text[id][0], id_text[id][1], id_text[id][1]);
    get_file_content(id_text[id][1], buffer);
    gtk_label_set_text(header_label, command);
    gtk_label_set_text(result_label, buffer);

    gtk_widget_show(window);
    gtk_main();
    
}
int main(int argc, char *argv[])
{
    pid_t pid[2];

    if ((pid[0] = fork()) < 0) {
        printf("fork error!\n");
        return -1;
    } else if (pid[0] == 0) {
        go(&argc, &argv, 1);
        exit(0);
    }

    if ((pid[1] = fork()) < 0) {
        printf("fork error!\n");
        return -1;
    } else if (pid[1] == 0) {
        go(&argc, &argv, 2);
        exit(0);
    }

    go(&argc, &argv, 0);
    wait(NULL);
    wait(NULL);
    return 0;
}
