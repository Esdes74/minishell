#include "minishell.h"

static char *determine_echo_or_cd(char **str, int *option);
static char *return_buf(char **spt);

int search_builtins(char **spt, char **envi, t_cmd *pip, int i)
{
    int     option;

    option = 0;
    if (ft_strlen(spt[0]) == 3 && ft_strncmp(spt[0], "pwd", 3) == 0)
        return (pwd(pip, i), -1);
    else if (ft_strlen(spt[0]) == 4 && ft_strncmp(spt[0], "exit", 4) == 0)
        return (exitt(), -1);
    else if (ft_strlen(spt[0]) == 3 && ft_strncmp(spt[0], "env", 3) == 0)
        return (env(envi), -1);
    else if ((ft_strlen(spt[0]) == 4 && ft_strncmp(spt[0], "echo", 4) == 0) \
    || (ft_strlen(spt[0]) == 2 && ft_strncmp(spt[0], "cd", 2) == 0))
    {
        determine_echo_or_cd(spt, &option);
            // return (error(MALLOC, NULL), 1);
        if (option == 0 || option == 1)
            return (echo(spt, option), -1);
        else if (option == 2)
            return (cd(spt[1]), -1);
    }
    return (1);
}


static char     *determine_echo_or_cd(char **str, int *option)
{
    if (ft_strlen(str[0]) == 4 && ft_strncmp(str[0], "echo", 4) == 0)
    {
        if (str[1] == NULL)
            return (return_buf(str));
        else if (ft_strlen(str[1]) == 2 && ft_strncmp(str[1], "-n", 2) == 0)
        {
            *option = 1;
            if (str[2] == NULL)
                return (return_buf(str));
        }
    }
    else if (ft_strlen(str[0]) == 2 && ft_strncmp(str[0], "cd", 2) == 0)
    {
        *option = 1;
        if (str[1] == NULL)
            return (return_buf(str));
        *option = 2;
        if (str[2] != NULL) // TODO: Bulle a résoudre
            return (error(TOO_MANY_ARG, str[0]), return_buf(str));
    }
    else
        *option = 3;
    return (NULL);
}

static char *return_buf(char **spt)
{
    char *buf;

    buf = ft_calloc(1, 1);
    if (buf == NULL)
        return (anihilation(spt), error(MALLOC, NULL), NULL);
    return (buf);
}
