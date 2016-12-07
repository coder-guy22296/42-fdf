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
#include "stdio.h"
#include <stdlib.h>
#include <fcntl.h>

int render_loop(void *param)
{
	t_renderer renderer = *((t_renderer *)param);
	if(renderer.scene)
	{
		//printf("render start!\n");
		renderer.render(renderer, *renderer.scene);
		mlx_clear_window(renderer.mlx, renderer.window);
		//printf("render complete!\n");
	}
	return (0);
}

void object_translation_controls(int keycode, t_renderer *renderer)
{
	t_3d_object *obj;
	t_vec3f		*pos;

	obj = ((t_3d_object *)renderer->scene->objects->content);
	pos = &(obj->pos_vector.position);
	if (keycode == 126)			// UP
		*pos = translate_point(*pos, vec3f(0, 1, 0));
	else if (keycode == 125)	// DOWN
		*pos = translate_point(*pos, vec3f(0, -1, 0));
	else if (keycode == 123)	// LEFT
		*pos = translate_point(*pos, vec3f(-1, 0, 0));
	else if (keycode == 124)	// RIGHT
		*pos = translate_point(*pos, vec3f(1, 0, 0));
}

void object_rotation_controls(int keycode, t_renderer *renderer)
{
	t_3d_object *obj;

	obj = ((t_3d_object *)renderer->scene->objects->content);
	if (keycode == 89)	//NUM_7
		rotate_object(obj, vec3f(0.0, 0.0, 0.1));
	else if (keycode == 92)	//NUM_9
		rotate_object(obj, vec3f(0.0, 0.0, -0.1));
	else if (keycode == 91)	//NUM_8
		rotate_object(obj, vec3f(-0.1, 0.0, 0.0));
	else if (keycode == 87)	//NUM_5
		rotate_object(obj, vec3f(0.1, 0.0, 0.0));
	else if (keycode == 86)	//NUM_4
		rotate_object(obj, vec3f(0.0, 0.1, 0.0));
	else if (keycode == 88)	//NUM_6
		rotate_object(obj, vec3f(0.0, -0.1, 0.0));
}

void camera_translation_controls(int keycode, t_renderer *renderer)
{
	t_vec3f		*pos;
	
	pos = &(renderer->scene->camera->loc.position);
	if (keycode == 13)		//W
		*pos = translate_point(*pos, vec3f(0, 0, -5));
	else if (keycode == 1)	//S
		*pos = translate_point(*pos, vec3f(0, 0, 5));
	else if (keycode == 0)		//A
		*pos = translate_point(*pos, vec3f(-5, 0, 0));
	else if (keycode == 2)	//D
		*pos = translate_point(*pos, vec3f(5, 0, 0));
	else if (keycode == 15)		//R
		*pos = translate_point(*pos, vec3f(0, 5, 0));
	else if (keycode == 3)	//F
		*pos = translate_point(*pos, vec3f(0, -5, 0));
}

int key_pressed(int keycode, void *param)
{
	t_renderer *renderer = (t_renderer *)param;

	object_translation_controls(keycode, renderer);
	object_rotation_controls(keycode, renderer);
	camera_translation_controls(keycode, renderer);
	if (keycode == 53)	//ESC
		exit (1);

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
	return (0);
}

int mouse_motion_hook(int x, int y, void *param)
{
	t_renderer	*renderer;
	t_vec3f		*rotation;

	renderer = (t_renderer *)param;
	rotation = &(renderer->scene->camera->loc.rotation);

	x -= 500;
	y -= 500;
	if (renderer->last_click.x != -99 && renderer->last_click.y != -99)
	{
		*rotation = translate_point(*rotation,
					vec3f( 0.0,-((x - renderer->last_click.x)/1000.0), 0.0));
		*rotation = translate_point(*rotation,
					vec3f(-((y - renderer->last_click.y)/1000.0), 0.0, 0.0));
		renderer->last_click.x = x;
		renderer->last_click.y = y;
	}
	return (0);
}

