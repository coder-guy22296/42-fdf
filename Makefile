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
INCLUDES = minilibx/mlx.h
all: $(NAME)
	
$(NAME):
	make -C minilibx/
	gcc -Wall -Wextra -Werror -c $(SRC)
	gcc -o $(NAME) $(OFILES) -I minilibx/ -L minilibx/ -lmlx -framework OpenGL -framework AppKit

clean:
	rm -rf $(OFILES)
	make -C minilibx/ clean

fclean: clean
	make -C minilibx/ clean
	rm -f $(NAME)

re: fclean all
