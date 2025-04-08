NAME        =   minishell
CC          =   cc
CFLAGS      =   -Wall -Wextra -Werror -g -I$(LIBFT_INC) -I/usr/include/readline
RM          =   rm -rf

# Libft
LIBFT_DIR   =   libft
LIBFT       =   $(LIBFT_DIR)/libft.a
LIBFT_INC   =   $(LIBFT_DIR)

# Readline
READLINE_LIB =   -lreadline

# Sources and objects
SRC         =   main.c utilits.c
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
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re