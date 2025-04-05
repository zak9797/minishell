SRC			= ft_memset ft_bzero ft_memcpy ft_memmove ft_memchr ft_memcmp ft_strlen ft_isalpha ft_isdigit ft_isalnum ft_isascii ft_isprint ft_toupper ft_tolower ft_strchr ft_strrchr ft_strcmp ft_strncmp ft_strlcpy ft_strlcat ft_strnstr ft_atoi ft_calloc ft_strdup ft_substr ft_strjoin ft_strtrim ft_split ft_itoa ft_strmapi ft_putchar_fd ft_putstr_fd ft_putendl_fd ft_putnbr_fd ft_striteri
B_SRC		= ft_lstnew_bonus ft_lstadd_front_bonus ft_lstsize_bonus ft_lstlast_bonus ft_lstadd_back_bonus ft_lstdelone_bonus ft_lstclear_bonus ft_lstiter_bonus ft_lstmap_bonus
SRCS		= $(addsuffix .c, ${SRC})
B_SRCS		= $(addsuffix .c, ${B_SRC})
OBJS		= ${SRCS:.c=.o}
B_OBJS		= ${B_SRCS:.c=.o}
NAME		= libft.a
CC			= cc
RM			= rm -f
AR			= ar rcs
CFLAGS		= -Wall -Wextra -Werror
all: $(NAME)
%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@
$(NAME): ${OBJS}
	${AR} ${NAME} ${OBJS}
bonus: $(B_OBJS) $(NAME)
	${AR} ${NAME} ${B_OBJS}
clean:
	${RM} ${OBJS} ${B_OBJS}
fclean: clean
	${RM} ${NAME}
re: fclean all
.PHONY:	all bonus clean fclean re
