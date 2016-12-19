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

#include "mlx.h"
#include "libgraphics.h"
#include "libft.h"
#include "stdio.h"//REMOVE
#include <stdlib.h>
#include <fcntl.h>



int render_loop(void *param)
{
	t_renderer *renderer = (t_renderer *)param;
	if(renderer->scene)
	{
		//printf("render start!\n");
		//mlx_clear_window(renderer->mlx, renderer->window);
		renderer->render(renderer, *renderer->scene);
		//printf("render complete!\n");
	}
	return (0);
}

void object_translation_controls(int keycode, t_renderer *renderer)
{
	t_3d_object	*obj;
	t_vec3fc		*pos;

	obj = ((t_3d_object *)renderer->scene->objects->content);
	pos = &(obj->pos_vector.position);
	if (keycode == UP)			// UP
		*pos = translate_point(*pos, vec3f(0, 1, 0));
	else if (keycode == DOWN)	// DOWN
		*pos = translate_point(*pos, vec3f(0, -1, 0));
	else if (keycode == LEFT)	// LEFT
		*pos = translate_point(*pos, vec3f(-1, 0, 0));
	else if (keycode == RIGHT)	// RIGHT
		*pos = translate_point(*pos, vec3f(1, 0, 0));
}

void object_rotation_controls(int keycode, t_renderer *renderer)
{
	t_3d_object *obj;

	obj = ((t_3d_object *)renderer->scene->objects->content);
	if (keycode == NUM_7)			//NUM_7
		rotate_object(obj, vec3f(0.0, 0.0, 3.14/64.0));
	else if (keycode == NUM_9)		//NUM_9
		rotate_object(obj, vec3f(0.0, 0.0, -3.14/64.0));
	else if (keycode == NUM_8)		//NUM_8
		rotate_object(obj, vec3f(-3.14/64.0, 0.0, 0.0));
	else if (keycode == NUM_5)		//NUM_5
		rotate_object(obj, vec3f(3.14/64.0, 0.0, 0.0));
	else if (keycode == NUM_4)		//NUM_4
		rotate_object(obj, vec3f(0.0, 3.14/64.0, 0.0));
	else if (keycode == NUM_6)		//NUM_6
		rotate_object(obj, vec3f(0.0, -3.14/64.0, 0.0));
}

void camera_translation_controls(int keycode, t_renderer *renderer)
{
	t_vec3fc		*pos;
	
	pos = &(renderer->scene->camera->loc.position);
	if (keycode == W)			//W
		*pos = translate_point(*pos, vec3f(0, 0, -5));
	else if (keycode == S)		//S
		*pos = translate_point(*pos, vec3f(0, 0, 5));
	else if (keycode == A)		//A
		*pos = translate_point(*pos, vec3f(-5, 0, 0));
	else if (keycode == D)		//D
		*pos = translate_point(*pos, vec3f(5, 0, 0));
	else if (keycode == R)		//R
		*pos = translate_point(*pos, vec3f(0, 5, 0));
	else if (keycode == F)		//F
		*pos = translate_point(*pos, vec3f(0, -5, 0));
}

int key_pressed(int keycode, void *param)
{
	t_renderer *renderer = (t_renderer *)param;

	object_translation_controls(keycode, renderer);
	object_rotation_controls(keycode, renderer);
	camera_translation_controls(keycode, renderer);
	if (keycode == NUM_0)		//NUM_0
	{
		if (renderer->scene->projection_method == perspective_projection)
			renderer->scene->projection_method = orthographic_projection;
		else
			renderer->scene->projection_method = perspective_projection;
	}
	if (keycode == NUM_3)		//NUM_3
		renderer->scene->scale = translate_point(renderer->scene->scale, vec3f(0.2,0.2,0.2));
	if (keycode == NUM_DOT)		//NUM_DOT
		renderer->scene->scale = translate_point(renderer->scene->scale, vec3f(-0.2,-0.2,-0.2));
	if (keycode == ESC)		//ESC
		exit (1);
	render_loop(param);
	printf("key pressed: %d\n", keycode);
	return (0);
}

