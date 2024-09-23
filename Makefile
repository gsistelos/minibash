NAME = minibash

SRC = $(addprefix src/, main.c \
	$(addprefix builtin/, builtin_cd.c builtin_env.c builtin_exit.c builtin_export.c builtin_null.c \
		builtin_unset.c get_builtin.c) \
	$(addprefix executor/, executor.c execve_cmd.c fork_exec.c run_cmd.c wait_pids.c) \
	$(addprefix parser/, expand.c expansor.c interpreter.c lexer.c parser.c) \
	$(addprefix signal/, setup_signals.c) \
	$(addprefix utils/, free_cmd.c free_token.c get_path.c new_cmd.c new_token.c \
		quotes_len.c skip_whitespaces.c) \
)

OBJ = $(SRC:.c=.o)

LIBGS_PATH = libgs

LIBGS = $(LIBGS_PATH)/libgs.a

LIBLST_PATH = liblst

LIBLST = $(LIBLST_PATH)/liblst.a

CC = gcc -O3

CFLAGS = -Wall -Werror -Wextra -Iinclude -I$(LIBGS_PATH)/include -I$(LIBLST_PATH)/include

RM = rm -f


.PHONY: all
all: $(NAME)

$(NAME): $(OBJ) $(LIBGS) $(LIBLST)
	$(CC) $(OBJ) -lreadline -L$(LIBGS_PATH) -lgs -L$(LIBLST_PATH) -llst -o $(NAME)

$(LIBGS_PATH):
	git clone https://github.com/gsistelos/libgs.git

$(LIBGS): $(LIBGS_PATH)
	make -C $(LIBGS_PATH)

$(LIBLST_PATH):
	git clone https://github.com/gsistelos/liblst.git

$(LIBLST): $(LIBLST_PATH)
	make -C $(LIBLST_PATH)

.PHONY: clean
clean:
	make -C $(LIBGS_PATH) clean
	make -C $(LIBLST_PATH) clean
	$(RM) $(OBJ)

.PHONY: fclean
fclean:
	make -C $(LIBGS_PATH) fclean
	make -C $(LIBLST_PATH) fclean
	$(RM) $(OBJ) $(NAME)

.PHONY: re
re: fclean all
