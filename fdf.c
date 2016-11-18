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

typedef struct	s_vec2i
{
	int x;
	int y;
}				t_vec2i;

typedef struct	s_vec2f
{
	float x;
	float y;
}				t_vec2f;

typedef struct	s_vec3f
{
	float x;
	float y;
	float z;
}				t_vec3f;

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
	void (*transform)(struct	s_3d_object obj);

}				t_3d_object;

typedef struct	s_3d_scene
{
	t_3d_vector *eye_relative_canvas;
	t_3d_vector *viewport_vector;
	int viewport_cnt;
	t_3d_point	origin_point;
	t_3d_object *objects;
	int object_cnt;

	void (*projection_method)(struct s_3d_scene scene);
}				t_3d_scene;

typedef struct	s_renderer
{
	void *mlx;
	void *window;
	//int window_cnt;
	t_3d_scene *scenes;
	int scenes_cnt;


	void (*render)(struct	s_renderer renderer, t_3d_scene scene);
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

t_point point(int x, int y)
{
	t_point new;
	new.x = x;
	new.y = y;
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

/*t_point perspective_projection(t_3d_point coord, t_3d_scene scene)
{
	t_point point;
	int viewerpos[] = {0, 0, 100};
	point.x = (double)((((double)viewerpos[Z] / ((double)coord.z)) * (double)coord.x) - (double)viewerpos[X]);
	point.y = (double)((((double)viewerpos[Z] / ((double)coord.z)) * (double)coord.y) - (double)viewerpos[Y]);
	printf("(%d,%d,%d) projected -> (%d,%d)\n", coord.x, coord.y, coord.z, point.x, point.y);
	///exit(1);
	return (point);
}*/

t_point perspective_projection(t_3d_point coord/*, t_3d_scene scene*/)
{


	int viewerpos[] = {0, 0, 1};
	
	t_3d_vector camera = {0, 0, 0, 0.0, 0.0, 0.0};
	t_vec3f new_coord;
	t_vec2f projection;
	t_vec2f normalized_coords;
	t_point rasterized_coords;

	new_coord.x = cos(camera.y_rot)*(sin(camera.z_rot)*(coord.y-camera.y_pos) + 
	cos(camera.z_rot)*(coord.x-camera.x_pos)) - sin(camera.y_rot)*(coord.z-camera.z_pos);

	new_coord.y = sin(camera.x_rot)*(cos(camera.y_rot)*(coord.z-camera.z_pos) + sin(camera.y_rot)*(sin(camera.z_rot)*(coord.y-camera.y_pos) + cos(camera.z_rot)*(coord.x-camera.x_pos))) + 
	cos(camera.x_rot)*(cos(camera.z_rot)*(coord.y-camera.y_pos) - sin(camera.z_rot)*(coord.x-camera.x_pos));

	new_coord.z = cos(camera.x_rot)*(cos(camera.y_rot)*(coord.z-camera.z_pos) + sin(camera.y_rot)*(sin(camera.z_rot)*(coord.y-camera.y_pos) + cos(camera.z_rot)*(coord.x-camera.x_pos))) - 
	sin(camera.x_rot)*(cos(camera.z_rot)*(coord.y-camera.y_pos) - sin(camera.z_rot)*(coord.x-camera.x_pos));


	// projection
	projection.x = (double)((((double)viewerpos[Z] / ((double)new_coord.z)) * (double)new_coord.x) - (double)viewerpos[X]);
	projection.y = (double)((((double)viewerpos[Z] / ((double)new_coord.z)) * (double)new_coord.y) - (double)viewerpos[Y]);

	// normalization + screen spacing
	normalized_coords.x = (projection.x + 1)/2;
	normalized_coords.y = (1 - projection.y)/2;
	// rasterization
	rasterized_coords.x = (int)(normalized_coords.x * 1000);
	rasterized_coords.y = (int)(normalized_coords.y * 1000);
	//printf("(%d,%d,%d) projected -> (%d,%d)\n", new_coord.x, new_coord.y, new_coord.z, point.x, point.y);
	///exit(1);
	return (rasterized_coords);
}

t_point orthographic_projection(t_3d_point coord/*, t_3d_scene scene*/)
{
	t_point point;
	int offset[] = {0, 0, 0};
	int scale[] = {1, 1, 1};
	point.x = scale[X]*coord.x + offset[X];
	point.y = scale[Y]*coord.y + offset[Y];
	printf("(%d,%d,%d) projected -> (%d,%d)\n", coord.x, coord.y, coord.z, point.x, point.y);
	///exit(1);
	return (point);
}

t_3d_point translate(t_3d_point coord, t_3d_vector obj_vector)
{
	coord.x += obj_vector.x_pos;
	coord.y += obj_vector.y_pos;
	coord.z += obj_vector.z_pos;
	return (coord);
}

void drawline(void *mlx, void *window, t_point point_a, t_point point_b)
{
	int		deltax = point_b.x - point_a.x;
	int		deltay = point_b.y - point_a.y;
	double	slope;
	if (deltax == 0)
		slope = 0;
	else 
		slope = (double)deltay/(double)deltax;
	double	error = -1.0;
	double	deltaerr = fabs(slope);
	int		xdir = 1;
	int		ydir = 1;
	int		x = point_a.x;
	int		y = point_a.y;

	if (deltax < 0)
		xdir = -1;
	if (deltay < 0)
		ydir = -1;
	//printf("xdir:%d ydir:%d slope:%f deltaerr:%f\n", xdir, ydir, slope, deltaerr);
	while ((fabs(slope) > 1.0 || deltax == 0) && y != point_b.y)
	{
		if (y == point_a.y)
		{
			deltaerr = fabs((double)deltax/(double)deltay);
			//printf("new delta err:%f\n", deltaerr);
			error += deltaerr;
		}
		mlx_pixel_put(mlx, window, x, y, 0x00FF00FF);
		//printf("case2: rendered(%d,%d) error:%f\n", x, y, error);
		error += deltaerr;
		if (error >= 0.0)
		{
			x += xdir;
			error -= 1.0;
		}
		y += ydir;
	}
	error += deltaerr;
	while (fabs(slope) <= 1.0 && x != point_b.x)
	{
		mlx_pixel_put(mlx, window, x, y, 0x00FF00FF);
		//printf("case1: rendered(%d,%d) error:%f\n", x, y, error);
		error += deltaerr;
		if (error >= 0.0)
		{
			y += ydir;
			//printf("minor shift\n");
			error -= 1.0;
		}
		x += xdir;
		//printf("major shift\n");
	}
}

void drawline3d(void *mlx, void *window, t_3d_point *coord1, t_3d_point *coord2)
{
	t_point point1;
	t_point point2;

	point1 = perspective_projection(*coord1);
	point2 = perspective_projection(*coord2);

	drawline(mlx, window, point1, point2);
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
	t_3d_point vert1;
	t_3d_point vert2;

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
			vert1 = translate(obj.vertices[face_vertices[cur_vertex]], obj.pos_vector);
			vert2 = translate(obj.vertices[face_vertices[next_vertex]], obj.pos_vector);
			drawline3d(mlx, window, &vert1, &vert2);
			printf("rendered a line\n\n");
			cur_vertex++;
		}
		printf("\nrendered a face^^^^^(%d)\n", cur_vertex);
		face_vertices = &face_vertices[cur_vertex];
		//printf("(%p)next face verticies index array mem address\n", face_vertices);
		cur_face++;
	}
	//exit(1);
}

