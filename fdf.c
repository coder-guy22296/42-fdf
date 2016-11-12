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
#include <stdlib.h>
#include <math.h>
#define X 0
#define Y 1
#define Z 2

typedef struct	s_3d_vector
{
	int x_pos;
	int y_pos;
	int z_pos;
	double x_rot;
	double y_rot;
	double z_rot;
}				t_3d_vector;

typedef struct	s_point
{
	int y
	int x;
}				t_point;

typedef struct	s_3d_point
{
	int x;
	int y;
	int z;
}				t_3d_point;

typedef struct	s_3d_object
{
	t_list vertices;
	3d_vector pos_vector;
}				t_3d_object;

typedef struct	s_3d_scene
{
	t_3d_vector camera_vector;
	t_3d_point	origin_point;


}				t_3d_scene;

t_point *new_point(int x, int y)
{

}

int *projection(int x, int y, int z)
{
	int *point;
	int viewerpos[] = {1, 1, 1};
	point = (int *)malloc(sizeof(int) * 2);
	point[X] = ((viewerpos[Z] / z) * x) - viewerpos[X];
	point[Y] = ((viewerpos[Z] / z) * y) - viewerpos[Y];
	printf("render point: (%d,%d)", point[X], point[Y]);
	///exit(1);
	return (point);
}

void drawline(void *mlx, void *window, int x0, int y0, int x1, int y1)
{
	double deltax = 0;
	double deltay = 0;
	double error;
	double deltaerror;
	int stop = 0;
	int x = 0;
	int y = 0;

	if (x1 >= x0 && y1 >= y0)
	{
		deltax = x1 - x0;
		deltay = y1 - y0;
	}
	else if (x0 >= x1 && y0 >= y1)
	{
		deltax = x0 - x1;
		deltay = y0 - y1;
	}
	
	error = -1.0;
	if (deltay/deltax < 1)
	{
		//printf("dx:%f dy:%f", deltax, deltay);
		//exit(1);
		x = x0;
		y = y0;
		if (x1 >= x0 && y1 >= y0)
		{
			x = x0;
			y = y0;
			stop = x1;
		}
		else if (x0 >= x1 && y0 >= y1)
		{
			x = x1;
			y = y1;
			stop = x0;
		}

		deltaerror = fabs(deltay/deltax);
		error += deltaerror;
		while (x <= stop)
		{
			mlx_pixel_put(mlx, window, x+500, y+500, 0x00FF00FF);
			error += deltaerror;
			if (error >= 0)
			{
				y++;
				error -= 1.0;
			}
			x++;			
		}
	}
	else if (deltay/deltax > 1)
	{
		x = x0;
		y = y0;		
		if (x1 >= x0 && y1 >= y0)
		{
			x = x0;
			y = y0;
			stop = y1;
		}
		else if (x0 >= x1 && y0 >= y1)
		{
			x = x1;
			y = y1;
			stop = y0;
		}
		deltaerror = fabs(deltax/deltay);
		error += deltaerror;
		while (y <= stop)
		{
			mlx_pixel_put(mlx, window, x+500, y+500, 0x00FF00FF);
			error += deltaerror;
			if (error >= 0)
			{
				x++;
				error -= 1.0;
			}
			y++;			
		}
	}
}

void drawline3d(void *mlx, void *window, int x0, int y0, int z0, int x1, int y1, int z1)
{
	int *point1;
	int *point2;

	point1 = projection(x0, y0, z0);
	point2 = projection(x1, y1, z1);
	drawline(mlx, window, point1[0], point1[1], point2[0], point2[1]);
}

int main()
{
	void *mlx;
	void *window;
	//void *point1;
	//void *point2;

	mlx = mlx_init();
	window = mlx_new_window(mlx, 1000, 1000, "line drawing");
	//drawline(mlx, window, 0-17, 0-17, 5-17, 30-17);
	//drawline(mlx, window, 5-17, 30-17, 35-17, 35-17);
	//drawline(mlx, window, 35-17, 35-17, 30-17, 5-17);
	//drawline(mlx, window, 30-17, 5-17, 0-17, 0-17);

	//drawline(mlx, window, 0-17, 0-17, 30-17, 5-17);
	//drawline(mlx, window, 30-17, 5-17, 35-17, 35-17);
	//drawline(mlx, window, 35-17, 35-17, 5-17, 30-17);
	//drawline(mlx, window, 5-17, 30-17, 0-17, 0-17);

	//drawline(mlx, window, 50, 50, 50, 100);
	//drawline(mlx, window, 50, 100, 100, 100);
	//drawline(mlx, window, 100, 100, 100, 50);
	//drawline(mlx, window, 100, 50, 50, 50);
	//drawline(mlx, window, 10, 30, 12, 25);
	//drawline(mlx, window, 12, 25, 25, 2);
	//drawline(mlx, window, 25, 2, 0, 0);


	drawline3d(mlx, window, 1, 1, 1, 1, 1, 150);
	drawline3d(mlx, window, 1, 1, 1, 1, 150, 1);
	drawline3d(mlx, window, 1, 1, 1, 150, 1, 1);
	//drawline3d(mlx, window, 100, 100, 0, 100, 100, 100);
	//drawline3d(mlx, window, 100, 100, 0, 100, 100, 100);
	//drawline3d(mlx, window, 100, 100, 0, 100, 100, 100);
	mlx_loop(mlx);
	return (0);
}
/*

int main()
{
	void *mlx;
	void *window;
	int x;
	int y;
	*//*double matrix[4][4] = {	{0, 0, 0, 0},
							{0, 0, 0, 0},
							{0, 0, 0, 0}};*//*
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
	*//*x = 50;
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
	mlx_pixel_put(mlx, window, 50, 50, 0x00FFFFFF);*//*
	mlx_loop(mlx);
	return (0);
}

*/