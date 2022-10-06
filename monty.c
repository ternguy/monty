#include "monty.h"

unsigned int line_number = 0;

/**
 * main - control program flow
 * @argc: argument count
 * @argv: argument list
 * Return: 0 Always
 */
int main(int argc, char *argv[])
{
	char **tokens = NULL;
	stack_t *head = NULL; /* ptr to top of stack */
	char *buffer = NULL; /* stores the content from the file */
	FILE *fp;
	size_t n;

	if (argc != 2)
	{
		fprintf(stderr, "USAGE: monty file\n");
		exit(EXIT_FAILURE);
	}

	fp = fopen(argv[1], "r+");
	if (fp == NULL)
	{
		fprintf(stderr, "Error: Can't open file %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	while ((getline(&buffer, &n, fp)) != -1)
	{
		line_number++;
		tokens = tokenize(buffer);
		/*DEBUG*/
		/*
		 * printf("%s", *tokens);
		 * printf("-----\n");
		 */

		if (tokens != NULL && strcmp(*tokens, "$") != 0)
		{
			call(tokens, &head);
			free(tokens);
		}
	}
	free(buffer);
	free_stack(&head);
	fclose(fp);

	return (0);
}

/**
 * free_stack - free the stack
 * @stack: ptr to stack
 * Return: Nothing
 */
void free_stack(stack_t **stack)
{
	stack_t *head = *stack;

	while (head)
	{
		if (!head->next)
		{
			free(head);
			break;
		}
		head = head->next;
		free(head->prev);
	}
}

/**
 * tokenize - tokenize a given buffer
 * @buffer: string to tokenize
 *
 * Return: an array
 */
char **tokenize(char *buffer)
{
	char *token = NULL; /* ptr to next token */
	char **store; /* store tokens in array */
	int idx = 3;
	char *delim = "\t  \r\n";

	token = strtok(buffer, delim);

	if (token == NULL)
		return (NULL);

	store = malloc(sizeof(char **) * 3);
	if (store == NULL)
	{
		fprintf(stderr, "Error: malloc failed");
		free(buffer);
		exit(EXIT_FAILURE);
	}

	while (--idx)
		store[idx - 1] = NULL;

	while (token)
	{
		if (idx < 3)
			store[idx] = token;
		token = strtok(NULL, delim); /* pt to next token */
		idx++;
	}

	store[2] = NULL;

	return (store);
}

static instruction_t operations[] = {
		{"push", f_push},
		{"pall", f_pall},
		{"pop", f_pop},
		{"swap", f_swap},
		{"add", f_add},
		{"nop", NULL},
		{"pint", f_pint},
		{"mul", f_mul},
		{"div", f_div},
		{"mod", f_mod},
		{"sub", f_sub},
		{"pchar", f_pchar},
		{"pstr", f_pstr},
		{"rotl", f_rotl},
		{"rotr", f_rotr},
		{"stack", f_stack},
		{"queue", f_queue},
		{NULL, NULL}
	};

/**
 * call - call appropriate function
 * @tokens: pointer to array of tokens
 * @stack: pointer to a stack
 *
 * Return: nothing
 */
void call(char **tokens, stack_t **stack)
{
	int idx = 0;

	while (operations[idx].opcode)
	{
		if (tokens[0][0] == '#') /* incase of comments in file*/
			return;
		if (strcmp(tokens[0], operations[idx].opcode) == 0)
		{
			if (operations[idx].f)
				operations[idx].f(stack, line_number);
			break;
		}
		idx++;
	}

	if (strcmp(tokens[0], "push") == 0)
	{
		is_valid(tokens, stack);
		(*stack)->n = atoi(tokens[1]);
	}
	else if (!(operations[idx].opcode))
	{
		fprintf(stderr, "L%u: unknown instruction %s\n",
				line_number,
				tokens[0]);
		exit(EXIT_FAILURE);
	}
}
