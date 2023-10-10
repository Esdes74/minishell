/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   historic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 11:30:24 by dbaule            #+#    #+#             */
/*   Updated: 2023/08/04 17:02:18 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
History :
-	Faire deux fois la même commande d'affilé ne le note pas, sauf s'il y a une différence (un espace suffit)
-   mettre dans la liste l'historique (normalemnt stocké dans la ram, mais go la stocke dans la liste pendant tout le programme)
-   !(nombre de la commande dans l'historique), permet de lancer la commande en question selon son nombre dans l'historique, rajouter quelque chose +
!(nombre de la commande) permet de rajouter à la commande choisis dans l'historique
-   !! lance la derniere comande faite dans l'historique
-   !-(nombre : n) execute la derniere commande -n (si !-2) execute la commande avant la derniere commande !-1 reviens à !!, ne rajoute pas à l'historique lors de l'utilisation de l'historique
-   history (nombre : n) renvoie les n dernieres commandes
*/


int historic_fct(char *bufff, char *test, t_cmd *pip) // gérer les espace dans l'historique
{
    int     i;
    int     save;
    int     flag;
    char    *tmp;
    char    *buf;

    i = 0;
    flag = 0;
    save = 0;
    // Cette boucle sert a placer le i derrière la dernière quote afin 
    // de mettre un \0 et de pouvoir rajouter le hd_history
    while (test[i] != '\0')
    {
        if (test[i] == '"' && flag == 0)
            flag = 1;
        else if (test[i] == '"' && flag == 1)
        {
            flag = 0;
            save = i + 1;
        }
        else if (test[i] == '\'' && flag == 0)
            flag = 2;
        else if (test[i] == '\'' && flag == 2)
        {
            flag = 0;
            save = i + 1;
        }
        i++;
    }
    if (save != 0)
        flag = 1;
    i = save;
    while (test[i] != '\n' && test[i] != '\0')
        i++;
    if (test[i] == '\n')
        flag = 1;
    test[i] = '\0';

    if (pip->hd_history != NULL)
    {
        tmp = NULL;
        buf = NULL;
        i = 0;
        while (pip->hd_history[i])
        {
            if (i == 0)
                buf = ft_strjoin("\n", pip->hd_history[i]);
            else
                buf = ft_strjoin(tmp, pip->hd_history[i]);
            if (buf == NULL)
                return (1);
            free(tmp);
            tmp = buf;
            i++;
        }
        tmp = ft_strjoin(test, buf);
        free(buf);
    }
    else
        tmp = test;
    if (test[0] == ' ' || test[0] == '\0')
        return (0);
    if (bufff == NULL)
        add_history(tmp);
    else
    {
        if (strncmp(test, bufff, ft_strlen(test) + 1) != 0 || flag == 1)
            add_history(tmp);
    }
    if (tmp != test)
        free(tmp);
    return (0);
}
