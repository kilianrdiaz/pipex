/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 15:23:38 by kroyo-di          #+#    #+#             */
/*   Updated: 2024/11/27 22:03:47 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex_bonus.h"

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

void	child(char *cmd, char **envp)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
		error_handler(1);
	pid = fork();
	if (pid < 0)
		error_handler(2);
	if (pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		exec(cmd, envp);
	}
	else
	{
		close(pipefd[1]);
		dup2(pipefd[1], STDIN_FILENO);
		waitpid(pid, NULL, 0);
	}
}

int	main(int argc, char **argv, char **envp)
{
	int		i;
	int		infile;
	int		outfile;

	if (argc < 5)
		return (0);
	if (ft_strncmp(argv[1], "here_doc", 8))
	{ 
		i = 3;
		outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0777);
		here_doc(argv[2], argc);
	}
	else
	{
		i = 2;
		infile = open(argv[1], O_RDONLY);
		outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (outfile == -1 || infile == -1)
			error_handler(5);
		dup2(infile, STDIN_FILENO);
	}
	while (i < (argc - 2))
		child(argv[i++], envp);
	dup2(outfile, STDOUT_FILENO);
	exec(argv[argc - 2], envp);
	exit(EXIT_SUCCESS);
}
