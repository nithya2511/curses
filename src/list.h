#ifndef SRC_LIST_H_
#define SRC_LIST_H_


typedef struct {
	void *next;
}list_t;

int listAdd(list_t **list, void *item);
void listPrint(list_t *list);

#endif /* SRC_LIST_H_ */
