#include "minishell.h"


char *verif_read(char *rd_line, char **env)
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
    second_parsing_check(rd_line, &flag, spt);
    buff = rd_line;
    while (flag == 1 || flag == 2)
    {
        rd_line = readline("> ");
        second_parsing_check(rd_line, &flag, spt);
        buf = ft_strjoin(buff, "\n");
        free(buff);
        buff = ft_strjoin(buf, rd_line);
        free(buf);
    }
    if (execution_center(spt, env) == 1)
    {
        return (NULL);
    }
	annihilation(spt, free, DEBUG);
    return (0);
}
