/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 15:16:22 by eslamber          #+#    #+#             */
/*   Updated: 2023/10/26 11:51:01 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

//  - 1 : prob au niveau des espaces, le bash de base semble fonctionner lorsque l'on sépare pas les commandes c'est à dire que ls|grep minishell, fonctionne!
// par contre si je fais "ls|grep minishell", il ,ne fonctionne pas, les guillemets veut dire : "l'intérieur est une commande"

//  - 2 : notre minishell actuel ne fonctionne pas lorsque l'on met une commande entre "", tout simplement parce qu'il prend en compte comme char les guillemets
// Attention ! le vrai bash ne fontionne pas non plus si l'on met les commandes entre guillemets si l'on ajoute une option comme "ls -a" 
// donc guillemets veut dire "commande", si l'on rajouter quoi que ce soit qui diffère d'une commande comme un espace entre les guillemts, celle-ci ne fonctionnera pas

//  - 3 : ca me parait bizarre, mais apperement si je tape ça dans le bash  : ls -a|grep minishell|                 wc -l
// et bien il effectue bien la commande, même si les pipes sont collé (à verifier)

//  - 4 : si dans le cas ou les pipes peuvent etre collé

/*
This function checks whether the parameter sent by the user is actually the one he should send in terms of syntax.
*/

static void	init_parsing(t_pars *struc, int *flag);
static void	first_while(t_pars *p, int *flag, const char *rd_line);
static void	after_first_while(t_pars *p, int *flag, t_list *ret);
static void	second_while(t_pars *p, int *flag, const char *rd_line);
static void	after_second_while(t_pars *p, int *flag, t_list *ret);
static void	third_while(t_pars *p, int *flag, const char *rd_line);
static void	end_of_parsing(t_pars *p, int *flag, t_list *ret);

void	parsing(const char *rd_line, int *flag, t_list *ret)
{
	t_pars	struc;
	
	init_parsing(&struc, flag);
	first_while(&struc, flag, rd_line);
	after_first_while(&struc, flag, ret);
	second_while(&struc, flag, rd_line);
	after_second_while(&struc, flag, ret);
	third_while(&struc, flag, rd_line);
	end_of_parsing(&struc, flag, ret);
}

static void	init_parsing(t_pars *struc, int *flag)
{
	struc->i = 0;
	struc->compt = 1;
	struc->save_flag = *flag;
	struc->spt = 0;
	struc->new_flag = 0;
	struc->tmp_flag = 0;
}

static void	first_while(t_pars *p, int *flag, const char *rd_line)
{
	while (rd_line[p->i]) // Compte le nombre d'aguments qu'il y a (les | sont comptés comme des arguments a part entière)
	{
		while(rd_line[p->i] == '|' && (*flag == 0 || *flag == 3 || *flag == 4))
		{
			if (p->i > 0 && rd_line[p->i - 1] != ' ')
				p->compt++;
			*flag = 0;
			p->i++;
		}
		if(rd_line[p->i] == '\0')
		{
			if (p->i > 0 && rd_line[p->i - 1] != '|')
				*flag = 3;
			break;
		}
		if (p->i > 0 && rd_line[p->i] != ' ' && rd_line[p->i - 1] == '|' && (*flag == 0 || *flag == 3))
			p->compt++;
		if (rd_line[p->i] == '\n' && *flag == 0)
		{
			p->compt++;
			break;
		}
		if ((rd_line[p->i] != ' ' && *flag == 3) || (p->i > 0 && rd_line[p->i] == ' ' && rd_line[p->i - 1] == '|' && *flag != 1 && *flag != 2))
			*flag = 0;
		if (p->tmp_flag == 0 && rd_line[p->i] != ' ')
			p->tmp_flag = 1;
		if (*flag == 4 && rd_line[p->i] == '\n')
		{
			p->compt++;
			break;
		}
		if (*flag == 4 && rd_line[p->i] == ' ')
			*flag = 0;
		if ((rd_line[p->i] == '<' || rd_line[p->i] == '>') && *flag == 0)
			*flag = 4;
		else if (ft_is_redirection(rd_line, p->i) && *flag == 4)
			p->compt++;
		else if (rd_line[p->i] == '"' && (*flag == 0 || *flag == 4))
			*flag = 1;
		else if (rd_line[p->i] == '"' && *flag == 1)
		{
			if (rd_line[p->i + 1] != '\0' && rd_line[p->i + 1] != ' ' && p->new_flag == 1)
				p->compt++;
			p->new_flag = 0;
			*flag = 0;
		}
		else if (rd_line[p->i] == '\'' && *flag == 0)
			*flag = 2;
		else if (rd_line[p->i] == '\'' && *flag == 2)
		{
			if (rd_line[p->i + 1] != '\0' && rd_line[p->i + 1] != ' ' && p->new_flag == 1)
				p->compt++;
			p->new_flag = 0;
			*flag = 0;
		}
		else if (rd_line[p->i] == '\n' && (*flag == 1 || *flag == 2))
			p->new_flag = 1;
		else if (p->i > 0 && rd_line[p->i] == ' ' && *flag == 0 && p->tmp_flag == 1)
			p->compt++;
		if (rd_line[p->i] == ' ' && *flag == 0)
			*flag = 3;
		p->i++;
	}
}

