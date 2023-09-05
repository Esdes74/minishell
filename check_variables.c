/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 11:08:50 by eslamber          #+#    #+#             */
/*   Updated: 2023/08/29 11:08:54 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int  replace_variable(char *data, char *sent, t_cell *tmp, char **env);

int check_variables(t_list *spt, char **env)
{
    t_cell  *tmp;
    char    *data;
    int     flag;
    int     i;

    flag = 0;
    tmp = spt->head;
    while (tmp) // Boucle sur chque éléments de la commande
    {
        data = (char *) tmp->data_cell->data; // Simplification de lecture
        i = 0;
        while (data[i]) // Je regarde chaques caractères
        {
            if (flag == 1 && data[i] == '\'') // J'enlève le mode (single quote)
                flag = 0;
            else if (flag == 0 && data[i] == '\'') // Je remet le mode (single quote)
                flag = 1;
            if (flag != 1 && data[i] == '$') // Je ne suis pas dans le mode single quote et je trouve un $
            {
                if (replace_variable(&data[i], data, tmp, env) == 1) // J'appel la fonction qui le remplace
                    return (1);
                data = (char *) tmp->data_cell->data;
            }
            i++;
        }
        tmp = tmp->next;
    }
    return (0);
}

static int  replace_variable(char *data, char *sent, t_cell *tmp, char **env)
{
    int     i;
    int     j;
    int     len_data;
    int     len_tot;
    int     flag;
    char    *new;
    char    *new_sent;

    // La je trouve la fin du mot qui représente la variable (si c'est $PATH j'arrive après le H)
    i = 0;
    while (data[i] && data[i] != ' ' && data[i] != '\n' && data[i] != '\0')
        i++;
    len_data = i;


    new = (char *) malloc(sizeof(char) * (i + 1)); // Je fais un malloc afin de pouvoir de copier
    if (new == NULL)
        return (1);
    ft_strlcpy(new, &data[1], i); // Je le copie
    new[i] = '\0';


    i = 0;
    // Je parcours l'environnement pour retrouver la variable en question
    while (env[i])
    {
        j = 0;
        while (env[i][j] != '=' && env[i][j] != '\0' && env[i][j] == new[j] \
        && new[j] != '\0')
            j++;
        if (env[i][j] == '=' && new[j] == '\0') // Je l'ai retrouvé alors je break (a remplacer)
            break ;
        i++;
    }


    free(new);
    if (env[i] == NULL) // Si j'ai rien trouvé alors je sors
        return (0);
    new = ft_strdup(&env[i][j + 1]); // je dup la valeur de la variable pour pouvoir la manipuler
    if (new == NULL) // Je teste
        return (1);


    // Je créer la nouvelle phrase qui va acceuillir l'ancienne plus la valeur
    len_tot = ft_strlen(sent) - len_data + ft_strlen(new);
    new_sent = (char *) malloc(sizeof(char) * (len_tot + 1));
    if (new_sent == NULL) // Je test
        return (free(new), 1);


    // Je met ce qu'il faut dans la nouvelle phrase (l'ancienne plus la valeur)
    i = 0;
    j = 0;
    len_data = 0;
    flag = 0;
    while (i < len_tot)
    {
        if (flag == 0 && sent[j] == '\'')
            flag = 1;
        else if (flag == 0 && sent[j] == '$')
        {
            flag = 2;
            while (sent[j] != '\0' && sent[j] != ' ')
                j++;
        }
        else if (flag == 1 && sent[j] == '\'')
            flag = 0;
        if (flag == 2)
        {
            if (new[len_data] != '\0')
                new_sent[i] = new[len_data++];
            else
                flag = -1;
        }
        else
            new_sent[i] = sent[j++];
        i++;
    }
    new_sent[i] = '\0';
    free(sent);
    free(new);
    tmp->data_cell->data = new_sent;
    return (0);
}