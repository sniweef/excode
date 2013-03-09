#include <stdio.h>
#include <glib.h>
//#include <glib/gstdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>
#include "system.h"

static gushort _cpu_count = 0;
static gulong jiffies_total_delta = 0;

static int 
get_pagesize (void)
{
    static int pagesize = 0;
    if (pagesize == 0)
    {
        pagesize = sysconf (_SC_PAGESIZE);
        if (pagesize == 0)
            pagesize = 4096;

    }
    return pagesize;

}
static void 
get_cpu_percent(guint pid, gulong jiffies_user, gfloat *cpu_user,
        gulong jiffies_system, gfloat *cpu_system)
{
    static GHashTable *hash_cpu_user = NULL;
    static GHashTable *hash_cpu_system = NULL;
    gulong jiffies_user_old, jiffies_system_old;

    if (hash_cpu_user == NULL) {
        hash_cpu_user = g_hash_table_new(NULL, NULL);
        hash_cpu_system = g_hash_table_new(NULL, NULL);
    }
    jiffies_user_old = GPOINTER_TO_UINT(
            g_hash_table_lookup(hash_cpu_user, GUINT_TO_POINTER(pid)));
    jiffies_system_old = GPOINTER_TO_UINT(
            g_hash_table_lookup(hash_cpu_system, GUINT_TO_POINTER(pid)));
    g_hash_table_insert(hash_cpu_user, GUINT_TO_POINTER (pid), 
            GUINT_TO_POINTER (jiffies_user));
    g_hash_table_insert(hash_cpu_system, GUINT_TO_POINTER (pid),
            GUINT_TO_POINTER (jiffies_system));

    if (jiffies_user < jiffies_user_old || jiffies_system < jiffies_system_old)
        return ;
    if (jiffies_total_delta > 0) {
        *cpu_user = (jiffies_user - jiffies_user_old) * 100 / 
            (gdouble)jiffies_total_delta;
        *cpu_system = (jiffies_system - jiffies_system_old) * 100 / 
            (gdouble)jiffies_total_delta;
    } else {
        *cpu_user = *cpu_system = 0;
    }

}
static gboolean
get_process_cmdline(Process *process)
{
    FILE *file;
    gchar filename[96]; 

    snprintf(filename, 96, "/proc/%i/cmdline", process->pid);
    if ((file = fopen(filename, "r")) == NULL)
        return FALSE;

    /* read full command byte per byte until EOF 
       note: arguments separated by '\0' */
    gint i;
    gchar c;

    for (i = 0; (c = fgetc(file)) != EOF && 
            i < (gint)sizeof(process->cmdline) - 1; i++)
        process->cmdline[i] = (c == '\0') ? ' ' : c; 
    process->cmdline[i] = '\0';
    if (process->cmdline[i-1] == ' ')
        process->cmdline[i-1] = '\0';
    fclose (file);

    /* kernel processes doesnot have a cmdline */
    if (i == 0) {
        size_t len = strlen(process->name);
        g_strlcpy(&process->cmdline[1], process->name, len+1);
        process->cmdline[0] = '[';
        process->cmdline[len+1] = ']';
        process->cmdline[len+2] = '\0';
    }
    return TRUE;
}

static gboolean
get_process_details(guint pid, Process *process)
{
    FILE *file;
    gchar buffer[1024];
    gchar filename[96];
    struct passwd *pw;
    struct stat sstat;

    snprintf(filename, 96, "/proc/%d/stat", pid);
    if ((file = fopen(filename, "r")) == NULL ||
            fgets(buffer, 1024, file) == NULL)
        return FALSE;
    fclose(file);

    /* get the short process name */
    {
        gchar *p_left, *p_cur, *p_right;
        guint i = 0;
        p_left = p_cur = g_strstr_len(buffer, -1, "(");
        p_right = g_strrstr_len(buffer, -1, ")");
        while (p_cur <= p_right) {
            if (p_cur > p_left && p_cur < p_right) {
                process->name[i++] = *p_cur;
                process->name[i] = '\0';
            }
            p_cur++;
        }
    }

    /* parse the stat file*/
    {
        gchar dummy[256];
        gint idummy;
        gulong jiffies_user = 0, jiffies_system = 0;

        sscanf(buffer, "%i %255s %1s %i %i %i %i %i %255s %255s %255s %255s %255s %lu %lu %i %i %i %d %i %i %i %llu %llu %255s %255s %255s %i %255s %255s %255s %255s %255s %255s %255s %255s %255s %255s %i %255s %255s",
                &process->pid, // processid
                dummy,     // processname
                process->state,    // processstate
                &process->ppid,   // parentid
                &idummy,   // processs groupid

                &idummy,   // session id
                &idummy,   // tty id
                &idummy,   // tpgid the process group ID of the process
                           //running on tty of the process
                dummy,     // flags
                dummy,     // minflt minor faults the process has maid

                dummy,     // cminflt
                dummy,     // majflt
                dummy,     // cmajflt
                &jiffies_user,  // utime the number of jiffies that this
                                // process has scheduled in user mode
                &jiffies_system,// stime " system mode

                &idummy,   // cutime " waited for children in user mode
                &idummy,   // cstime " system mode
                &idummy,   // priority (nice value + fifteen)
                (gint*)&process->prio, // nice range from 19 to -19
                &idummy,   // hardcoded 0

                &idummy,   // itrealvalue time in jiffies to next SIGALRM
                           // send to this process
                &idummy,   // starttime jiffies the process startet after
                           // system boot
                (unsigned long long*)&process->vsz, // vsize in bytes
                (unsigned long long*)&process->rss, // rss (number of pages in real memory)
                dummy,     // rlim limit in bytes for rss

                dummy,     // startcode
                dummy,     // endcode
                &idummy,   // startstack
                dummy,     // kstkesp value of esp (stack pointer)
                dummy,     // kstkeip value of EIP (instruction pointer)

                dummy,     // signal. bitmap of pending signals
                dummy,     // blocked: bitmap of blocked signals
                dummy,     // sigignore: bitmap of ignored signals
                dummy,     // sigcatch: bitmap of catched signals
                dummy,     // wchan

                dummy,     // nswap
                dummy,     // cnswap
                dummy,     // exit_signal
                &idummy,   // CPU number last executed on
                dummy,
                dummy
        );

        process->rss *= get_pagesize();
        get_cpu_percent(process->pid, jiffies_user, &process->cpu_user,
                jiffies_system, &process->cpu_system);
        stat(filename, &sstat);
        pw = getpwuid(sstat.st_uid);
        process->uid = sstat.st_uid;
        g_strlcpy(process->uid_name, (pw==NULL)?pw->pw_name:"nobody",
                sizeof(process->uid_name));
    }

    /* read the full command line */
    if (!get_process_cmdline(process))
        return FALSE;
    return TRUE;
}

gboolean
get_process_list(GArray *process_list)
{
    GDir *dir;
    const gchar *name;
    guint pid;
    Process process = {0};

    if ((dir = g_dir_open("/proc", 0, NULL)) == NULL)
        return FALSE;

    while ((name = g_dir_read_name(dir)) != NULL) {
        if ((pid = g_ascii_strtoull(name, NULL, 0)) > 0) {
            if (get_process_details(pid, &process))
                g_array_append_val(process_list, process);
        }
    }
    g_dir_close(dir);
    return TRUE;
}
