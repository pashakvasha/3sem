#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "queue.h"

#define N 8
#define STACK_SIZE 100

int typedef Data;

Data graph[N][N] = {
		{0, 1, 1, 1, 0, 0, 0, 0},
		{1, 0, 0, 0, 1, 1, 0, 0},
		{1, 0, 0, 0, 0, 0, 0, 0},
		{1, 0, 0, 0, 0, 0, 0 ,0},
		{0, 1, 0, 0, 0, 0, 0, 0},
		{0, 1, 0, 0, 0, 0, 1, 1},
		{0, 0, 0, 0, 0, 1, 0, 0},
		{0, 0, 0, 0, 0, 1, 0, 0}
};

void find_DFS(struct Stack * s, int dir) {
	int i;
	if (0)
		return;
	else {
		for (i = dir; i < N; i++)
		{
			if ( graph[dir][i] ) {
				stack_push(s, i + 1);
				find_DFS(s, i);
			}
		}
	}
}

void find_BFS(Queue * queue) {
	Data dir;
	int i;
	while ( !queue_is_empty(queue) )
	{
		dir = queue_pop(queue);
		printf("%d ", dir + 1);
		if (0) {
			break;
		} else {
			for (i = dir; i < N; i++)
			{
				if (graph[dir][i]) {
					queue_push(queue, i);
				}
			}
		}
	}
}


int main() {
	int first_node = 0;
	
	struct Stack * stack = stack_create( STACK_SIZE );
	stack_push(stack, first_node + 1);
	find_DFS(stack, first_node);
	
	Queue * queue = malloc( sizeof(Queue) );
	queue_init(queue);
	queue_push(queue, first_node);
	find_BFS(queue);
	
	printf("\n");
	stack_print(stack);
	
	stack_clear(stack);
	stack_destroy(stack);
	
	queue_delete(queue);
	
	return 0;
}