void	after_first_while(t_pars *p, int *flag, t_list *ret)
{
	if (*flag == 3 && p->compt != 1)
		p->compt--;
	p->spt = (char **) malloc(sizeof(char *) * (p->compt + 1));
	if (p->spt == NULL)
		return (error(MALLOC, NULL), annihilation(ret, free, DEBUG));
	p->spt[p->compt] = NULL;
	p->tab = (int *) ft_calloc(p->compt, sizeof(int));
	if (p->tab == NULL)
		return (free(p->spt), error(MALLOC, NULL), annihilation(ret, free, DEBUG));
	p->i = 0;
	p->compt = 0;
	*flag = p->save_flag;
	p->new_flag = 0;
	p->tmp_flag = 0;
}

static void	second_while(t_pars *p, int *flag, const char *rd_line)
{
		while (rd_line[p->i]) // compte le nombre de caractère par arguments
	{
		while (rd_line[p->i] == '|' && (*flag == 0 || *flag == 3 ||*flag == 4))
		{
			if (p->i > 0 && rd_line[p->i - 1] != ' ')
				p->compt++;
			*flag = 0;
			p->tab[p->compt] += 1;
			p->i++;
		}
		if (rd_line[p->i] == '\0')
		{
			if (p->i > 0 && rd_line[p->i - 1] != '|')
				*flag = 3;
			break;
		}
		if (p->i > 0 && rd_line[p->i] != ' ' && rd_line[p->i - 1] == '|' && (*flag == 0 || *flag == 3))
			p->compt++;
		if (rd_line[p->i] == '\n' && (*flag == 0 || *flag == 3))
		{
			*flag = 5;
			p->tab[p->compt] += 1;
			p->compt++;
			p->tab[p->compt] -= 1;
		}
		if (p->tmp_flag == 0 && rd_line[p->i] != ' ')
			p->tmp_flag = 1;
		if (rd_line[p->i] != ' ' && *flag == 3)
			*flag = 0;
		if (*flag == 4 && rd_line[p->i] == '\n') // si pas d'espace exemple : cat <<test\n il reste en flag 4 et aucun prob, si espace avant test ou après test passe en 5 et segfault
		{
			p->tab[p->compt] += 1;
			p->compt++;
			p->tab[p->compt] -= 1;
			*flag = 5;
		}
		if (*flag == 4 && rd_line[p->i] == ' ')
			*flag = 0;
		if ((rd_line[p->i] == '<' || rd_line[p->i] == '>') && *flag == 0)
			*flag = 4;
		else if (ft_is_redirection(rd_line, p->i) && *flag == 4)
		{
			p->compt++;
			p->tab[p->compt] += 1;
			if (rd_line[p->i + 1] == '>' || rd_line[p->i + 1] == '<')
				p->tab[p->compt] += 1;
		}
		else if (rd_line[p->i] == '"' && (*flag == 0 || *flag == 4))
			*flag = 1;
		else if (rd_line[p->i] == '"' && *flag == 1)
		{
			if (rd_line[p->i + 1] != '\0' && rd_line[p->i + 1] != ' ' && p->new_flag == 1)
				p->compt++;
			p->new_flag = 0;
			*flag = 0;
		}
		else if (rd_line[p->i] == '\'' && *flag == 0)
			*flag = 2;
		else if (rd_line[p->i] == '\'' && *flag == 2)
		{
			if (rd_line[p->i + 1] != '\0' && rd_line[p->i + 1] != ' ' && p->new_flag == 1)
				p->compt++;
			p->new_flag = 0;
			*flag = 0;
		}
		else if (rd_line[p->i] == '\n' && (*flag == 1 || *flag == 2))
			p->new_flag = 1;
		else if (rd_line[p->i] == ' ' && *flag == 0 && p->tmp_flag == 1) // i > 0
		{
			p->compt++;
			*flag = 3;
		}
		if (*flag != 3)
			p->tab[p->compt] += 1;
		p->i++;
	}
}

