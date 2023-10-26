# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/04 16:30:03 by eslamber          #+#    #+#              #
#    Updated: 2023/10/26 22:20:45 by dbaule           ###   ########.fr        #
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
REPO_HEADER := incs/
HEADER := $(REPO_HEADER)minishell.h

# Definition of files variables
SRC := main.c \
	   signals.c \
	   signals_handler.c \
	   error.c \
	   cmd_build.c \
	   historic.c \
	   builtins.c \
	   free_struc.c \
	   cmd_center_simple.c \
	   parsing/parsing.c \
	   parsing/parse_arguments.c \
	   parsing/parsing_count_arg.c \
	   parsing/parsing_count_caractere.c \
	   verif_read/verif_read.c \
	   verif_read/verif_read_utils_bis.c \
	   verif_read_utils.c \
	   execution_center.c \
	   check_redirection.c \
	   prep_heredoc.c \
	   search_builtins.c \
	   builtins_parent.c \
	   environement/environement.c \
	   environement/exp_env.c \
	   check_variables.c \
	   builtins_utils.c \
	   export/export.c \
	   export/add_exp_env.c \
	   export/check_if_replace_env.c \
	   export/check_if_replace_exp_env.c \
	   export/export_utils.c \
	   expand.c \
	   minishell_utils.c \
	   check_redirection_parent.c \
	   unset.c \
	   echo.c \
	   echo_utils.c
OBJ := $(SRC:%.c=.obj/%.o)

#
### Compilation rules
#

# Compilation
all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $@ -L. $(LIB) $(LIBRAIRIE)

.obj/parsing/%.o: parsing/%.c $(REPO_HEADER)parsing.h
	$(CC) $(FLAGS) $(INC) -c $< -o $@ $(LIBRAIRIE)

.obj/export/%.o: export/%.c $(REPO_HEADER)export.h
	$(CC) $(FLAGS) $(INC) -c $< -o $@ $(LIBRAIRIE)

.obj/verif_read/%.o: verif_read/%.c $(REPO_HEADER)verif_read.h
	$(CC) $(FLAGS) $(INC) -c $< -o $@ $(LIBRAIRIE)

.obj/environement/%.o: environement/%.c $(REPO_HEADER)environement.h
	$(CC) $(FLAGS) $(INC) -c $< -o $@ $(LIBRAIRIE)
	
.obj/%.o: %.c $(HEADER) $(LIB)
	$(CC) $(FLAGS) $(INC) -c $< -o $@ $(LIBRAIRIE)

$(LIB): FORCE
	make -C libft

# Debug
debug:
	make -C libft debug
	make -C ./ "FLAGS = -Wall -Werror -Wextra -fsanitize=address \
	-fno-omit-frame-pointer -g3"

val_deb:
	make -C libft valgrind_deb
	make -C ./ "FLAGS = -Wall -Werror -Wextra -g3"

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
