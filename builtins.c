#include "minishell.h"

char    *pwd(void)
{
    return (getcwd(NULL, 0));
}

void    exitt(void)
{
    quit();
}

void    env(char **envir)
{
    int i;

    i = 0;
    while (envir[i])
        ft_printf(envir[i++]);
}

void    echo(char *arg, int option)
{
    ft_printf(arg);
    if (option == 0)
        ft_printf("\n");
    free(arg);
}

void    cd(char *path)
{
    if (path != NULL && chdir(path) != 0)
        ft_printf("-bash: cd: %s: No such file or directory\n");
}