static int	load_into_list(int fd, t_list **lines, int *column_cnt)
{
	char		*line;
	int 		line_cnt;
	char 		**column_arr;

	line_cnt = 0;
	line = NULL;
	//read line by line
	while (get_next_line(fd, &line) == 1)
	{
		column_arr = ft_strsplit(line, ' ');
		*column_cnt = ft_cntwords(line, ' ');
		ft_lstadd(lines, ft_lstnew((void *)column_arr,
							sizeof(char *) * (*column_cnt + 1)));
		ft_memdel((void **)&line);
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
	int		row;
	int		col;

	lst = lines;
	row = 0;
	while (lst != NULL && row < rows)
	{
		tmp = lst->next;
		col = columns - 1;
		while (col >= 0)
		{
			arr2d[row][col] = ft_atoi(((char **)lst->content)[col]);
			col--;
		}
		lst = tmp;
		row++;
	}
}

static void array2d_to_object(int **arr2d, t_3d_object *obj, int rows, int cols)
{
	int cur_face_vert;
	int	cur_vert;
	int y;
	int x;

	obj->face_cnt = (rows - 1) * (cols - 1);
	obj->vertex_cnt = rows * cols;
	obj->faces_arr = (int *)ft_memalloc(sizeof(int) * obj->face_cnt);
	obj->vertex_ind = (int *)ft_memalloc(sizeof(int) * obj->face_cnt * 4);
	obj->vertices = (t_vec3f *)ft_memalloc(sizeof(t_vec3f) * obj->vertex_cnt);
	cur_face_vert = 0;
	y = 0;
	cur_vert = 0;
	while (y < rows)
	{
		x = 0;
		while (x < cols)
		{
			obj->vertices[cur_vert] = vec3f(x * 10, y * 10, arr2d[y][x] * 2);
			if (x < cols - 2 && y < rows - 2)
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

t_3d_object *load_wireframe(char *filename)
{
	t_3d_object	*obj;
	t_list		*lines;
	int			**array2d;
	int 		row_cnt;
	int 		col_cnt;
	int			file;

	//allocate space for object struct
	if (!(obj = (t_3d_object *)ft_memalloc(sizeof(t_3d_object))))
		return (NULL);

	//open file for reading
	if ((file = open(filename, O_RDONLY)) == -1)
		return (NULL);

	//load file into a list(rows) of arrays(columns)
	row_cnt = load_into_list(file, &lines, &col_cnt);

	//allocate 2d int array
	array2d = (int **)new_2darray(row_cnt, col_cnt, sizeof(int));

	//convert linked list of arrays of strings -> 2d array of ints
	convert_list2array(lines, array2d, row_cnt, col_cnt);

	array2d_to_object(array2d, obj, row_cnt, col_cnt);

	return (obj);
}

int main(int argc, char **argv)
{
	t_renderer *fdf_renderer = new_renderer(render_scene);

	fdf_renderer->last_click.x = -99;
	fdf_renderer->last_click.y = -99;

	if (argc == 2)
		ft_putstr("found a file!!\n");
	t_3d_object *obj = load_wireframe(argv[1]);


	//t_3d_object *obj = (t_3d_object *)ft_memalloc(sizeof(t_3d_object));
	t_scene *scene1 = new_scene(perspective_projection);
	scene1->camera = new_camera(vec6f(vec3f(0, 0, 150), vec3f(0.0, 0.0, 0.0)), vec3f(0, 0, 4));
	scene1->origin_point = vec3f(0,0,0);
	fdf_renderer->window = mlx_new_window(fdf_renderer->mlx, 1000, 1000, "line drawing");

//	int faces[] = {4, 4, 4, 4, 4, 4};
//	int vertex_ind[] = {	0,1,2,3,		//FRONT
//							4,5,6,7,		//BACK
//							4,5,1,0,		//TOP
//							7,6,2,3,		//BOTTOM
//							1,5,6,2,		//LEFT
//							0,4,7,3			//RIGHT
//						 };
//	t_vec3f		 vertices[] = {	{ 50,  50,    50 },
//								{-50,  50,    50 },
//								{-50, -50,    50 },
//								{ 50, -50,    50 },
//								{ 50,  50,   -50 },
//								{-50,  50,   -50 },
//								{-50, -50,   -50 },
//								{ 50, -50,   -50 }	};
	//obj->faces_arr = (int *)ft_memalloc(sizeof(int) * 6);
	//obj->face_cnt = 6;
	//obj->vertex_ind = (int *)ft_memalloc(sizeof(int) * 24);
	//obj->vertices = (t_vec3f *)ft_memalloc(sizeof(t_vec3f) * 8);
	//obj->vertex_cnt = 8;

	//ft_memcpy(obj->faces_arr, &faces[0], sizeof(int) * 6);
	//ft_memcpy(obj->vertex_ind, &vertex_ind[0], sizeof(int) * 24);
	//ft_memcpy(obj->vertices, &vertices[0], sizeof(t_vec3f) * 8);

	//translate(obj, vec3f(0, 0, -150));
	obj->pos_vector.position = vec3f(0, 0, -150);
	//rotate(obj, vec3f(0.0,0.0,0));

	add_object(scene1, obj);

	/*int spacing = 105;

	obj->pos_vector.position = vec3f(0, spacing, -150);
	add_object(scene1, obj);

	obj->pos_vector.position = vec3f(-spacing, spacing, -150);
	add_object(scene1, obj);
	obj->pos_vector.position = vec3f(spacing, 0, -150);
	add_object(scene1, obj);*/
	fdf_renderer->scene = scene1;

	fdf_renderer->render(*fdf_renderer, *(fdf_renderer->scene));
	mlx_hook(fdf_renderer->window, 2, 0, key_pressed, fdf_renderer);/*key_*/
	//mlx_do_key_autorepeatoff(fdf_renderer->mlx);
	mlx_hook(fdf_renderer->window, 4, 0, mouse_press_hook, fdf_renderer);
	mlx_hook(fdf_renderer->window, 5, 0, mouse_release_hook, fdf_renderer);
	mlx_hook(fdf_renderer->window, 6, 0, mouse_motion_hook, fdf_renderer);
	mlx_loop_hook(fdf_renderer->mlx, render_loop, fdf_renderer);
	mlx_loop(fdf_renderer->mlx);
	return (0);
}
