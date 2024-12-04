/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 15:23:38 by kroyo-di          #+#    #+#             */
/*   Updated: 2024/12/04 16:52:15 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex_bonus.h"

void	check_args(int here_doc, int argc)
{
	if (argc < 5)
		exit(EXIT_SUCCESS);
	else if (here_doc == 1 && argc < 6)
		exit(EXIT_SUCCESS);
	return ;
}

void	here_doc(char *limiter, int argc)
{
	pid_t	pid;
	int		pipefd[2];
	char	*line;

	check_args(1, argc);
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
	}
}

int	child(char *cmd, char **envp, int outfile)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipefd) == -1)
		error_handler(1);
	pid = fork();
	if (pid < 0)
		error_handler(2);
	if (pid == 0)
	{
		close(pipefd[0]);
		if (outfile > 0)
			dup2(outfile, STDOUT_FILENO);
		else
			dup2(pipefd[1], STDOUT_FILENO);
		exec(cmd, envp);
	}
	else
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
	}
	waitpid(pid, &status, 0);
	return (status);
}

int	main(int argc, char **argv, char **envp)
{
	int		i;
	int		status;
	int		outfile;
	int		infile;

	check_args(0, argc);
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
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
		if (outfile < 0 || infile < 0)
			error_handler(5);
		dup2(infile, STDIN_FILENO);
	}
	while (i < (argc - 2))
		status = child(argv[i++], envp, 0);
	status = child(argv[i++], envp, outfile);
	return (WEXITSTATUS(status));
}
