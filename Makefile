NAME		=	minibash

SRC			=	$(addprefix src/,		main.c \
				$(addprefix builtin/,	builtin_cd.c builtin_env.c builtin_exit.c builtin_export.c builtin_null.c \
										builtin_unset.c get_builtin.c) \
				$(addprefix executor/,	executor.c execve_cmd.c fork_exec.c run_cmd.c wait_pids.c) \
				$(addprefix parser/,	expand.c expansor.c interpreter.c lexer.c parser.c) \
				$(addprefix signal/,	setup_signals.c) \
				$(addprefix utils/,		free_cmd.c free_token.c get_path.c list_clear.c list_free.c \
										list_last.c list_new.c list_push_back.c list_size.c matrix_free.c \
										new_cmd.c new_token.c quotes_len.c skip_whitespaces.c split.c \
										strjoin.c) \
				)

OBJ			=	$(SRC:.c=.o)

CC			=	gcc -O3

CFLAGS		=	-Wall -Werror -Wextra -Iinclude

FLAGS		=	-lreadline

RM			=	rm -f

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(FLAGS) -o $(NAME)

leaks: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes --suppressions=valgrind.supp ./$(NAME)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean:
	$(RM) $(OBJ) $(NAME)

re: fclean all

.PHONY: all clean fclean re
