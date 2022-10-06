#ifndef MONTY_H
#define MONTY_H
#define  _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

extern unsigned int line_number;

/**
 * struct stack_s - doubly linked list representation of a stack (or queue)
 * @n: integer
 * @prev: points to the previous element of the stack (or queue)
 * @next: points to the next element of the stack (or queue)
 *
 * Description: doubly linked list node structure
 * for stack, queues, LIFO, FIFO Holberton project
 */
typedef struct stack_s
{
	int n;
	struct stack_s *prev;
	struct stack_s *next;
} stack_t;


/**
 * struct instruction_s - opcode and its function
 * @opcode: the opcode
 * @f: function to handle the opcode
 *
 * Description: opcode and its function
 * for stack, queues, LIFO, FIFO Holberton project
 */
typedef struct instruction_s
{
	char *opcode;
	void (*f)(stack_t **stack, unsigned int line_number);
} instruction_t;

stack_t *build_list(stack_t *head);
void call(char **tokens, stack_t **stack);
char **tokenize(char *buffer);
stack_t *add_node_start(stack_t *h, int n);
void f_push(stack_t **stack, unsigned int n);
void f_pall(stack_t **stack, unsigned int n);
void f_swap(stack_t **stack, unsigned int line_number);
void f_add(stack_t **stack, unsigned int line_number);
void f_sub(stack_t **stack, unsigned int line_number);
void f_div(stack_t **stack, unsigned int line_number);
void f_mod(stack_t **stack, unsigned int line_number);
void f_pint(stack_t **stack, unsigned int line_number);
void f_nop(stack_t **stack, unsigned int line_number);
void f_mul(stack_t **stack, unsigned int line_number);
void f_pop(stack_t **stack, unsigned int line_number);
void f_pchar(stack_t **stack, unsigned int line_number);
void f_pstr(stack_t **stack, unsigned int line_number);
void f_rotl(stack_t **stack, unsigned int line_number);
void f_rotr(stack_t **stack, unsigned int line_number);
void f_queue(stack_t **stack, unsigned int n);
void f_stack(stack_t **stack, unsigned int n);
void is_valid(char **token, stack_t **stack);
void free_stack(stack_t **stack);

#endif
