#include <stdio.h>
#include <stdlib.h>
int typedef Data;

struct Stack {
	int n; //size of stack
	int size; //size of memory
	Data* a;
};

struct Stack * stack_create(int size) {
	struct Stack * stk = malloc (1*sizeof(struct Stack));
	stk->size = size + 1;
	stk->n = 0;
	stk->a = malloc (stk->size * sizeof(Data));
	return stk;
}

void stack_push(struct Stack * s, Data x) {
	if (s->n < s->size) {
		s->a[s->n] = x;
		s->n++;
	}
	else {
		s->size *= 7;
		s->a = realloc(s->a, (s->size)*sizeof(Data));
		s->a[s->n] = x;
		s->n++;
	}
}

int  stack_is_empty(struct Stack * s) {
	if (s->n != 0)
		return 0;
	else
		return 1;
}

Data stack_pop(struct Stack * s) {
	if (stack_is_empty(s)) {
		printf("error: pop from empty stack\n");
		exit(0);
	}
	s->n--;
	return s->a[s->n];
}

Data stack_top(struct Stack * s) {
	return s->a[s->n-1];
}

void stack_print(struct Stack * s) {
	if (s->n != 0) {
		int i;
		for (i = 0; i < s->n; i++)
		{
			printf("%d ", s->a[i]);
		}
		printf("\n");
	} else
		printf("Empty stack\n");
	
}
int  stack_size(struct Stack * s) {
	return s->n;
}

void stack_clear(struct Stack * s) {
	s->n = 0;
}
void stack_destroy(struct Stack * s) {
	free(s->a);
	free(s);
}
