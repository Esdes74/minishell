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

static char	*check_quote(char *str);
static int	execute_child(char **environ, char **str, t_cmd *pip);

int cmd_center_simple(char **str, t_cmd *pip) //j'ai enlever la condtion si env != NULL 
{
    if (search_builtins(str, pip) == -1)
        return (free_all(pip), -1);
    if (execute_child(pip->env, str, pip) == 1)
        return (-1); // changement de 1 à -1
    return (0);
}

static int	execute_child(char **environ, char **str, t_cmd *pip)
{
	char	*cmd;
    char    *false_cmd;
    char    *new_str;

    new_str = check_quote(str[0]);
    free(str[0]);
    str[0] = new_str;
	cmd = cmd_build(str[0], environ);
	if (cmd == NULL)
    {
        false_cmd = ft_calloc(1, 1);
        if (!false_cmd)
            return (free(str), 1);
        if (write(STDOUT_FILENO, false_cmd, 1) == -1)
            return (free (false_cmd), free(str), free_all(pip), 1);
		return (free(str), free(false_cmd), free_all(pip), exit(EXIT_FAILURE), 1);
    }
    annihilation(list, free, DEBUG);
	execve(cmd, str, environ);
	error(EXEC, "0");
	return (free(cmd), anihilation(str), free_all(pip), 1);
}

static char	*check_quote(char *str)
{
	int	i;
	int j;
	char *new_str;

	j = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '"' || str[i] == '\'')
			j++;
		i++;
	}
	if (j == 0)
		return (str);
	new_str = malloc(sizeof(char) * ((i - j) + 1));
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != '"' && str[i] != '\'')
			new_str[j++] = str[i];
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}