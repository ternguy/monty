#include "monty.h"

/**
 * is_valid - check if second spot of a token array is valid
 * @token: tokens value to check
 * @stack: pointer to a stack
 */
void is_valid(char **token, stack_t **stack)
{
	int idx = 0;

	if (!token[1])
	{
		fprintf(stderr, "L%u: usage: push integer\n", line_number);
		if (*stack)
			free_stack(stack);
		free(token);
		exit(EXIT_FAILURE);
	}

	while (token[1][idx])
	{
		if (token[1][idx] == '-' && idx == 0)
			idx++;
		if (isdigit(token[1][idx]) == 0)
		{
			fprintf(stderr, "L%u: usage: push integer\n", line_number);
			if (*stack)
				free_stack(stack);
			free(token);
			exit(EXIT_FAILURE);
		}
		idx++;
	}
}

/**
 * f_queue - function that ...
 * @stack: head of stack
 * @n: line count
 *
 * Return: nothing
 */
void f_queue(stack_t **stack, unsigned int n)
{
	(void)stack;
	(void)n;
	/*TODO: to convert the stack to queue */
}

/**
 * f_stack - function that ...
 * @stack: head of stack
 * @n: line count
 * Return: nothing
 */
void f_stack(stack_t **stack, unsigned int n)
{
	(void)stack;
	(void)n;
	/*TODO: to convert the queue to stack */
}