int mouse_press_hook(int button, int x, int y, void *param)
{
	t_renderer *renderer;

	renderer = (t_renderer *)param;
	x -= 500;
	y -= 500;
	if (button && x && y)
		ft_putchar('\0');
	renderer->last_click.x = x;
	renderer->last_click.y = y;
	render_loop(param);
	return (0);
}

int mouse_release_hook(int button, int x, int y, void *param)
{
	t_renderer *renderer;

	renderer = (t_renderer *)param;
	x -= 500;
	y -= 500;
	if	(button)
		ft_putchar('\0');
	renderer->last_click.x = -99;
	renderer->last_click.y = -99;
	render_loop(param);
	return (0);
}

int mouse_motion_hook(int x, int y, void *param)
{
	t_renderer	*renderer;
	t_vec3fc		*rotation;

	renderer = (t_renderer *)param;
	rotation = &(renderer->scene->camera->loc.rotation);

	x -= 500;
	y -= 500;
	if (renderer->last_click.x != -99 && renderer->last_click.y != -99)
	{
		if (renderer->scene->projection_method == orthographic_projection)
			*rotation = translate_point(*rotation,
					vec3f( 0.0,((x - renderer->last_click.x)/1000.0), 0.0));
		else
			*rotation = translate_point(*rotation,
					vec3f( 0.0,-((x - renderer->last_click.x)/1000.0), 0.0));
		*rotation = translate_point(*rotation,
					vec3f(-((y - renderer->last_click.y)/1000.0), 0.0, 0.0));
		renderer->last_click.x = x;
		renderer->last_click.y = y;
		render_loop(param);
	}
	return (0);
}

static int	load_into_list(int fd, t_list **lines, int *max_column_cnt)
{
	char		*line;
	int 		line_cnt;
	int 		col_cnt;
	char 		**column_arr;

	line_cnt = 0;
	*max_column_cnt = -99;
	line = NULL;
	//read line by line
	while (get_next_line(fd, &line) == 1)
	{

		column_arr = ft_strsplit(line, ' ');
		col_cnt = ft_cntwords(line, ' ');
		*max_column_cnt = (col_cnt > *max_column_cnt) ? col_cnt:*max_column_cnt;
		//ft_putstr("cols: ");ft_putnbr(col_cnt);ft_putstr("\n");
		//ft_putstr("max col: ");
		//ft_putnbr(*max_column_cnt);
		//ft_putstr("\n");
		ft_lstadd(lines, ft_lstnew((void *)column_arr,
							sizeof(char *) * (col_cnt + 1)));
		ft_memdel((void **)&line);
		//ft_putstr("line read complete\n");
		line_cnt++;
	}
	return (line_cnt);
}

void **new_2darray(int rows, int columns, size_t element_size)
{
	void	**array_2d;
	int		row;

	if	(!(array_2d = ft_memalloc(sizeof(void *) * (rows + 1))))
		return (NULL);
	row = 0;
	while (row < rows)
	{
		array_2d[row] = ft_memalloc(element_size * (columns + 1));
		row++;
	}
	return (array_2d);
}

static void convert_list2array(t_list *lines, int **arr2d,
									int rows, int columns)
{
	t_list	*lst;
	t_list	*tmp;
	int		valid_col;
	int		row;
	int		col;

	lst = lines;
	row = 0;
	while (lst != NULL && row < rows)
	{
		tmp = lst->next;
		col = 0;
		valid_col = (((char **)lst->content)[col] == '\0') ? 0 : 1;
		while (col < columns)
		{
			if (valid_col)
			{
				ft_putstr("col: ");ft_putnbr(col);ft_putstr("\n");
				valid_col = (((char **)lst->content)[col + 1] == '\0') ? 0 : 1;
				arr2d[row][col] = ft_atoi(((char **)lst->content)[col]);
			}
			else
			{
				ft_putstr("added ghost pixel\n");
				arr2d[row][col] = -2147483648;
			}
			col++;
		}
		lst = tmp;
		row++;
	}
}

