#include <stdio.h>
#include <stddef.h>
//#include "list.h"

typedef struct {
	void *next; // reserved
	// user specific elements.
}list_t;

int listAdd(list_t **list, void *item)
{
	list_t *cur = *list, *prev=NULL;
	if (cur == NULL) {
		*list = item;
		(*list)->next = NULL;
	} else {
		while (cur != NULL) {
			prev = cur;
			cur = cur->next;
		}
		// when here, cur is NULL
		prev->next = cur = item;
		cur->next = NULL;
	}
	return 0;
}

void listPrint(list_t *list)
{
	list_t *cur = list;
	printf("LIST: ");
	if (list == NULL) {
		printf("NULL\n");
		return;
	}
	while (cur != NULL) {
		printf("0x%p -> ", cur);
		cur = cur->next;
	}
	printf("NULL\n");
}
