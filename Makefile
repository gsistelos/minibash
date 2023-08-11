NAME		=	minibash

SRCS		=	$(addprefix srcs/, main.c \
				$(addprefix parser/, expansor.c interpreter.c lexer.c parser.c) \
				$(addprefix test/, print_cmds.c print_fd.c print_tokens.c) \
				$(addprefix utils/, free_cmd.c free_token.c lst_clear.c lst_last.c lst_new.c lst_push.c new_cmd.c new_token.c quotes_len.c) \
				)

OBJS		=	$(SRCS:.c=.o)

INCLUDES	=	-Iincludes/

CC			=	cc

CFLAGS		=	-Wall -Werror -Wextra

FLAGS		=	-lreadline

RM			=	rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(FLAGS) -o $(NAME)

.c.o:
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean:
	$(RM) $(OBJS) $(NAME)

re: fclean all

.PHONY: all clean fclean re
