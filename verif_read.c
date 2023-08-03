#include "minishell.h"

// static int  check_redirection(char **arg);

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

// static int  check_redirection(char **arg)
// {
//     int infile;

//     if (arg[0][0] == '<')
//     {
//         if (arg[0][1] == '\0')
//         {
//             infile = open(arg[1], O_RDONLY);
//             if (infile == -1)
//                 return (error(OPEN, "0"), 1)
//             if (dup2(infile, STDIN_FILENO) == -1)
//                 return (error(DUP, "0"), 1);
//         }
//     }
//     return (0);
// }