static void	after_second_while(t_pars *p, int *flag, t_list *ret)
{
	p->i = 0;
	if (*flag == 3 && p->compt != 0)
		p->compt--;
	if (*flag != 0 && *flag == p->save_flag) // permet de compter le \n si jamais on doit en rajouter un a la fin de la chaine pasque l'argument n'est pas finis
		p->tab[p->compt] += 1;
	while (p->i <= p->compt) // créer les chaines de caractères
	{
		p->spt[p->i] = (char *)ft_calloc(sizeof(char) , (p->tab[p->i] + 1));
		if (p->spt[p->i] == NULL)
		{
			while (p->i >= 0)
				free(p->spt[p->i--]);
			return (free(p->tab), free(p->spt), error(MALLOC, NULL), annihilation(ret, free, DEBUG));
		}
		p->i++;
	}
	free(p->tab);
	p->i = 0;
	p->compt = 0;
	*flag = p->save_flag;
	p->j = 0;
	p->tmp_flag = 0;
}

static void	third_while(t_pars *p, int *flag, const char *rd_line)
{
	while (rd_line[p->i]) // remplis les chaines de caractères
	{
		if (p->i > 0 && rd_line[p->i] == '|' && rd_line[p->i - 1] != ' ' && (*flag == 0 || *flag == 3 || *flag == 4))
			p->spt[p->compt++][p->j] = '\0';
		while(rd_line[p->i] == '|' && (*flag == 0 || *flag == 3 || *flag == 4))
		{
			// ft_printf_fd(2, "compt = %d\n", compt);
			p->spt[p->compt][0] = '|';
			p->spt[p->compt][1] = '\0';
			p->j = 0;
			p->i++;
			if (rd_line[p->i] != '\0')
				p->compt++;
		}
		if (rd_line[p->i] == '\0')
			break ;
		if (rd_line[p->i] == '\n' && (*flag == 0 || *flag == 3))
		{
			p->tmp_flag = -1;
			*flag = 5;
			p->spt[p->compt][p->j] = '\n';
			p->spt[p->compt][p->j + 1] = '\0';
			if (p->i > 0 && rd_line[p->i - 1] != ' ')
				p->compt++;
			p->j = 0;
			p->i++;
		}
		if ((rd_line[p->i] == '<' || rd_line[p->i] == '>') && p->tmp_flag == 0 && *flag != 5)
			p->tmp_flag = 1;
		else if (ft_is_redirection(rd_line, p->i) && p->tmp_flag == 1)
		{
			if (p->i > 0 && rd_line[p->i - 1] != ' ')
				p->spt[p->compt++][p->j] = '\0';
			p->j = 0;
			p->spt[p->compt][p->j++] = rd_line[p->i++];
			if (rd_line[p->i] == '>' || rd_line[p->i] == '<')
				p->spt[p->compt][p->j++] = rd_line[p->i++];
		}
		else if (p->tmp_flag == 1 && (rd_line[p->i] == ' '))
			p->tmp_flag = 0;
		else if (rd_line[p->i] == '\n' && p->tmp_flag == 1 && *flag != 5)
		{
			p->spt[p->compt][p->j + 1] = '\0';
			p->j = 0;
			p->i++;
			p->compt++;
			p->tmp_flag = 0;
		}
		if (rd_line[p->i] != ' ' && *flag == 3)
			*flag = 0;
		if (rd_line[p->i] == '"' && *flag == 0)
			*flag = 1;
		else if (rd_line[p->i] == '"' && *flag == 1)
		{
			*flag = 0;
			p->spt[p->compt][p->j] = '"';
			if (rd_line[p->i + 1] == ' ' || (rd_line[p->i + 1] != '\0' && rd_line[p->i + 1] != ' ' && p->new_flag == 1))
			{
				p->spt[p->compt][p->j + 1] = '\0';
				p->j = 0;
				p->i++;
				p->compt++;
			}
			p->new_flag = 0;
		}
		else if (rd_line[p->i] == '\'' && *flag == 0)
			*flag = 2;
		else if (rd_line[p->i] == '\'' && *flag == 2)
		{
			*flag = 0;
			p->spt[p->compt][p->j] = '\'';
			if (rd_line[p->i + 1] == ' ' || (rd_line[p->i + 1] != '\0' && rd_line[p->i + 1] != ' ' && p->new_flag == 1))
			{
				p->spt[p->compt][p->j + 1] = '\0';
				p->j = 0;
				p->i++;
				p->compt++;
			}
			p->new_flag = 0;
		}
		else if (p->i > 0 && rd_line[p->i] == ' ' && rd_line[p->i - 1] != '|' && *flag == 0 && p->i > 0)
		{
			p->spt[p->compt][p->j] = '\0';
			p->j = 0;
			p->compt++;
		}
		else if (rd_line[p->i] == '\n' && (*flag == 1 || *flag == 2))
			p->new_flag = 1;
		if (rd_line[p->i] == ' ' && *flag == 0)
			*flag = 3;
		if ((*flag != 3 && rd_line[p->i] != '|') || (*flag != 0 && rd_line[p->i] == '|') || *flag == 5)
		{
			p->spt[p->compt][p->j] = rd_line[p->i];
			p->j++;
		}
		p->i++;
	}
}

