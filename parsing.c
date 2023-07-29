/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 15:16:22 by dbaule            #+#    #+#             */
/*   Updated: 2023/07/21 09:54:05 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//  - 1 : prob au niveau des espaces, le bash de base semble fonctionner lorsque l'on sépare pas les commandes c'est à dire que ls|grep minishell, fonctionne!
// par contre si je fais "ls|grep minishell", il ,ne fonctionne pas, les guillemets veut dire : "l'intérieur est une commande"

//  - 2 : notre minishell actuel ne fonctionne pas lorsque l'on met une commande entre "", tout simplement parce qu'il prend en compte comme char les guillemets
// Attention ! le vrai bash ne fontionne pas non plus si l'on met les commandes entre guillemets si l'on ajoute une option comme "ls -a" 
// donc guillemets veut dire "commande", si l'on rajouter quoi que ce soit qui diffère d'une commande comme un espace entre les guillemts, celle-ci ne fonctionnera pas

//  - 3 : ca me parait bizarre, mais apperement si je tape ça dans le bash  : ls -a|grep minishell|                 wc -l
// et bien il effectue bien la commande, même si les pipes sont collé (à verifier)

//  -4 : si dans le cas ou les pipes peuvent etre collé

/*
This function checks whether the parameter sent by the user is actually the one he should send in terms of syntax.
*/

int parsing_check(char *rd_line) // on peux compter le nombrede pipe dans cette fonction
{
    int i;
    int j;
    char **buf;

    i = 0;
    j = 0;
    buf = ft_split(rd_line, ' ');
    if (buf == NULL)
        return (error(SPLIT, 0), 1);
    while (buf[i])
    {
        if (buf[i][j] == '|' && buf[i + 1] && buf[i + 1][j] == '|')
            return(ft_printf_fd(2, "syntax error near unexpected token %c\n", buf[i][j]), 1); // faut pas free rd_line, enleve de l'historique
        while (buf[i][j])
        {
            //if (buf[i][j] == '"') il ne faut pas regarder dans les "" !
            j++;
        }
        j = 0;
        i++;
    }
    return (0);
}

char    **second_parsing_check(char *rd_line)
{
    int     i;
    int     j;
    int     flag;
    int     compt;
    int     *tab;
    char    **spt;

    i = 0;
    flag = 0;
    compt = 1;
    spt = 0;
    while (rd_line[i])
    {
        if (rd_line[i] != ' ' && flag == 3)
            flag = 0;
        if (rd_line[i] == '"' && flag == 0)
            flag = 1;
        else if (rd_line[i] == '"' && flag == 1)
            flag = 0;
        else if (rd_line[i] == '\'' && flag == 0)
            flag = 2;
        else if (rd_line[i] == '\'' && flag == 2)
            flag = 0;
        else if (rd_line[i] == ' ' && flag == 0)
            compt++;
        if (rd_line[i] == '|' && rd_line[i - 1] != ' ' && flag == 0)
            compt++;
        if (rd_line[i] == '|' && rd_line[i + 1] != ' ' && flag == 0)
            compt++;
        if (rd_line[i] == ' ' && flag == 0)
            flag = 3;
        i++;
    }
    ft_printf("compt = %d\n", compt);
    spt = (char **) malloc(sizeof(char *) * (compt + 1));
    if (spt == NULL)
        return (error(MALLOC, NULL), NULL);
    spt[compt] = NULL;
    tab = (int *) ft_calloc(compt, sizeof(int));
    if (tab == NULL)
        return (error(MALLOC, NULL), NULL);
    i = 0;
    compt = 0;
    flag = 0;
    while (rd_line[i])
    {
        if (rd_line[i] != ' ' && flag == 3)
            flag = 0;
        if (rd_line[i] == '"' && flag == 0)
            flag = 1;
        else if (rd_line[i] == '"' && flag == 1)
            flag = 0;
        else if (rd_line[i] == '\'' && flag == 0)
            flag = 2;
        else if (rd_line[i] == '\'' && flag == 2)
            flag = 0;
        else if (rd_line[i] == ' ' && flag == 0)
        {
            compt++;
            flag = 3;
        }
        if (rd_line[i] == '|' && rd_line[i - 1] != ' ' && flag == 0)
            compt++;
        if (flag != 3)
            tab[compt] += 1;
        if (rd_line[i] == '|' && rd_line[i + 1] != ' ' && flag == 0)
            compt++;
        i++;
    }
    i = 0;
    while (i <= compt)
    {
        ft_printf("tab[%d] = %d\n", i, tab[i]);
        spt[i] = (char *) malloc(sizeof(char) * (tab[i] + 1));
        if (spt[i] == NULL)
            return (error(MALLOC, NULL), NULL);
        i++;
    }
    free(tab);
    i = 0;
    compt = 0;
    flag = 0;
    j = 0;
    while (rd_line[i])
    {
        if (rd_line[i] != ' ' && flag == 3)
            flag = 0;
        if (rd_line[i] == '"' && flag == 0)
            flag = 1;
        else if (rd_line[i] == '"' && flag == 1)
        {
            flag = 0;
            spt[compt][j] = '"';
            if (rd_line[i + 1] == ' ')
            {
                spt[compt][j + 1] = '\0';
                j = 0;
                i++;
                compt++;
            }
        }
        else if (rd_line[i] == '\'' && flag == 0)
            flag = 2;
        else if (rd_line[i] == '\'' && flag == 2)
        {
            flag = 0;
            spt[compt][j] = '\'';
            if (rd_line[i + 1] == ' ')
            {
                spt[compt][j + 1] = '\0';
                j = 0;
                i++;
                compt++;
            }
        }
        else if (rd_line[i] == ' ' && flag == 0)
        {
            spt[compt][j] = '\0';
            j = 0;
            compt++;
        }
        else if (rd_line[i] == '|' && flag == 0 \
        && (rd_line[i - 1] != ' ' || \
        (rd_line[i - 1] != ' ' && rd_line[i + 1] != ' ')))
        {
            spt[compt][j] = '\0';
            j = 0;
            compt++;
            spt[compt][j] = '|';
            j++;
        }
        else if (rd_line[i] == '|' && flag == 0 && rd_line[i - 1] == ' ')
        {
            spt[compt][j] = '|';
            j++;
        }
        if (rd_line[i] == '|' && flag == 0 && rd_line[i + 1] != ' ')
        {
            spt[compt][j] = '\0';
            j = 0;
            compt++;
        }
        if (rd_line[i] == ' ' && flag == 0)
            flag = 3;
        if (flag != 3 || (rd_line[i] != '|' && flag != 0))
        {
            ft_printf("compt = %d, j = %d\n", compt, j);
            spt[compt][j] = rd_line[i];
            j++;
        }
        i++;
    }
    spt[compt][j] = '\0';
    return (spt);
}