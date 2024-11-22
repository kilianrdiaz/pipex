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
		perror ("Error ocurred while using execve().");
		ft_free_tab(s_cmd);
		exit(EXIT_FAILURE);
	}
}

void	first_child(char **argv, int	*pipefd, char **envp)
{
	int	fd;

	fd = open(argv[1], O_RDONLY, 0777);
	dup2(fd, STDIN_FILENO);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[0]);
	exec(argv[2], envp);
}

void	second_child(char **argv, int *pipefd, char **envp)
{
	int	fd;
	fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	dup2(fd, STDOUT_FILENO);
	dup2(pipefd[0], STDIN_FILENO);
	close (pipefd[1]);
	exec(argv[3], envp);
}
	
int main(int argc, char **argv, char **envp)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (argc != 5)
		return (0);
	if (pipe(pipefd) == -1)
	{
		perror("Error ocurred while creating pipe().");
		exit(EXIT_FAILURE);
	}
	pid1 = fork();
	if (pid1 < 0)
	{
		perror("Error ocurred while using first fork().");
		exit(EXIT_FAILURE);
	}
	else if (pid1 == 0)
		first_child(argv, pipefd, envp);
	pid2 = fork();
	if (pid2 < 0)
	{
		perror("Error ocurred while using second fork().");
		exit(EXIT_FAILURE);
	}
	else if (pid2 == 0)
		second_child(argv, pipefd, envp);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return 0;
}
