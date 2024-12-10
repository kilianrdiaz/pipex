# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kroyo-di <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/02 15:29:13 by kroyo-di          #+#    #+#              #
#    Updated: 2024/12/10 12:50:37 by kroyo-di         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Variables
CC = cc
CFLAGS = -Wall -Wextra -Werror

LIBFT_DIR = Libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC = pipex.c utils.c
OBJ = $(SRC:.c=.o)

BONUS_SRC = pipex_bonus.c utils_bonus.c
BONUS_OBJ = $(BONUS_SRC:.c=.o)

NAME = pipex
BONUS_NAME = pipex_bonus


# Reglas
all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $^ -L$(LIBFT_DIR) -lft

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

%.o: %.c pipex.h Makefile
	@$(CC) $(CFLAGS) -c $< -o $@

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_OBJ) $(LIBFT)
	$(CC) $(CFLAGS) -o $(BONUS_NAME) $^ -L$(LIBFT_DIR) -lft

clean:
	@$(MAKE) clean -C $(LIBFT_DIR)
	@rm -f $(OBJ) $(BONUS_OBJ)

fclean: clean
	@$(MAKE) fclean -C $(LIBFT_DIR)
	@rm -f $(NAME) $(BONUS_NAME)

re: fclean all

.PHONY: all clean fclean re bonus