static void	end_of_parsing(t_pars *p, int *flag, t_list *ret)
{
	if (*flag != 3)
	{
		if (*flag != 0 && *flag == p->save_flag) // Rajoute le \n si c'est un rappel de la fonction et qu'on est toujours pas sortie
			p->spt[p->compt][p->j++] = '\n';
		p->spt[p->compt][p->j] = '\0';
	}
	p->i = 0;
	while (p->spt[p->i] && p->spt[p->i][0] != '\0') // ajoute les arguments a la liste 
	{
		if (tailing_list(ret, p->spt[p->i++], STRING, DEBUG) == 0)
		{
			anihilation(p->spt);
			error(MALLOC, NULL);
		}
	}
	if (p->j == 0 && p->spt[p->compt] == NULL)
		free(p->spt[p->compt]);
	if (p->i != p->compt)
		free(p->spt[p->i]);
	free(p->spt);
}

// void	parsing(const char *rd_line, int *flag, t_list *ret)
// {
	// int     i;
	// int     j;
	// int     compt;
	// int     tmp_flag;
	// int     new_flag;
	// int     save_flag;
	// int     *tab;
	// char    **spt;

	// i = 0;
	// compt = 1;
	// save_flag = *flag;
	// spt = 0;
	// new_flag = 0;
	// tmp_flag = 0;
	// while (rd_line[i]) // Compte le nombre d'aguments qu'il y a (les | sont comptés comme des arguments a part entière)
	// {
	// 	while(rd_line[i] == '|' && (*flag == 0 || *flag == 3 || *flag == 4))
	// 	{
	// 		if (i > 0 && rd_line[i - 1] != ' ')
	// 			compt++;
	// 		*flag = 0;
	// 		i++;
	// 	}
	// 	if(rd_line[i] == '\0')
	// 	{
	// 		if (i > 0 && rd_line[i - 1] != '|')
	// 			*flag = 3;
	// 		break;
	// 	}
	// 	if (i > 0 && rd_line[i] != ' ' && rd_line[i - 1] == '|' && (*flag == 0 || *flag == 3))
	// 		compt++;
	// 	if (rd_line[i] == '\n' && *flag == 0)
	// 	{
	// 		compt++;
	// 		break;
	// 	}
	// 	if ((rd_line[i] != ' ' && *flag == 3) || (i > 0 && rd_line[i] == ' ' && rd_line[i - 1] == '|' && *flag != 1 && *flag != 2))
	// 		*flag = 0;
	// 	if (tmp_flag == 0 && rd_line[i] != ' ')
	// 		tmp_flag = 1;
	// 	if (*flag == 4 && rd_line[i] == '\n')
	// 	{
	// 		compt++;
	// 		break;
	// 	}
	// 	if (*flag == 4 && rd_line[i] == ' ')
	// 		*flag = 0;
	// 	if ((rd_line[i] == '<' || rd_line[i] == '>') && *flag == 0)
	// 		*flag = 4;
	// 	else if (ft_is_redirection(rd_line, i) && *flag == 4)
	// 		compt++;
	// 	else if (rd_line[i] == '"' && (*flag == 0 || *flag == 4))
	// 		*flag = 1;
	// 	else if (rd_line[i] == '"' && *flag == 1)
	// 	{
	// 		if (rd_line[i + 1] != '\0' && rd_line[i + 1] != ' ' && new_flag == 1)
	// 			compt++;
	// 		new_flag = 0;
	// 		*flag = 0;
	// 	}
	// 	else if (rd_line[i] == '\'' && *flag == 0)
	// 		*flag = 2;
	// 	else if (rd_line[i] == '\'' && *flag == 2)
	// 	{
	// 		if (rd_line[i + 1] != '\0' && rd_line[i + 1] != ' ' && new_flag == 1)
	// 			compt++;
	// 		new_flag = 0;
	// 		*flag = 0;
	// 	}
	// 	else if (rd_line[i] == '\n' && (*flag == 1 || *flag == 2))
	// 		new_flag = 1;
	// 	else if (i > 0 && rd_line[i] == ' ' && *flag == 0 && tmp_flag == 1)
	// 		compt++;
	// 	if (rd_line[i] == ' ' && *flag == 0)
	// 		*flag = 3;
	// 	i++;
	// }
	// Créer le tableau de chaine de caractères et le tableau permettant de compter le nombre de caractères dans chaque arguments
	// if (*flag == 3 && compt != 1)
	// 		compt--;
	// // ft_printf_fd(2, "première boucle = %d, flag = %d\n", compt, *flag);
	// spt = (char **) malloc(sizeof(char *) * (compt + 1));
	// if (spt == NULL)
	// 	return (error(MALLOC, NULL), annihilation(ret, free, DEBUG));
	// spt[compt] = NULL;
	// tab = (int *) ft_calloc(compt, sizeof(int));
	// if (tab == NULL)
	// 	return (free(spt), error(MALLOC, NULL), annihilation(ret, free, DEBUG));
	// i = 0;
	// compt = 0;
	// *flag = save_flag;
	// new_flag = 0;
	// tmp_flag = 0;
	// while (rd_line[i]) // compte le nombre de caractère par arguments
	// {
	// 	while (rd_line[i] == '|' && (*flag == 0 || *flag == 3 ||*flag == 4))
	// 	{
	// 		if (i > 0 && rd_line[i - 1] != ' ')
	// 			compt++;
	// 		*flag = 0;
	// 		tab[compt] += 1;
	// 		i++;
	// 	}
	// 	if (rd_line[i] == '\0')
	// 	{
	// 		if (i > 0 && rd_line[i - 1] != '|')
	// 			*flag = 3;
	// 		break;
	// 	}
	// 	if (i > 0 && rd_line[i] != ' ' && rd_line[i - 1] == '|' && (*flag == 0 || *flag == 3))
	// 		compt++;
	// 	if (rd_line[i] == '\n' && (*flag == 0 || *flag == 3))
	// 	{
	// 		*flag = 5;
	// 		tab[compt] += 1;
	// 		compt++;
	// 		tab[compt] -= 1;
	// 	}
	// 	if (tmp_flag == 0 && rd_line[i] != ' ')
	// 		tmp_flag = 1;
	// 	if (rd_line[i] != ' ' && *flag == 3)
	// 		*flag = 0;
	// 	if (*flag == 4 && rd_line[i] == '\n') // si pas d'espace exemple : cat <<test\n il reste en flag 4 et aucun prob, si espace avant test ou après test passe en 5 et segfault
	// 	{
	// 		tab[compt] += 1;
	// 		compt++;
	// 		tab[compt] -= 1;
	// 		*flag = 5;
	// 	}
	// 	if (*flag == 4 && rd_line[i] == ' ')
	// 		*flag = 0;
	// 	if ((rd_line[i] == '<' || rd_line[i] == '>') && *flag == 0)
	// 		*flag = 4;
	// 	else if (ft_is_redirection(rd_line, i) && *flag == 4)
	// 	{
	// 		compt++;
	// 		tab[compt] += 1;
	// 		if (rd_line[i + 1] == '>' || rd_line[i + 1] == '<')
	// 			tab[compt] += 1;
	// 	}
	// 	else if (rd_line[i] == '"' && (*flag == 0 || *flag == 4))
	// 		*flag = 1;
	// 	else if (rd_line[i] == '"' && *flag == 1)
	// 	{
	// 		if (rd_line[i + 1] != '\0' && rd_line[i + 1] != ' ' && new_flag == 1)
	// 			compt++;
	// 		new_flag = 0;
	// 		*flag = 0;
	// 	}
	// 	else if (rd_line[i] == '\'' && *flag == 0)
	// 		*flag = 2;
	// 	else if (rd_line[i] == '\'' && *flag == 2)
	// 	{
	// 		if (rd_line[i + 1] != '\0' && rd_line[i + 1] != ' ' && new_flag == 1)
	// 			compt++;
	// 		new_flag = 0;
	// 		*flag = 0;
	// 	}
	// 	else if (rd_line[i] == '\n' && (*flag == 1 || *flag == 2))
	// 		new_flag = 1;
	// 	else if (rd_line[i] == ' ' && *flag == 0 && tmp_flag == 1) // i > 0
	// 	{
	// 		compt++;
	// 		*flag = 3;
	// 	}
	// 	if (*flag != 3)
	// 		tab[compt] += 1;
	// 	i++;
	// }
	// i = 0;
	// if (*flag == 3 && compt != 0)
	// 	compt--;
	// // ft_printf_fd(2, "seconde boucle = %d\n", compt);
	// if (*flag != 0 && *flag == save_flag) // permet de compter le \n si jamais on doit en rajouter un a la fin de la chaine pasque l'argument n'est pas finis
	// 	tab[compt] += 1;
	// while (i <= compt) // créer les chaines de caractères
	// {
	// 	// ft_printf_fd(2, "tab[%d] = %d\n", i, tab[i]);
	// 	spt[i] = (char *)ft_calloc(sizeof(char) , (tab[i] + 1));
	// 	if (spt[i] == NULL)
	// 	{
	// 		while (i >= 0)
	// 			free(spt[i--]);
	// 		return (free(tab), free(spt), error(MALLOC, NULL), annihilation(ret, free, DEBUG));
	// 	}
	// 	i++;
		// ft_printf_fd(2, "tab[%d] = %d\n", i, tab[i]);
	// }
	// free(tab);
	// i = 0;
	// compt = 0;
	// *flag = save_flag;
	// j = 0;
	// tmp_flag = 0;
	// while (rd_line[i]) // remplis les chaines de caractères
	// {
	// 	if (i > 0 && rd_line[i] == '|' && rd_line[i - 1] != ' ' && (*flag == 0 || *flag == 3 || *flag == 4))
	// 		spt[compt++][j] = '\0';
	// 	while(rd_line[i] == '|' && (*flag == 0 || *flag == 3 || *flag == 4))
	// 	{
	// 		// ft_printf_fd(2, "compt = %d\n", compt);
	// 		spt[compt][0] = '|';
	// 		spt[compt][1] = '\0';
	// 		j = 0;
	// 		i++;
	// 		if (rd_line[i] != '\0')
	// 			compt++;
	// 	}
	// 	if (rd_line[i] == '\0')
	// 		break ;
	// 	if (rd_line[i] == '\n' && (*flag == 0 || *flag == 3))
	// 	{
	// 		tmp_flag = -1;
	// 		*flag = 5;
		// ft_printf_fd(2, "tab[%d] = %d\n", i, tab[i]);
	// 		spt[compt][j] = '\n';
	// 		spt[compt][j + 1] = '\0';
	// 		if (i > 0 && rd_line[i - 1] != ' ')
	// 			compt++;
	// 		j = 0;
	// 		i++;
	// 	}
	// 	if ((rd_line[i] == '<' || rd_line[i] == '>') && tmp_flag == 0 && *flag != 5)
	// 		tmp_flag = 1;
	// 	else if (ft_is_redirection(rd_line, i) && tmp_flag == 1)
	// 	{
	// 		if (i > 0 && rd_line[i - 1] != ' ')
	// 			spt[compt++][j] = '\0';
	// 		j = 0;
	// 		spt[compt][j++] = rd_line[i++];
	// 		if (rd_line[i] == '>' || rd_line[i] == '<')
	// 			spt[compt][j++] = rd_line[i++];
	// 	}
	// 	else if (tmp_flag == 1 && (rd_line[i] == ' '))
	// 		tmp_flag = 0;
	// 	else if (rd_line[i] == '\n' && tmp_flag == 1 && *flag != 5)
		// ft_printf_fd(2, "tab[%d] = %d\n", i, tab[i]);
	// 	{
	// 		spt[compt][j + 1] = '\0';
	// 		j = 0;
	// 		i++;
	// 		compt++;
	// 		tmp_flag = 0;
	// 	}
	// 	if (rd_line[i] != ' ' && *flag == 3)
	// 		*flag = 0;
	// 	if (rd_line[i] == '"' && *flag == 0)
	// 		*flag = 1;
	// 	else if (rd_line[i] == '"' && *flag == 1)
	// 	{
	// 		*flag = 0;
	// 		spt[compt][j] = '"';
	// 		if (rd_line[i + 1] == ' ' || (rd_line[i + 1] != '\0' && rd_line[i + 1] != ' ' && new_flag == 1))
	// 		{
	// 			spt[compt][j + 1] = '\0';
	// 			j = 0;
	// 			i++;
	// 			compt++;
	// 		}
	// 		new_flag = 0;
	// 	}
	// 	else if (rd_line[i] == '\'' && *flag == 0)
	// 		*flag = 2;
	// 	else if (rd_line[i] == '\'' && *flag == 2)
	// 	{
	// 		*flag = 0;
	// 		spt[compt][j] = '\'';
	// 		if (rd_line[i + 1] == ' ' || (rd_line[i + 1] != '\0' && rd_line[i + 1] != ' ' && new_flag == 1))
	// 		{
	// 			spt[compt][j + 1] = '\0';
	// 			j = 0;
	// 			i++;
	// 			compt++;
	// 		}
	// 		new_flag = 0;
	// 	}
	// 	else if (i > 0 && rd_line[i] == ' ' && rd_line[i - 1] != '|' && *flag == 0 && i > 0)
	// 	{
	// 		spt[compt][j] = '\0';
	// 		j = 0;
	// 		compt++;
	// 	}
	// 	else if (rd_line[i] == '\n' && (*flag == 1 || *flag == 2))
	// 		new_flag = 1;
	// 	if (rd_line[i] == ' ' && *flag == 0)
	// 		*flag = 3;
	// 	if ((*flag != 3 && rd_line[i] != '|') || (*flag != 0 && rd_line[i] == '|') || *flag == 5)
	// 	{
	// 		spt[compt][j] = rd_line[i];
	// 		j++;
	// 	}
	// 	i++;
	// }

	// if (*flag != 3)
	// {
	// 	if (*flag != 0 && *flag == save_flag) // Rajoute le \n si c'est un rappel de la fonction et qu'on est toujours pas sortie
	// 		spt[compt][j++] = '\n';
	// 	spt[compt][j] = '\0';
	// }
	// i = 0;
	// while (spt[i] && spt[i][0] != '\0') // ajoute les arguments a la liste 
	// {
	// 	if (tailing_list(ret, spt[i++], STRING, DEBUG) == 0)
	// 	{
	// 		anihilation(spt);
	// 		error(MALLOC, NULL);
	// 	}
	// }
	// if (j == 0 && spt[compt] == NULL)
	// 	free(spt[compt]);
	// if (i != compt)
	// 	free(spt[i]);
	// free(spt);
// }
