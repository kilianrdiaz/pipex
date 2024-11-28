/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 15:23:38 by kroyo-di          #+#    #+#             */
/*   Updated: 2024/11/28 20:18:30 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex_bonus.h"
#include <stdio.h>

void	exec(char *cmd, char **envp)
{
	char	**s_cmd;
	char	*path;

	s_cmd = ft_split(cmd, ' ');
	path = get_path(s_cmd[0], envp);
	if (execve(path, s_cmd, envp) == -1)
	{
		ft_free_tab(s_cmd);
		error_handler(4);
	}
	ft_free_tab(s_cmd);
	free(path);
}

void	here_doc(char *limiter, int argc)
{
	pid_t	pid;
	int		pipefd[2];
	char	*line;

	if (argc < 6)
		exit (EXIT_SUCCESS);
	if (pipe(pipefd) == -1)
		error_handler(1);
	pid = fork();
	if (pid == 0)
	{
		close(pipefd[0]);
		line = get_next_line(0);
		while (line)
		{
			if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
				exit(EXIT_SUCCESS);
			write(pipefd[1], line, ft_strlen(line));
			line = get_next_line(0);
		}
	}
	else
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		wait(NULL);
	}
}

int	child(char *cmd, char **envp, char **argv, int argc)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;
	int		pos;

	if (pipe(pipefd) == -1)
		error_handler(1);
	pid = fork();
	if (pid < 0)
		error_handler(2);
	pos = open_file(cmd, argv, argc);
	if (pid == 0)
	{
		close(pipefd[0]);
		if (pos != 2)
			dup2(pipefd[1], STDOUT_FILENO);
		exec(cmd, envp);
	}
	else
	{
		close(pipefd[1]);
		if (pos == 1)
			dup2(pipefd[0], STDIN_FILENO);
	}
	waitpid(pid, &status, 0);
	return (status);
}

int	main(int argc, char **argv, char **envp)
{
	int		i;
	int		status;

	if (argc < 5)
		return (0);
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{ 
		i = 3;
		//outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0777);
		here_doc(argv[2], argc);
	}
	else
	{
		i = 2;
		/*infile = open(argv[1], O_RDONLY);
		outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (outfile < 0 || infile < 0)
			error_handler(5);
		dup2(infile, STDIN_FILENO);*/
	}
	status = child(argv[i++], envp, argv, argc);
	while (i < (argc - 1) && WEXITSTATUS(status) == 0)
		status = child(argv[i++], envp, argv, argc);
	/*dup2(outfile, STDOUT_FILENO);
	exec(argv[argc - 2], envp);
	error_handler(4);*/
	return (WEXITSTATUS(status));
}