static void array2d_to_object(int **arr2d, t_3d_object *obj, int rows, int cols)
{
	int cur_face_vert;
	int	cur_vert;
	int color;
	int y;
	int x;

	obj->face_cnt = (rows - 1) * (cols - 1);
	obj->vertex_cnt = rows * cols;
	obj->faces_arr = (int *)ft_memalloc(sizeof(int) * obj->face_cnt);
	obj->vertex_ind = (int *)ft_memalloc(sizeof(int) * obj->face_cnt * 4);
	obj->vertices = (t_vec3fc *)ft_memalloc(sizeof(t_vec3fc) * obj->vertex_cnt);
	cur_face_vert = 0;
	y = 0;
	cur_vert = 0;
	while (y < rows)
	{
		x = 0;
		while (x < cols)
		{
			color = (arr2d[y][x] > 5 ) ? 0x00FF0000 : 0x00FFFFFF;
			if (arr2d[y][x] == -2147483648)
				color = 0x4F000000;
			obj->vertices[cur_vert] = vec3fc(x, y, arr2d[y][x], color);
			if (x < cols - 1 && y < rows - 1)
			{
				obj->faces_arr[cur_face_vert/4] = 4;
				obj->vertex_ind[cur_face_vert++] = cur_vert + 1;
				obj->vertex_ind[cur_face_vert++] = cur_vert;
				obj->vertex_ind[cur_face_vert++] = cur_vert + cols;
				obj->vertex_ind[cur_face_vert++] = cur_vert + cols + 1;
			}
			cur_vert++;
			x++;
		}
		y++;
	}
}

void center_obj_originxy(t_3d_object *object)
{
	float max_x;
	float max_y;
	int i;

	max_x = -99;
	max_y = -99;
	i = object->vertex_cnt - 1;
	while (i >= 0)
	{
		if (object->vertices[i].x > max_x)
			max_x = object->vertices[i].x;
		if (object->vertices[i].y > max_y)
			max_y = object->vertices[i].y;
		i--;
	}
	ft_putstr("max x: ");
	ft_putnbr(max_x);
	ft_putstr("\n");

	ft_putstr("max y: ");
	ft_putnbr(max_y);
	ft_putstr("\n");
	i = object->vertex_cnt - 1;
	while (i >= 0)
	{
		object->vertices[i].x -= (max_x / 2.0);
		object->vertices[i].y -= (max_y / 2.0);
		i--;
	}
}

t_3d_object *load_wireframe(char *filename)
{
	t_3d_object	*obj;
	t_list		*lines;
	int			**array2d;
	int			row_cnt;
	int			col_cnt;
	int			file;

	//allocate space for object struct
	if (!(obj = (t_3d_object *)ft_memalloc(sizeof(t_3d_object))))
		return (NULL);

	//open file for reading
	if ((file = open(filename, O_RDONLY)) == -1)
		return (NULL);

	ft_putstr("creating list:\n");
	//load file into a list(rows) of arrays(columns)
	row_cnt = load_into_list(file, &lines, &col_cnt);
	ft_putstr("list created!\n");

	ft_putstr("alloc 2d array:\n");
	//allocate 2d int array
	array2d = (int **)new_2darray(row_cnt, col_cnt, sizeof(int));
	ft_putstr("alloc complete!\n");

	ft_putstr("list to array cpy(str -> int)\n");
	//convert linked list of arrays of strings -> 2d array of ints
	convert_list2array(lines, array2d, row_cnt, col_cnt);
	ft_putstr("copy complete!\n");

	ft_putstr("construct t_3d_object\n");
	//final loading to an object
	array2d_to_object(array2d, obj, row_cnt, col_cnt);
	ft_putstr("construction complete!\n");

	ft_putstr("center origin:\n");
	center_obj_originxy(obj);
	ft_putstr("centering origin complete!\n");
	return (obj);
}

