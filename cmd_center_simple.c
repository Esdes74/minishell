/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_center.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 15:17:58 by eslamber          #+#    #+#             */
/*   Updated: 2023/07/04 15:17:58 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
-   il faut prendre en compte entré et sortie en amont
-   uniquement la string qu'on lui envoie doit s'executer
-   le nombre de pipe et le nombre de processeur sera établis en amont
-   les dup et la fermeture des pipes en amont
-      en amont pour le heredoc
*/

#include "minishell.h"
static int	execute_child(char **environ, char **str);

int cmd_center_simple(char **str, t_cmd *pip) //j'ai enlever la condtion si env != NULL 
{
    if (search_builtins(str, pip) == -1)
        return (exit(EXIT_SUCCESS), 0);
    if (execute_child(pip->env, str) == 1)
        return (1);
    return (0);
}

static int	execute_child(char **environ, char **str)
{
	char	*cmd;
    char    *false_cmd;

	cmd = cmd_build(str[0], environ);
	if (cmd == NULL)
    {
        false_cmd = ft_calloc(1, 1);
        if (!false_cmd)
            return (free(str), 1);
        if (write(STDOUT_FILENO, false_cmd, 1) == -1)
            return (free (false_cmd), free(str), 1);
		return (free(str), free(false_cmd), exit(EXIT_FAILURE), 1);
    }
    annihilation(list, free, DEBUG);
	execve(cmd, str, environ);
	error(EXEC, "0");
	return (free(cmd), anihilation(str), 2);
}
