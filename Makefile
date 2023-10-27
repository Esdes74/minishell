# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/04 16:30:03 by eslamber          #+#    #+#              #
#    Updated: 2023/10/27 15:15:14 by dbaule           ###   ########.fr        #
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

# Definitions of differents repos
PARS := parsing/
ECHO := echo/
EXP := export/
VERIF := verif_read/
ENV := environement/
BUILTINS := builtins/
PREP := prep_heredoc/
RED := check_redirection/

# Definition of files variables
SRC := main.c \
	   signals.c \
	   signals_handler.c \
	   error.c \
	   cmd_build.c \
	   historic.c \
	   free_struc.c \
	   cmd_center_simple.c \
	   $(PARS)parsing.c \
	   $(PARS)parse_arguments.c \
	   $(PARS)parsing_count_arg.c \
	   $(PARS)parsing_count_caractere.c \
	   $(VERIF)verif_read.c \
	   $(VERIF)verif_read_utils_bis.c \
	   $(VERIF)verif_read_utils.c \
	   $(BUILTINS)builtins_utils.c \
	   $(BUILTINS)search_builtins.c \
	   $(BUILTINS)builtins_parent.c \
	   $(BUILTINS)builtins.c \
	   $(BUILTINS)builtins_utils_bis.c \
	   $(BUILTINS)search_builtins_parent.c \
	   execution_center.c \
	   $(RED)check_redirection.c \
	   $(RED)duplication.c \
	   $(RED)check_red_utils.c \
	   $(PREP)prep_heredoc.c \
	   $(PREP)prep_heredoc_utils.c \
	   $(PREP)handle_history.c \
	   $(ENV)environement.c \
	   $(ENV)exp_env.c \
	   $(EXP)export.c \
	   $(EXP)add_exp_env.c \
	   $(EXP)check_if_replace_env.c \
	   $(EXP)check_if_replace_exp_env.c \
	   $(EXP)export_utils.c \
	   expand.c \
	   minishell_utils.c \
	   check_redirection_parent.c \
	   unset.c \
	   $(ECHO)echo.c \
	   $(ECHO)echo_utils.c
OBJ := $(SRC:%.c=.obj/%.o)

#
### Compilation rules
#

# Compilation
all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $@ -L. $(LIB) $(LIBRAIRIE)

.obj/$(PARS)%.o: $(PARS)%.c $(REPO_HEADER)parsing.h $(HEADER) $(LIB)
	$(CC) $(FLAGS) $(INC) -c $< -o $@ $(LIBRAIRIE)

.obj/$(EXP)%.o: $(EXP)%.c $(REPO_HEADER)export.h $(HEADER) $(LIB)
	$(CC) $(FLAGS) $(INC) -c $< -o $@ $(LIBRAIRIE)

.obj/$(VERIF)%.o: $(VERIF)%.c $(REPO_HEADER)verif_read.h $(HEADER) $(LIB)
	$(CC) $(FLAGS) $(INC) -c $< -o $@ $(LIBRAIRIE)

.obj/$(ENV)%.o: $(ENV)%.c $(REPO_HEADER)environement.h $(HEADER) $(LIB)
	$(CC) $(FLAGS) $(INC) -c $< -o $@ $(LIBRAIRIE)

.obj/$(ECHO)%.o: $(ECHO)%.c $(REPO_HEADER)echo.h $(HEADER) $(LIB)
	$(CC) $(FLAGS) $(INC) -c $< -o $@ $(LIBRAIRIE)

.obj/$(BUILTINS)%.o: $(BUILTINS)%.c $(REPO_HEADER)builtins.h $(HEADER) $(LIB)
	$(CC) $(FLAGS) $(INC) -c $< -o $@ $(LIBRAIRIE)
	
.obj/$(PREP)%.o: $(PREP)%.c $(REPO_HEADER)prep_heredoc.h $(HEADER) $(LIB)
	$(CC) $(FLAGS) $(INC) -c $< -o $@ $(LIBRAIRIE)

.obj/$(RED)%.o: $(RED)%.c $(REPO_HEADER)check_redirection.h $(HEADER) $(LIB)
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
