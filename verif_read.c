#include "minishell.h"

static int  check_redirection(char **arg);

char *verif_read(char *rd_line, char **env, t_cmd *pip)
{
    int     flag;
    char    *buff;
    char    *buf;
    t_list  *spt;

    flag = 0;
    spt = (t_list *) malloc(sizeof(t_list));
	if (spt == NULL)
		return (error(MALLOC, NULL), NULL);
    init_list(spt);
    parsing(rd_line, &flag, spt);
    if (spt->len == 0)
        return (rd_line);
    buff = rd_line;
    while (flag == 1 || flag == 2)
    {
        rd_line = readline("> ");
        parsing(rd_line, &flag, spt);
        buf = ft_strjoin(buff, "\n");
        free(buff);
        buff = ft_strjoin(buf, rd_line);
        free(buf);
    }
    if (execution_center(spt, env, pip) == 1)
    {
        return (NULL);
    }
    wait(NULL);
	annihilation(spt, free, DEBUG);
    return (buff);
}

static int  check_redirection(char **arg)
{
    char    **tmp;
    int     file;
    int     flag;
    int     compt;
    int     i;
    int     j;

    i = 0;
    flag = 0;
    while (arg[i]) // récupération des différentes redirection
    {
        if (arg[i][0] == '<')
        {
            flag += 1;
            if (arg[i][1] == '\0')
            {
                if (arg[i + 1] == NULL)
                    return (error(TOKEN, "0"), 1);
                file = open(arg[i + 1], O_RDONLY);
                if (file == -1)
                    return (error(OPEN, "0"), 1);
                else if (dup2(file, STDIN_FILENO) == -1)
                    return (close(file), error(DUP, "0"), 1);
                close(file);
            }
        }
        else if (arg[i][0] == '>')
        {
            flag += 2;
            if (arg[i][1] == '\0')
            {
                if (arg[i + 1] == NULL)
                    return (error(TOKEN, "0"), 1);
                file = open(arg[i + 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
                if (file == -1)
                    return (error(OPEN, "0"), 1);
                else if (dup2(file, STDOUT_FILENO) == -1)
                    return (close(file), error(DUP, "0"), 1);
                close(file);
            }
            else if (arg[i][1] == '>')
            {
                if (arg[i][2] == '\0')
                {
                    if (arg[i + 1] == NULL)
                        return (error(TOKEN, "0"), 1);
                    file = open(arg[i + 1], O_CREAT | O_RDWR | O_APPEND, 0644);
                    if (file == -1)
                        return (error(OPEN, "0"), 1);
                    else if (dup2(file, STDOUT_FILENO) == -1)
                        return (close(file), error(DUP, "0"), 1);
                    close(file);
                }
            }
        }
        i++;
    }
    if (flag == 0) // S'il n'y a pas de redirections pas besoin de retravailler le tableau de chaine de caractères
        return (flag);
    i = 0;
    compt = 0;
    while (arg[i]) // compte le nombre de redirection pour refaire un tableau de chaines de caractères
    {
        if (arg[i][0] == '<')
        {
            compt--;
            if (arg[i][1] == '\0' || (arg[i][1] == '<' && arg[i][2] == '\0'))
                i += 1
        }
        else if (arg[i][0] == '>')
        {
            compt--;
            if (arg[i][1] == '\0' || (arg[i][1] == '>' && arg[i][2] == '\0'))
                i += 1
        }
        i++;
        compt++;
    }
    tmp = (char **) malloc(sizeof(char *) compt);
    i = 0;
    j = 0;
    while (i < compt) // ici on ajoute les chaines de caractère correctes a tmp, on free les redirections car elles ne servent plus et on remet tmp dans arg a la fin
    {
        if (arg[i][0] == '<')
        {
            if (arg[i][1] == '\0' || (arg[i][1] == '<' && arg[i][2] == '\0'))
            {
                free(arg[i + 1])
                i++;
            }
            free(arg[i]);
        }
        else if (arg[i][0] == '>')
        {
            if (arg[i][1] == '\0' || (arg[i][1] == '>' && arg[i][2] == '\0'))
            {
                free(arg[i + 1]);
                i++;
            }
            free(arg[i]);
        }
        else
            tmp[j] = arg[i];
        i++;
        j++;
    }
    free(arg);
    arg = tmp;
    return (flag);
}
