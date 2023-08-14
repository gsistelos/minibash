NAME		=	minibash

SRCS		=	$(addprefix srcs/, main.c \
				$(addprefix executor/, executor.c) \
				$(addprefix parser/, expansor.c interpreter.c lexer.c parser.c) \
				$(addprefix signal/, setup_signals.c) \
				$(addprefix test/, print_cmds.c print_fd.c print_tokens.c) \
				$(addprefix utils/,	expand.c free_cmd.c free_token.c get_path.c heredoc.c \
									new_cmd.c new_token.c quotes_len.c safe_close.c skip_whitespaces.c) \
				)

OBJS		=	$(SRCS:.c=.o)

LIBGS_DIR	=	libgs

LIBGS		=	$(LIBGS_DIR)/libgs.a

CC			=	gcc -O3

CFLAGS		=	-Wall -Werror -Wextra -Iincludes -I$(LIBGS_DIR)/includes

FLAGS		=	-lreadline -L$(LIBGS_DIR) -lgs

RM			=	rm -f

all: $(NAME)

$(NAME): $(OBJS) $(LIBGS)
	$(CC) $(OBJS) $(FLAGS) -o $(NAME)

$(LIBGS):
	make -C $(LIBGS_DIR)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make -C $(LIBGS_DIR) clean
	$(RM) $(OBJS)

fclean:
	make -C $(LIBGS_DIR) fclean
	$(RM) $(OBJS) $(NAME)

re: fclean all

.PHONY: all clean fclean re
