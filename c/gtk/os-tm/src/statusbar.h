#ifndef STATUSBAR_H
#define STATUSBAR_H

void init_statusbar(GtkStatusbar *statusbar);
void update_statusbar(gfloat cpu_usage, gfloat mem_usage);

#endif
