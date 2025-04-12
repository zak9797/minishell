FT_PRINTF_DIR = ft_printf
GET_NEXT_LINE_DIR = get_next_line

# Sources for libft
SRC         = ft_memset ft_bzero ft_memcpy ft_memmove ft_memchr ft_memcmp ft_strlen \
              ft_isalpha ft_isdigit ft_isalnum ft_isascii ft_isprint ft_toupper \
              ft_tolower ft_strchr ft_strrchr ft_strcmp ft_strncmp ft_strlcpy \
              ft_strlcat ft_strnstr ft_atoi ft_calloc ft_strdup ft_substr ft_strjoin \
              ft_strtrim ft_split ft_itoa ft_strmapi ft_putchar_fd ft_putstr_fd \
              ft_putendl_fd ft_putnbr_fd ft_striteri

# Sources for bonus (libft)
B_SRC       = ft_lstnew_bonus ft_lstadd_front_bonus ft_lstsize_bonus ft_lstlast_bonus \
              ft_lstadd_back_bonus ft_lstdelone_bonus ft_lstclear_bonus ft_lstiter_bonus \
              ft_lstmap_bonus

# Sources for ft_printf
FT_PRINTF_SRC = ft_printf/ft_printf.c ft_printf/ft_putchar.c ft_printf/ft_putaddress.c \
                ft_printf/ft_put_hexa.c ft_printf/ft_putnbr.c ft_printf/ft_putstr.c \
                ft_printf/ft_put_unsigned.c

# Sources for get_next_line
GET_NEXT_LINE_SRC = get_next_line/get_next_line.c get_next_line/get_next_line_utils.c

# All source files (libft + bonus + ft_printf + get_next_line)
SRCS        = $(addsuffix .c, ${SRC})
B_SRCS      = $(addsuffix .c, ${B_SRC})
FT_PRINTF_SRCS = $(FT_PRINTF_SRC)
GNL_SRCS    = $(GET_NEXT_LINE_SRC)

# Object files
OBJS        = ${SRCS:.c=.o}
B_OBJS      = ${B_SRCS:.c=.o}
FT_PRINTF_OBJS = ${FT_PRINTF_SRCS:.c=.o}
GNL_OBJS    = ${GNL_SRCS:.c=.o}

# Final library name
NAME        = libft.a

# Compiler and tools
CC          = cc
RM          = rm -f
AR          = ar rcs
CFLAGS      = -Wall -Wextra -Werror

# Makefile targets
all: $(NAME)

# Compile object files
%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

# Create the static library (libft + ft_printf + get_next_line)
$(NAME): ${OBJS} ${FT_PRINTF_OBJS} ${GNL_OBJS}
	${AR} ${NAME} ${OBJS} ${FT_PRINTF_OBJS} ${GNL_OBJS}

# Bonus target
bonus: $(B_OBJS) ${FT_PRINTF_OBJS} ${GNL_OBJS} $(NAME)
	${AR} ${NAME} ${B_OBJS} ${FT_PRINTF_OBJS} ${GNL_OBJS}

# Clean object files
clean:
	${RM} ${OBJS} ${B_OBJS} ${FT_PRINTF_OBJS} ${GNL_OBJS}

# Clean everything (objects and library)
fclean: clean
	${RM} ${NAME}

# Rebuild everything
re: fclean all

# Phony targets
.PHONY: all bonus clean fclean re