t_3d_object	*testCube()
{
	t_3d_object *obj = (t_3d_object *)ft_memalloc(sizeof(t_3d_object));
	int faces[] = {4, 4, 4, 4, 4, 4};
	int vertex_ind[] = {	0,1,2,3,		//FRONT
							4,5,6,7,		//BACK
							4,5,1,0,		//TOP
							7,6,2,3,		//BOTTOM
							1,5,6,2,		//LEFT
							0,4,7,3			//RIGHT
						 };
	t_vec3fc		 vertices[] = {	{ 50,  50,    50, 0x00FF0000 },
                                    {-50,  50,    50, 0x00FF0000 },
                                    {-50, -50,    50, 0x00FF0000 },
                                    { 50, -50,    50, 0x00FF0000 },
                                    { 50,  50,   -50, 0x00FFFFFF },
                                    {-50,  50,   -50, 0x00FFFFFF },
                                    {-50, -50,   -50, 0x00FFFFFF },
                                    { 50, -50,   -50, 0x00FFFFFF }	};
	obj->faces_arr = (int *)ft_memalloc(sizeof(int) * 6);
	obj->face_cnt = 6;
	obj->vertex_ind = (int *)ft_memalloc(sizeof(int) * 24);
	obj->vertices = (t_vec3fc *)ft_memalloc(sizeof(t_vec3fc) * 8);
	obj->vertex_cnt = 8;
	ft_memcpy(obj->faces_arr, &faces[0], sizeof(int) * 6);
	ft_memcpy(obj->vertex_ind, &vertex_ind[0], sizeof(int) * 24);
	ft_memcpy(obj->vertices, &vertices[0], sizeof(t_vec3fc) * 8);
	obj->pos_vector.position = vec3f(0, 0, -150);
	return (obj);
}

void setup_hooks(t_renderer *renderer)
{
	mlx_hook(renderer->window, 2, 0, key_pressed, renderer);
	mlx_hook(renderer->window, 4, 0, mouse_press_hook, renderer);
	mlx_hook(renderer->window, 5, 0, mouse_release_hook, renderer);
	mlx_hook(renderer->window, 6, 0, mouse_motion_hook, renderer);
	//mlx_loop_hook(renderer->mlx, render_loop, renderer);
	mlx_loop(renderer->mlx);
}

int main(int argc, char **argv)
{
	t_renderer	*fdf_renderer;
	t_scene		*scene1;
	t_3d_object	*obj;

	if (argc != 2)
	{
		ft_putstr("Usage: ./fdf <filename>\n");
		return (0);
	}
	fdf_renderer = new_renderer(render_scene);
	scene1 = new_scene(perspective_projection);
	fdf_renderer->window = mlx_new_window(fdf_renderer->mlx, 1000, 1000, "line drawing");
	fdf_renderer->win_x = 1000;
	fdf_renderer->win_y = 1000;
	fdf_renderer->last_click.x = -99;
	fdf_renderer->last_click.y = -99;

	scene1->cur_frame.width = fdf_renderer->win_x;
	scene1->cur_frame.height = fdf_renderer->win_y;
	scene1->camera = new_camera(vec6f(vec3f(0, 0, 150), vec3f(0.0, 0.0, 0.0)), vec3f(0, 0, 4));
	scene1->origin_point = vec3f(0,0,0);
	scene1->scale = vec3f(1, 1, 1);

	obj = load_wireframe(argv[1]);
	obj->pos_vector.position = vec3f(0, 0, -150);
	add_object(scene1, obj);
	fdf_renderer->scene = scene1;
	setup_hooks(fdf_renderer);
	return (0);
}
