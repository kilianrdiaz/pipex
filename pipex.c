/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 15:23:38 by kroyo-di          #+#    #+#             */
/*   Updated: 2024/10/20 18:38:52 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex.h"

void	exec(char *cmd, char **envp)
{
	char	**s_cmd;
	char	*path;

	s_cmd = ft_split(cmd, ' ');
	path = get_path(s_cmd[0], envp);
	if (execve(path, s_cmd, envp) == -1)
	{
		perror ("Error ocurred at execve!");
		ft_free_tab(s_cmd);
		exit(EXIT_FAILURE);
	}
}

void	child(char **argv, int	*pipefd, char **envp)
{
	int	fd;

	fd = open(argv[1], O_RDONLY, 0777);
	dup2(fd, STDIN_FILENO);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[0]);
	exec(argv[2], envp);
}

void	parent(char **argv, int *pipefd, char **envp)
{
	int	fd;
	fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	dup2(fd, STDOUT_FILENO);
	dup2(pipefd[0], STDIN_FILENO);
	close (pipefd[1]);
	wait(NULL);
	exec(argv[3], envp);
}

int main(int argc, char **argv, char **envp)
{
	int		pipefd[2];
	pid_t	pid;

	if (argc != 5)
		return (0);
	if (pipe(pipefd) == -1)
	{
		perror("Error al crear la tuberia");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("Error al hacer fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
		child(argv, pipefd, envp);
	else
		parent(argv, pipefd, envp);
	return 0;
}
