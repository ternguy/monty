#include "monty.h"

/**
 * f_div - swaps the data in two nodes
 * @stack: pointer to a stack_t
 * @line_number: line number
 * Return: Nothing
 */
void f_div(stack_t **stack, unsigned int line_number)
{
	if (stack == NULL || (*stack == NULL) || ((*stack)->next == NULL))
	{
		fprintf(stderr, "L%u: can't div, stack too short\n", line_number);
		if (*stack)
			free_stack(stack);
		exit(EXIT_FAILURE);
	}
	if ((*stack)->n == 0)
	{
		fprintf(stderr, "L%u: division by zero\n", line_number);
		free_stack(stack);
		exit(EXIT_FAILURE);
	}
	(*stack)->next->n = ((*stack)->next->n) / ((*stack)->n);
	f_pop(stack, line_number);
}

/**
 * f_mod - mods the data in two nodes
 * @stack: pointer to a stack_t
 * @line_number: line number
 * Return: Nothing
 */
void f_mod(stack_t **stack, unsigned int line_number)
{
	if (stack == NULL || (*stack == NULL) || ((*stack)->next == NULL))
	{
		fprintf(stderr, "L%u: can't mod, stack too short\n", line_number);
		if (*stack)
			free_stack(stack);
		exit(EXIT_FAILURE);
	}
	if ((*stack)->n == 0)
	{
		fprintf(stderr, "L%u: division by zero\n", line_number);
		free_stack(stack);
		exit(EXIT_FAILURE);
	}
	(*stack)->next->n = ((*stack)->next->n) % ((*stack)->n);
	f_pop(stack, line_number);
}

/**
 * f_mul - multiply and pop nodes
 * @stack: pointer to stack_t
 * @line_number: number of lines
 * Return: Nothing
 */
void f_mul(stack_t **stack, unsigned int line_number)
{
	if (stack == NULL || (*stack == NULL) || ((*stack)->next == NULL))
	{
		fprintf(stderr, "L%u: can't mul, stack too short\n", line_number);
		if (*stack)
			free_stack(stack);
		exit(EXIT_FAILURE);
	}
	(*stack)->next->n = ((*stack)->next->n) * ((*stack)->n);
	f_pop(stack, line_number);
}

/**
 * f_pop - remove the top elem of a stack
 * @stack: pointer to top of a stack
 * @n: line number
 */
void f_pop(stack_t **stack, unsigned int n)
{
	stack_t *node;

	(void) n;

	if (!stack || !(*stack))
	{
		fprintf(stderr, "L%u: can't pop an empty stack\n", n);
		exit(EXIT_FAILURE);
	}

	node = *stack;

	if ((*stack)->next)
	{
		*stack = (*stack)->next;
		(*stack)->prev = NULL;
	}
	else
		*stack = NULL;

	free(node);
}
