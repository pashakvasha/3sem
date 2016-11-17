#include <stdio.h>
#include <stdlib.h>
typedef struct Node {
	Data val;
	struct Node * next;
	struct Node * prev;
} Node;

typedef struct Queue {
	Node * head;
	Node * tail;
} Queue;

void queue_init(Queue * queue) {
	queue->head = malloc( sizeof(Node) );
	queue->tail = malloc( sizeof(Node) );
	
	queue->head->next = NULL;
	queue->head->prev = queue->tail;
	
	queue->tail->next = queue->head;
	queue->tail->prev = NULL;
}

int queue_is_empty(Queue * queue) {
	if (queue->head->prev == queue->tail)
		return 1;
	return 0;
}

void queue_push(Queue * queue, Data val) {
	Node * item = malloc( sizeof( Node ) );
	item->val = val;
	
	queue->tail->next->prev = item;
	item->prev = queue->tail;
	item->next = queue->tail->next;
	queue->tail->next = item;
}

void queue_remove(Queue * queue, Node * item) {
	queue->head->prev = item->prev;
	item->prev->next = queue->head;
}

Data queue_pop(Queue * queue) {
	if ( queue_is_empty(queue) ) {
		printf("error: pop from empty queue\n");
		exit(0);
	}
	Data res = queue->head->prev->val;
	Node * item = queue->head->prev;
	queue_remove(queue, item);
	free(item);
	return res;
}

void queue_delete(Queue * queue) {
	free(queue->head);
	free(queue->tail);
	free(queue);
}
