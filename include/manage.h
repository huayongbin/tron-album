#ifndef __MANAGE_H_
#define __MANAGE_H_

#include "common.h"

typedef struct node 
{
	struct node *prev;
	struct node *next;
	char *name;
}listnode, *linklist;

linklist init_node(void);
struct node *new_node(char *name);
void list_add_tail(struct node *head, struct node  *new);
linklist manage(linklist haed,int argc, char **argv);

#endif