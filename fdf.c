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
	int x;
	int y;
}				t_point;

typedef struct	s_3d_point
{
	int x;
	int y;
	int z;
}				t_3d_point;

typedef struct	s_3d_object
{
	int			*faces_arr;
	int			face_cnt;
	int			*vertex_ind;
	t_3d_point	*vertices;
	t_3d_vector	pos_vector;

}				t_3d_object;

typedef struct	s_3d_scene
{
	t_3d_vector *viewport_vector;
	int viewport_cnt;
	int projection_method;
	t_3d_point	origin_point;
}				t_3d_scene;

typedef struct	s_renderer
{
	void *mlx;
	void *windows;
	int window_cnt;
	t_3d_scene *scenes;
	int scenes_cnt;
}				t_renderer;

/*
**	allocates and initializes a point struct
**	delete with free()
*/
t_point *new_point(int x, int y)
{
	t_point *new;

	if (!(new = (t_point *)/*ft_me*/malloc(sizeof(t_point))))
		return (NULL);
	new->x = x;
	new->y = y;
	return (new);
}

/*
**	allocates and initializes a 3d point struct
**	delete with free()
*/
t_3d_point *new_3d_point(int x, int y, int z)
{
	t_3d_point *new;

	if (!(new = (t_3d_point *)/*ft_me*/malloc(sizeof(t_3d_point))))
		return (NULL);
	new->x = x;
	new->y = y;
	new->z = z;
	return (new);
}

/*
**	allocates and initializes a 3d vector struct
**	stores a point in space and its direction
**	delete with free()
*/
t_3d_vector *new_3d_vector(int x, int y, int z, int x_rot, int y_rot, int z_rot)
{
	t_3d_vector *new;

	if (!(new = (t_3d_vector *)/*ft_me*/malloc(sizeof(t_3d_vector))))
		return (NULL);
	new->x_pos = x;
	new->y_pos = y;
	new->z_pos = z;
	new->x_rot = x_rot;
	new->y_rot = y_rot;
	new->z_rot = z_rot;
	return (new);
}
/*
t_3d_object *new_3d_object(parameters)
{
	t_3d_object *new;

	if (!(new = (t_3d_object *)ft_memalloc(sizeof(t_3d_object))))
		return (NULL);
	//initalization happens here

	return (new);
}*/

int *projection(t_3d_point coord/*, t_3d_scene scene*/)
{
	int *point;
	int viewerpos[] = {1, 1, 50};
	point = (int *)malloc(sizeof(int) * 2);//needs protection
	point[X] = ((viewerpos[Z] / coord.z) * coord.x) - viewerpos[X];
	point[Y] = ((viewerpos[Z] / coord.z) * coord.y) - viewerpos[Y];
	printf("(%d,%d,%d) projected -> (%d,%d)\n", coord.x, coord.y, coord.z, point[X], point[Y]);
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
	printf("linedraw call:\n");
	printf("*********************************************************************\n");
	/*if (x1 >= x0)
	{
		deltax = x1 - x0;
	}
	else if (x0 >= x1)
	{*/
		deltax = x1 - x0;
	/*} 
	if (y1 >= y0)
	{
		deltay = y1 - y0;
	}
	else if (y0 >= y1)
	{*/
		deltay = y1 - y0;
	/*}*/
	
	error = -1.0;
	printf("dx:%f dy:%f\n", deltax, deltay);
	if ( deltay/deltax <= 1/* && x0 != -26 && x1 != -251*/)
	{
		//printf("dx:%f dy:%f", deltax, deltay);
		//exit(1);

		printf(":case 1:\n");
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
			//printf("point (%d,%d) rendered!",x,y);
			error += deltaerror;
			if (error >= 0)
			{
				y++;
				error -= 1.0;
			}
			x++;			
		}
	}
	else if (deltay/deltax >= 1)
	{
		printf(":case 2:\n");
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
			//printf("point (%d,%d) rendered!",x,y);
			error += deltaerror;
			if (error >= 0)
			{
				x++;
				error -= 1.0;
			}
			y++;			
		}
	}
	printf("*********************************************************************\n");
}

void drawline3d(void *mlx, void *window, t_3d_point *coord1, t_3d_point *coord2)
{
	int *point1;
	int *point2;

	point1 = projection(*coord1);
	point2 = projection(*coord2);
	drawline(mlx, window, point1[0], point1[1], point2[0], point2[1]);
	free(point1);
	free(point2);
}


/*
**	render an object
**	define mode: WIRE_FRAME=0 SOLID_POLY=1
*/
void render3d_object(void *mlx, void *window, t_3d_object obj, int mode)
{
	int cur_face;
	int cur_vertex;
	int *face_vertices;
	int next_vertex;


	mode = -99;
	cur_face = 0;
	cur_vertex = 0;
	face_vertices = obj.vertex_ind;
	while (cur_face < obj.face_cnt)
	{
		cur_vertex = 0;
		while (cur_vertex < obj.faces_arr[cur_face])
		{
			if (cur_vertex + 1 < obj.faces_arr[cur_face])
				next_vertex = cur_vertex + 1;
			else
				next_vertex = 0;
			drawline3d(mlx, window, &obj.vertices[face_vertices[cur_vertex]],
									&obj.vertices[face_vertices[next_vertex]]);
			printf("rendered a line\n\n");
			cur_vertex++;
		}
		printf("\nrendered a face^^^^^(%d)\n", cur_vertex);
		face_vertices = &face_vertices[cur_vertex];
		printf("(%p)next face verticies index array mem address\n", face_vertices);
		cur_face++;
	}
	//exit(1);
}

int main()
{
	void *mlx;
	void *window;
	//void *point1;
	//void *point2;
	t_3d_object obj;


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
	
	int faces[] = {4, 4, 4, 4, 4, 4};
	int vertex_ind[] = {	0,1,2,3,		//FRONT
							4,5,6,7,		//BACK
							4,5,1,0,		//TOP
							6,7,2,3,		//BOTTOM
							1,5,6,2,		//LEFT
							0,4,7,3			//RIGHT
						 };
	t_3d_point vertices[] = {	{ 5,  5,  1 },
								{-5,  5,  1 },
								{-5, -5,  1 },
								{ 5, -5,  1 },
								{ 5,  5,  10},
								{-5,  5,  10},
								{-5, -5,  10},
								{ 5, -5,  10}	};
	obj.faces_arr = &faces[0];
	obj.face_cnt = 6;
	obj.vertex_ind = &vertex_ind[0];
	obj.vertices = &vertices[0];
	render3d_object(mlx, window, obj, 0);


	//drawline3d(mlx, window, new_3d_point(1, 1, 1), new_3d_point(1, 1, 150));
	//drawline3d(mlx, window, new_3d_point(1, 1, 1), new_3d_point(1, 150, 1));
	//drawline3d(mlx, window, new_3d_point(1, 1, 1), new_3d_point(150, 1, 1));
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
//only works if viewer is 1 unit from canvas
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