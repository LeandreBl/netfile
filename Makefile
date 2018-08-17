##
## EPITECH PROJECT, 2017
##
## File description:
## makefile
##

NAME		= netfile

CC		= gcc

LIBS		= -llsocket
LIBS		+= -lm

SRCS		= src/main.c
SRCS		+= src/netsend.c
SRCS		+= src/netreceive.c
SRCS		+= src/display.c

OBJS		= $(SRCS:.c=.o)

RM		= rm -f

CFLAGS		= -Werror -Wall -Wextra
CFLAGS		+= -I ./include -std=gnu99
# uncomment this for debug flag
CFLAGS		+= -g3

GREEN		= '\033[0;32m'
NO_COLOR	= '\033[0m'

%.o : %.c
	@$ $(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
	@echo "$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ ["$(GREEN)"OK"$(NO_COLOR)"]"
.SUFFIXES: .o .c

all: $(NAME)

$(NAME): $(OBJS)
	@$ $(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $@
	@echo "$(CC) $(OBJS) $(LIBS) -o $@ \
	["$(GREEN)"LINKING OK"$(NO_COLOR)"]"

tests_run:

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re tests_run
