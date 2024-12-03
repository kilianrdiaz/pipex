/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 17:53:47 by kroyo-di          #+#    #+#             */
/*   Updated: 2024/11/28 20:18:14 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex_bonus.h"

void	ft_free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i] != NULL)
		free(tab[i++]);
	free (tab);
	return ;
}

char	*ft_getenv(char *name, char **envp)
{
	int	i;
	int	j;

	i = 0;
	while (envp[i] != NULL)
	{
		j = 0;
		while (envp[i][j] && envp[i][j] != '=')
			j++;
		if (ft_strncmp(envp[i], name, j) == 0 && name[j] == '\0')
			return (envp[i] + j + 1);
		i++;
	}
	return (NULL);
}

char	*get_path(char *cmd, char **envp)
{
	char	**full_path;
	char	**s_cmd;
	char	*path;
	char	*exec;
	int		i;

	full_path = ft_split(ft_getenv("PATH", envp), ':');
	s_cmd = ft_split(cmd, ' ');
	i = 0;
	while (full_path[i] != NULL)
	{
		path = ft_strjoin(full_path[i], "/");
		exec = ft_strjoin(path, s_cmd[0]);
		free(path);
		if (access(exec, F_OK | X_OK) == 0)
		{
			ft_free_tab(s_cmd);
			ft_free_tab(full_path);
			return (exec);
		}
		free(exec);
		i++;
	}
	ft_free_tab(s_cmd);
	ft_free_tab(full_path);
	return (cmd);
}

void	error_handler(int error)
{
	if (error == 1)
		perror("Error ocurred while creating pipe.");
	else if (error == 2)
		perror("Error ocurred while creating first fork.");
	else if (error == 3)
		perror("Error ocurred while creating second fork.");
	else if (error == 4)
	{
		perror("Error ocurred while executing execve().");
		exit(127);
	}
	else if (error == 5)
		perror("Unable to open file.");
	else
		perror("Error.");
	exit(EXIT_FAILURE);
}

/*int	open_file(char **argv, int argc, int pos)
{
	int	fd;

	if (pos == 0)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd == -1)
			error_handler(5);
		dup2(fd, STDIN_FILENO);
		return (1);
	}
	else if(pos == 2)
	{
		if (ft_strcmp(argv[1], "here_doc") == 0) 
			fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0777);
		else
			fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (fd == -1)
			error_handler(5);
		dup2(fd, STDOUT_FILENO);
		return (2);
	}
	return (0);
}*/
