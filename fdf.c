/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyildiri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/08 18:57:06 by cyildiri          #+#    #+#             */
/*   Updated: 2016/11/08 18:57:09 by cyildiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minilibx_macos/mlx.h"
#include "stdio.h"

int main()
{
	void *mlx;
	void *window;
	int x;
	int y;
	/*double matrix[4][4] = {	{0, 0, 0, 0},
							{0, 0, 0, 0},
							{0, 0, 0, 0}};*/
	int viewerpos[] = {0, 0, -6};
	int objectpos[] = {500, 500, 5};
	int objectpos2[] = {505, 505, 4};
	int objectpos3[] = {510, 510, 6};
	//int screenpos[] = {99, 99};



	mlx = mlx_init();
	window = mlx_new_window(mlx, 1000, 1000, "testing yoooooo");
	x = -1 *((viewerpos[2]/objectpos[2])*objectpos[0]) - viewerpos[0];
	y = -1 *((viewerpos[2]/objectpos[2])*objectpos[1]) - viewerpos[1];

	mlx_pixel_put(mlx, window, x, y, 0x00FF00FF);
	printf("coords: (%d,%d)\n", x, y);



	
	x = -1 *((viewerpos[2]/objectpos2[2])*objectpos2[0]) - viewerpos[0];
	y = -1 *((viewerpos[2]/objectpos2[2])*objectpos2[1]) - viewerpos[1];

	mlx_pixel_put(mlx, window, x, y, 0x00FFFFFF);
	printf("coords: (%d,%d)\n", x, y);

	
	x = -1 *((viewerpos[2]/objectpos3[2])*objectpos3[0]) - viewerpos[0];
	y = -1 *((viewerpos[2]/objectpos3[2])*objectpos3[1]) - viewerpos[1];

	mlx_pixel_put(mlx, window, x, y, 0x00FFFF00);
	printf("coords: (%d,%d)\n", x, y);
	/*x = 50;
	while (x < 500)
	{
		y = 50;
		while (y < 500)
		{
			mlx_pixel_put(mlx, window, x, y, x+y+(x*x + y*y)+ 4916819);
			y++;
		}
		x++;
	}
	mlx_pixel_put(mlx, window, 50, 50, 0x00FFFFFF);*/
	mlx_loop(mlx);
	return (0);
}