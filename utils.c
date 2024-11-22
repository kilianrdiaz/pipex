/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 17:53:47 by kroyo-di          #+#    #+#             */
/*   Updated: 2024/10/20 18:49:56 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex.h"

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
