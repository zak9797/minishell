NAME        =   minishell
SHELL := /bin/bash
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -I$(LIBFT_INC) -I/usr/include/readline -I.
RM          =   rm -rf

# Libft
LIBFT_DIR   =   libft
LIBFT       =   $(LIBFT_DIR)/libft.a
LIBFT_INC   =   $(LIBFT_DIR)

# Readline
READLINE_LIB =   -lreadline

# Sources and objects
SRC         =   main.c redirections/r1.c redirections/r2.c redirections/r3.c redirections/r4.c redirections/r5.c expander/expander_utils1.c expander/expander_utils2.c tokinizer/t1.c tokinizer/t2.c tokinizer/t3.c tokinizer/t4.c  execution/external.c execution/u1.c execution/u2.c execution/u3.c execution/u4.c  builtin/builtin_cmd.c builtin/pwd.c builtin/env.c builtin/env1.c builtin/cd.c builtin/cd1.c builtin/echo.c builtin/export.c builtin/unset.c builtin/exit.c
OBJS        =   $(SRC:.c=.o)

# Linker flags (libraries only)
LDFLAGS     =   $(LIBFT) $(READLINE_LIB)

# Rules
all:    $(NAME)

$(NAME):    $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	$(RM) $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME) a.out
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
