/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 15:23:53 by kroyo-di          #+#    #+#             */
/*   Updated: 2024/11/26 15:02:54 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <stdlib.h>
# include "Libft/libft.h"

char	*get_path(char *cmd, char **env);
void	ft_free_tab(char **tab);
void	error_handler(int error);

#endif
