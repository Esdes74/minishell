#include "minishell.h"

static int  check_heredoc(char **arg, t_cmd *struc);
static char *heredoc(char *arg, char *rd_line, t_cmd *struc);
static int  write_hd_to_pip(t_cmd *struc, char *rd_line);

char    **check_redirection(char **arg, t_list *spt, t_cmd *struc)
{
    char    **tmp;
    char    *rd_line;
    int     file;
    int     compt;
    int     i;
    int     j;
    (void)  spt;

    i = 0;
    rd_line = NULL;
    struc->in = FALSE;
    struc->out = FALSE;
    if (check_heredoc(arg, struc) == 1)
        return (NULL);
    while (arg[i]) // récupération des différentes redirection
    {
        if (arg[i][0] == '<')
        {
            struc->in = TRUE;
            if (arg[i][1] == '\0')
            {
                if (arg[i + 1] == NULL)
                    return (error(TOKEN, "0"), NULL);
                file = open(arg[i + 1], O_RDONLY);
                if (file == -1)
                    return (error(OPEN, "0"), NULL);
                else if (dup2(file, STDIN_FILENO) == -1)
                    return (close(file), error(DUP, "0"), NULL);
                close(file);
                i++;
            }
            else if (arg[i][1] == '<' && arg[i][2] == '\0')
            {
                rd_line = heredoc(arg[i + 1], rd_line, struc);
                if (rd_line == NULL)
                    return (NULL);
                i++;
            }
            else if (arg[i][1] == '<' && arg[i][2] != '\0')
            {
                rd_line = heredoc(&arg[i][2], rd_line, struc);
                if (rd_line == NULL)
                    return (NULL);
            }
            else
            {
                file = open(&arg[i][1], O_RDONLY);
                if (file == -1)
                    return (error(OPEN, "0"), NULL);
                else if (dup2(file, STDIN_FILENO) == -1)
                    return (close(file), error(DUP, "0"), NULL);
                close(file);
            }
        }
        else if (arg[i][0] == '>')
        {
            struc->out = TRUE;
            if (arg[i][1] == '\0')
            {
                if (arg[i + 1] == NULL)
                    return (error(TOKEN, "0"), NULL);
                file = open(arg[i + 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
                if (file == -1)
                    return (error(OPEN, "0"), NULL);
                else if (dup2(file, STDOUT_FILENO) == -1)
                    return (close(file), error(DUP, "0"), NULL);
                close(file);
                i++;
            }
            else if (arg[i][1] == '>')
            {
                if (arg[i][2] == '\0')
                {
                    if (arg[i + 1] == NULL)
                        return (error(TOKEN, "0"), NULL);
                    file = open(arg[i + 1], O_CREAT | O_RDWR | O_APPEND, 0644);
                    if (file == -1)
                        return (error(OPEN, "0"), NULL);
                    else if (dup2(file, STDOUT_FILENO) == -1)
                        return (close(file), error(DUP, "0"), NULL);
                    close(file);
                    i++;
                }
                else
                {
                    file = open(&arg[i][2], O_CREAT | O_RDWR | O_APPEND, 0644);
                    if (file == -1)
                        return (error(OPEN, "0"), NULL);
                    else if (dup2(file, STDOUT_FILENO) == -1)
                        return (close(file), error(DUP, "0"), NULL);
                    close(file);
                }
            }
            else
            {
                file = open(&arg[i][1], O_CREAT | O_RDWR | O_TRUNC, 0644);
                if (file == -1)
                    return (error(OPEN, "0"), NULL);
                else if (dup2(file, STDOUT_FILENO) == -1)
                    return (close(file), error(DUP, "0"), NULL);
                close(file);
            }
        }
        i++;
    }
    if (struc->in == FALSE && struc->out == FALSE) // S'il n'y a pas de redirections pas besoin de retravailler le tableau de chaine de caractères
        return (arg);
    if (struc->heredoc == 1)
        if (write_hd_to_pip(struc, rd_line) == 1)
            return (NULL);
    i = 0;
    compt = 0;
    while (arg[i]) // compte le nombre de redirection pour refaire un tableau de chaines de caractères
    {
        if (arg[i][0] == '<')
        {
            compt--;
            if (arg[i][1] == '\0' || (arg[i][1] == '<' && arg[i][2] == '\0'))
                i += 1;
        }
        else if (arg[i][0] == '>')
        {
            compt--;
            if (arg[i][1] == '\0' || (arg[i][1] == '>' && arg[i][2] == '\0'))
                i += 1;
        }
        i++;
        compt++;
    }
    tmp = (char **) malloc(sizeof(char *) * (compt + 1));
    i = 0;
    j = 0;
    while (arg[i]) // ici on ajoute les chaines de caractère correctes a tmp, on free les redirections car elles ne servent plus et on remet tmp dans arg a la fin
    {
        ft_printf_fd(2, "arg[%d] = %s\n", i, arg[i]);
        if (arg[i][0] == '<')
        {
            if (arg[i][1] == '\0' || (arg[i][1] == '<' && arg[i][2] == '\0'))
                i++;
        }
        else if (arg[i][0] == '>')
        {
            if (arg[i][1] == '\0' || (arg[i][1] == '>' && arg[i][2] == '\0'))
                i++;
        }
        else
            tmp[j++] = arg[i];
        i++;
    }
    free(arg);
    arg = tmp;
    arg[j] = NULL;
    return (arg);
}

// Cette fonction doit mettre en place le pipe du heredoc s'il  y en a besoin,
// pour le savoir elle check si la dernière redirection est un heredoc ou non
static int  check_heredoc(char **arg, t_cmd *struc)
{
    int i;

    i = 0;
    struc->heredoc = 0;
    while (arg[i]) // vérification que la dernière redirection est bien un heredoc car si c'est le cas il va falloire faire le pipe
    {
        if (arg[i][0] == '<')
        {
            if (arg[i][1] == '<')
                struc->heredoc = 1;
            else
                struc->heredoc = 0;
        }
        i++;
    }
    if (struc->heredoc == 0)
        return (0);
    struc->here_pipe = (int *) ft_calloc(2, sizeof(int));
    if (struc->here_pipe != NULL && pipe(struc->here_pipe) == -1) // création du pipe pour le heredoc
        return (error(PIPE, "0"), 1);
    else if (struc->here_pipe == NULL)
        return (error(CALLOC, "0"), 1);
    return (0);
}

static char *heredoc(char *arg, char *rd_line, t_cmd *struc)
{
    char    *tmp;
    char    *buf;
    char    *ret;

    tmp = NULL;
    buf = NULL;

    // Ici je fais en sorte que s'il y avait d'autres heredoc ils soient effacés
    if (rd_line != NULL)
    {
        free(rd_line);
        rd_line = NULL;
    }
    
    // Ici je demande a l'utilisateur de rentrer ses phrases jusqu'a ce que je retrouve le bon mot
    tmp = readline("> ");
    while (ft_strncmp(arg, tmp, ft_strlen(tmp) + 1) != 0)
    {
        if (rd_line == NULL) // Si c'est la première phrase alors je join un \n
        {
            rd_line = ft_strjoin(tmp, "\n");
            if (rd_line == NULL)
                return (error(JOIN, "0"), free(tmp), free(buf), NULL);
            free(tmp);
        }
        else // Sinon je join un \n et je join le nouveau a l'ancien
        {
            buf = ft_strjoin(tmp, "\n");
            if (buf == NULL)
                return (error(JOIN, "0"), free(tmp), free(rd_line), NULL);
            free(tmp);
            tmp = ft_strjoin(rd_line, buf);
            if (tmp == NULL)
                return (error(JOIN, "0"), free(rd_line), free(buf), NULL);
            free(rd_line);
            free(buf);
            rd_line = tmp;
        }
        tmp = readline("> ");
    }


    // Ici je fais tous un mic mac trop compliqué (a cause des gestions d'erreures)
    // Pour permettre de récupérer correctement les phrases dans l'historique ET de
    // Retourner le bon truc pour le heredoc
    ret = ft_strdup(rd_line); // Je duplique la valeur de retour pour ensuite pouvoir la manipuler
    if (ret == NULL) // Petite vérif
        return (error(STRDUP, "0"), NULL);
    buf = ft_strjoin(rd_line, tmp); // Je rajoute le dernier mot pour que l'hitorique soit plus propre
    if (buf == NULL) // Petite verif
        return (error(JOIN, "0"), NULL);
    free(tmp); // Je free tmp ici une fois que je l'ai ajouté au buf de l'historique
    free(rd_line); // Je free rd_line ici pour pouvoir remettre un join dedans sans leaks
    rd_line = ft_strjoin(buf, "\n"); // Je join un \n pour que l'historique soit plus beau
    if (rd_line == NULL) // Petite verif
        return (error(JOIN, "0"), NULL);
    free(buf); // Je free buf ici parcequ'il y en a plus besoin


    // Ici c'est le mic mac de l'hitorique
    if (struc->hd_history != NULL) // S'il existe
    {
        tmp = ft_strjoin(struc->hd_history, rd_line); // Alors je peux join
        if (tmp == NULL) // Petite verif
            return (error(JOIN, "0"), free(rd_line), free(struc->hd_history), NULL);
        free(rd_line); // Plus besoin
        free(struc->hd_history); // Je le free avant de mettre je nouvel hitorique
        struc->hd_history = tmp; // Remplacmeent de l'ancien historique
    }
    else // Sinon s'il n'existe pas
        struc->hd_history = rd_line; // Je met l'historique a sa place
    return (ret); // Je retourne la valeur qu'il faut retourner (le heredoc sans le mot d'arret)
}

static int  write_hd_to_pip(t_cmd *struc, char *rd_line)
{
    if (write(struc->here_pipe[1], rd_line, ft_strlen(rd_line)) == -1)
        return (error(WRITE, "0"), 1);
    free(rd_line);
    if (close(struc->here_pipe[1]) == -1)
        return (error(CLOSE, "0"), 1);
    if (dup2(struc->here_pipe[0], STDIN_FILENO) == -1)
        return (error(DUP, "0"), 1);
    if (close(struc->here_pipe[0]) == -1)
        return (error(CLOSE, "0"), 1);
    return (0);
}
