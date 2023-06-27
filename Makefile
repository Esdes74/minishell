# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/04 16:30:03 by eslamber          #+#    #+#              #
#    Updated: 2023/06/27 16:59:44 by eslamber         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#
### Definitions of variables
#

# Compilation flags
FLAGS := -Wall -Werror -Wextra
CC := gcc

#
### Definition of variables
#

# Definition of lib
LIB := libft/libft.a
INC := -Ilibft
LIBRAIRIE := -lreadline

# Definition of project variables
NAME := minishell
HEADER := minishell.h

# Definition of files variables
SRC := main.c \
	   signals.c
OBJ := $(SRC:%.c=.obj/%.o)

#
### Compilation rules
#

# Compilation
all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $@ -L. $(LIB) $(LIBRAIRIE)

.obj/%.o: %.c $(HEADER) $(LIB)
	$(CC) $(FLAGS) $(INC) -c $< -o $@ 

$(LIB): FORCE
	make -C libft

# Debug
debug:
	make -C libft debug
	make -C ./ "FLAGS = -Wall -Werror -Wextra -fsanitize=address \
	-fno-omit-frame-pointer"

val_deb:
	make -C libft valgrind_deb
	make -C ./ "FLAGS = -Wall -Werror -Wextra -g"

bonus_deb:
	make -C libft valgrind_deb
	make -C ./ "FLAGS = -Wall -Werror -Wextra -g" bonus

#
### Cleanup rules
#

clean:
	@rm -rfv $(OBJ) $(OBJ_B)
	make -sC libft clean

fclean:
	@rm -rfv $(OBJ) $(OBJ_B) $(NAME)
	make -sC libft fclean

re: fclean all

re_deb: fclean debug

re_val: fclean val_deb

FORCE:

.PHONY: all clean fclean re FORCE debug re_deb val_deb re_val
