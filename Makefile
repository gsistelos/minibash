NAME		=	minibash

SRC			=	$(addprefix src/,		main.c \
				$(addprefix builtin/,	builtin_exit.c builtin_test.c get_builtin.c) \
				$(addprefix executor/,	executor.c execve_cmd.c fork_exec.c set_pipes.c wait_pids.c) \
				$(addprefix parser/,	expand.c expansor.c interpreter.c lexer.c parser.c) \
				$(addprefix signal/,	setup_signals.c) \
				$(addprefix test/,		print_cmds.c print_fd.c print_tokens.c) \
				$(addprefix utils/,		free_cmd.c free_token.c get_path.c new_cmd.c \
										new_token.c quotes_len.c skip_whitespaces.c) \
				)

OBJ			=	$(SRC:.c=.o)

LIBGS_DIR	=	libgs

LIBGS		=	$(LIBGS_DIR)/libgs.a

CC			=	gcc -O3

CFLAGS		=	-Wall -Werror -Wextra -Iinclude -I$(LIBGS_DIR)/include

FLAGS		=	-lreadline -L$(LIBGS_DIR) -lgs

RM			=	rm -f

all: $(NAME)

$(NAME): $(OBJ) $(LIBGS)
	$(CC) $(OBJ) $(FLAGS) -o $(NAME)

leaks: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes --suppressions=valgrind.supp ./$(NAME)

$(LIBGS):
	make -C $(LIBGS_DIR)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make -C $(LIBGS_DIR) clean
	$(RM) $(OBJ)

fclean:
	make -C $(LIBGS_DIR) fclean
	$(RM) $(OBJ) $(NAME)

re: fclean all

.PHONY: all clean fclean re
