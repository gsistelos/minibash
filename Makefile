NAME		=	minibash

SRCS		=	$(addprefix srcs/, main.c \
				)

OBJS		=	$(SRCS:.c=.o)

INCLUDES	=	-Iincludes/

CC			=	cc

CFLAGS		=	-Wall -Werror -Wextra

RM			=	rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

.c.o:
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean:
	$(RM) $(OBJS) $(NAME)

re: fclean all

.PHONY: all clean fclean re
