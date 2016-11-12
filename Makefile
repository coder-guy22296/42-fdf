#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cyildiri <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/11/08 19:17:08 by cyildiri          #+#    #+#              #
#    Updated: 2016/11/08 19:17:11 by cyildiri         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME = fdf
SRC = fdf.c
OFILES = fdf.o
INCLUDES = minilibx_macos/mlx.h
all: $(NAME)
	
$(NAME):
	make -C minilibx_macos/
	gcc -Wall -Wextra -Werror -c $(SRC)
	gcc -o $(NAME) $(OFILES) -I minilibx_macos/ -L minilibx_macos/ -lmlx -framework OpenGL -framework AppKit

clean:
	rm -rf $(OFILES)
	make -C minilibx_macos/ clean

fclean: clean
	make -C minilibx_macos/ clean
	rm -f $(NAME)

re: fclean all
