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
    while (envir[i] != 0)
        ft_printf(envir[i++]);
}

void    echo(char *arg, int option)
{
    int i;

    i = 0;
    if (option > 0)
        while (arg[i] != '\n' && arg[i] != '\0')
            ft_putchar_fd(arg[i], 1);
    else
        ft_printf(arg);
}

void    cd(char *path)
{
    if (path != NULL && chdir(path) != 0)
        ft_printf("-bash: cd: %s: No such file or directory\n");
}