void render_scene(t_renderer renderer, t_3d_scene scene)
{
	int i;

	i = 0;
	while (i < scene.object_cnt)
	{
		render3d_object(renderer.mlx, renderer.window, scene.objects[i], 0);
		i++;
	}
}

int render_loop(void *param)
{
	t_renderer renderer = *((t_renderer *)param);
	renderer.render(renderer, *renderer.scenes);
	mlx_clear_window(renderer.mlx, renderer.window);
	return (0);
}


int key_pressed(int keycode, void *param)
{
	t_renderer *renderer = (t_renderer *)param;
	if (keycode == 13)		//W
	{
		//(renderer->scenes)[0].objects[0].vertices[1].y -= 1;
		printf("before: %d\n", (renderer->scenes)[0].objects[0].pos_vector.z_pos);
		//if ((renderer->scenes)[0].objects[0].pos_vector.z_pos - 1 <= -6)
			(renderer->scenes)[0].objects[0].pos_vector.z_pos += 1;
		printf("after: %d\n", (renderer->scenes)[0].objects[0].pos_vector.z_pos);
	}
	else if (keycode == 0)	//A
	{
	//	(renderer->scenes)[0].objects[0].vertices[1].x -= 1;
		printf("before: %d\n", (renderer->scenes)[0].objects[0].pos_vector.x_pos);
			(renderer->scenes)[0].objects[0].pos_vector.x_pos -= 1;
		printf("after: %d\n", (renderer->scenes)[0].objects[0].pos_vector.x_pos);
	}
	else if (keycode == 1)	//S
	{
		//(renderer->scenes)[0].objects[0].vertices[1].y += 1;
		printf("before: %d\n", (renderer->scenes)[0].objects[0].pos_vector.z_pos);
		//if ((renderer->scenes)[0].objects[0].pos_vector.z_pos + 1 <= -6)
			(renderer->scenes)[0].objects[0].pos_vector.z_pos -= 1;
		printf("after: %d\n", (renderer->scenes)[0].objects[0].pos_vector.z_pos);
	}
	else if (keycode == 2)	//D
	{
		//(renderer->scenes)[0].objects[0].vertices[1].x += 1;
		printf("before: %d\n", (renderer->scenes)[0].objects[0].pos_vector.x_pos);
			(renderer->scenes)[0].objects[0].pos_vector.x_pos += 1;
		printf("after: %d\n", (renderer->scenes)[0].objects[0].pos_vector.x_pos);
	}
	else if (keycode == 126)	//UP
	{
		//(renderer->scenes)[0].objects[0].vertices[0].y -= 1;
		printf("before: %d\n", (renderer->scenes)[0].objects[0].pos_vector.x_pos);
		(renderer->scenes)[0].objects[0].pos_vector.y_pos += 1;
		printf("after: %d\n", (renderer->scenes)[0].objects[0].pos_vector.x_pos);
	}
	else if (keycode == 123)	//LEFT
	{
		(renderer->scenes)[0].objects[0].vertices[0].x -= 1;
	}
	else if (keycode == 125)	//DOWN
	{
	//	(renderer->scenes)[0].objects[0].vertices[0].y += 1;
		printf("before: %d\n", (renderer->scenes)[0].objects[0].pos_vector.x_pos);
		(renderer->scenes)[0].objects[0].pos_vector.y_pos -= 1;
		printf("after: %d\n", (renderer->scenes)[0].objects[0].pos_vector.x_pos);
	}
	else if (keycode == 124)	//RIGHT
	{
		(renderer->scenes)[0].objects[0].vertices[0].x += 1;
	}
	if (param)
	printf("key pressed: %d\n", keycode);
	render_loop(renderer);
	return (0);
}
int main()
{
	t_renderer renderer;
	
	
	//void *mlx;
	//void *window;
	//void *point1;
	//void *point2;
	t_3d_object obj;
	t_3d_object obj2;
	t_3d_scene scene;


	renderer.mlx = mlx_init();
	renderer.window = mlx_new_window(renderer.mlx, 1000, 1000, "line drawing");
	//drawline(mlx, window, 0-17, 0-17, 5-17, 30-17);
	//drawline(mlx, window, 5-17, 30-17, 35-17, 35-17);
	//drawline(mlx, window, 35-17, 35-17, 30-17, 5-17);
	//drawline(mlx, window, 30-17, 5-17, 0-17, 0-17);

	//drawline(mlx, window, 0-17, 0-17, 30-17, 5-17);
	//drawline(mlx, window, 30-17, 5-17, 35-17, 35-17);
	//drawline(mlx, window, 35-17, 35-17, 5-17, 30-17);
	//drawline(mlx, window, 5-17, 30-17, 0-17, 0-17);

	/*drawline(mlx, window, point(50, 50), point(50, 100));
	drawline(mlx, window, point(50, 100), point(100, 100));
	drawline(mlx, window, point(100, 100), point(100, 50));
	drawline(mlx, window, point(100, 50), point(50, 50));
	drawline(mlx, window, point(10, 30), point(12, 25));
	drawline(mlx, window, point(12, 25), point(25, 2));
	drawline(mlx, window, point(25, 2), point(0, 0));*/

	//drawline(mlx, window, point(1, 1), point(6, 3));


	
	int faces[] = {4, 4, 4, 4, 4, 4};
	int vertex_ind[] = {	0,1,2,3,		//FRONT
							4,5,6,7,		//BACK
							4,5,1,0,		//TOP
							7,6,2,3,		//BOTTOM
							1,5,6,2,		//LEFT
							0,4,7,3			//RIGHT
						 };
	t_3d_point vertices[] = {	{ 50,  50,  50 },
								{-50,  50,  50 },
								{-50, -50,  50 },
								{ 50, -50,  50 },
								{ 50,  50,   -50 },
								{-50,  50,   -50 },
								{-50, -50,   -50 },
								{ 50, -50,   -50 }	};

	obj.faces_arr = &faces[0];
	obj.face_cnt = 6;
	obj.vertex_ind = &vertex_ind[0];
	obj.vertices = &vertices[0];
/////////////////////////////////////////////////////////////
	int faces2[] = {2};
	int vertex_ind2[] = {0,1};
	t_3d_point vertices2[] = {	{ 0,  0,  1 },
								{ -2, -1,  1 }	};

	obj2.faces_arr = &faces2[0];
	obj2.face_cnt = 1;
	obj2.vertex_ind = &vertex_ind2[0];
	obj2.vertices = &vertices2[0];
/////////////////////////////////////////////////////////////


	t_3d_vector pos = {0,0,-100,0.0,0.0,0.0};
	obj.pos_vector = pos;
	scene.objects = &obj;
	scene.object_cnt = 1;
	renderer.scenes = &scene;
	renderer.scenes_cnt = 1;
	renderer.render = render_scene;
	renderer.render(renderer, *renderer.scenes);
	//render3d_object(renderer.mlx, renderer.window, obj, 0);


	//drawline3d(mlx, window, new_3d_point(1, 1, 1), new_3d_point(1, 1, 150));
	//drawline3d(mlx, window, new_3d_point(1, 1, 1), new_3d_point(1, 150, 1));
	//drawline3d(mlx, window, new_3d_point(1, 1, 1), new_3d_point(150, 1, 1));
	//drawline3d(mlx, window, 100, 100, 0, 100, 100, 100);
	//drawline3d(mlx, window, 100, 100, 0, 100, 100, 100);
	//drawline3d(mlx, window, 100, 100, 0, 100, 100, 100);
	mlx_key_hook(renderer.window, key_pressed, &renderer);
	//mlx_loop_hook(renderer.mlx, render_loop, &renderer);
	mlx_loop(renderer.mlx);
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