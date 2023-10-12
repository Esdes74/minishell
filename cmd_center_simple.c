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

static int	execute_child(char **environ, char **str, t_cmd *pip);

int cmd_center_simple(char **str, t_cmd *pip) //j'ai enlever la condtion si env != NULL 
{
    if (search_builtins(str, pip) == 1)
        return (1);
    if (execute_child(pip->env, str, pip) == 1)
        return (1); // changement de 1 à -1
    return (-1);
}

static int	execute_child(char **environ, char **str, t_cmd *pip)
{
	int		i;
	char	*cmd;
    char    *new_str;


	i = 0;
	while (str[i])
	{
		new_str = check_quote(str[i]);
		if (str[i] != new_str)
			free(str[i]);
		str[i] = new_str;
		i++;
	}
	cmd = cmd_build(str[0], environ);
	if (cmd == NULL)
		return (pip->status = 127, 1); // anihilation(str), free(false_cmd)
    annihilation(list, free, DEBUG);
	pip->status = 0;
	execve(cmd, str, environ);
	error(EXEC, "0");
	pip->status = 126;
	return (1);//anihilation(str), free_all(pip), 1);
}

char	*check_quote(char *str)
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