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
#include <stdlib.h>
#include <fcntl.h>

int					render_loop(void *param)
{
	t_renderer	*renderer;

	renderer = (t_renderer *)param;
	if (renderer->scene)
	{
		renderer->render(renderer, *renderer->scene);
	}
	return (0);
}

void				object_translation_controls(int keycode,
												t_renderer *renderer)
{
	t_3d_object	*obj;
	t_vec3fc	*pos;

	obj = ((t_3d_object *)renderer->scene->objects->content);
	pos = &(obj->pos_vector.position);
	if (keycode == UP)
		*pos = translate_point(*pos, vec3f(0, 1, 0));
	else if (keycode == DOWN)
		*pos = translate_point(*pos, vec3f(0, -1, 0));
	else if (keycode == LEFT)
		*pos = translate_point(*pos, vec3f(-1, 0, 0));
	else if (keycode == RIGHT)
		*pos = translate_point(*pos, vec3f(1, 0, 0));
}

void				object_rotation_controls(int keycode, t_renderer *renderer)
{
	t_3d_object *obj;

	obj = ((t_3d_object *)renderer->scene->objects->content);
	if (keycode == NUM_7)
		rotate_object(obj, vec3f(0.0, 0.0, 3.14 / 64.0));
	else if (keycode == NUM_9)
		rotate_object(obj, vec3f(0.0, 0.0, -3.14 / 64.0));
	else if (keycode == NUM_8)
		rotate_object(obj, vec3f(-3.14 / 64.0, 0.0, 0.0));
	else if (keycode == NUM_5)
		rotate_object(obj, vec3f(3.14 / 64.0, 0.0, 0.0));
	else if (keycode == NUM_4)
		rotate_object(obj, vec3f(0.0, 3.14 / 64.0, 0.0));
	else if (keycode == NUM_6)
		rotate_object(obj, vec3f(0.0, -3.14 / 64.0, 0.0));
}

void				camera_translation_controls(int keycode,
												t_renderer *renderer)
{
	t_vec3fc	*pos;

	pos = &(renderer->scene->camera->loc.position);
	if (keycode == W)
		*pos = translate_point(*pos, vec3f(0, 0, -5));
	else if (keycode == S)
		*pos = translate_point(*pos, vec3f(0, 0, 5));
	else if (keycode == A)
		*pos = translate_point(*pos, vec3f(-5, 0, 0));
	else if (keycode == D)
		*pos = translate_point(*pos, vec3f(5, 0, 0));
	else if (keycode == R)
		*pos = translate_point(*pos, vec3f(0, 5, 0));
	else if (keycode == F)
		*pos = translate_point(*pos, vec3f(0, -5, 0));
}

int					key_pressed(int keycode, void *param)
{
	t_renderer	*renderer;

	renderer = (t_renderer *)param;
	object_translation_controls(keycode, renderer);
	object_rotation_controls(keycode, renderer);
	camera_translation_controls(keycode, renderer);
	if (keycode == NUM_0)
	{
		if (renderer->scene->projection_method == perspective_projection)
			renderer->scene->projection_method = orthographic_projection;
		else
			renderer->scene->projection_method = perspective_projection;
	}
	if (keycode == NUM_3)
		renderer->scene->scale = translate_point(renderer->scene->scale,
												vec3f(0.2, 0.2, 0.2));
	if (keycode == NUM_DOT)
		renderer->scene->scale = translate_point(renderer->scene->scale,
												vec3f(-0.2, -0.2, -0.2));
	if (keycode == ESC)
		exit(1);
	return (0);
}

int					mouse_press_hook(int button, int x, int y, void *param)
{
	t_renderer	*renderer;

	renderer = (t_renderer *)param;
	x -= 500;
	y -= 500;
	if (button && x && y)
		ft_putchar('\0');
	renderer->last_click.x = x;
	renderer->last_click.y = y;
	return (0);
}

int					mouse_release_hook(int button, int x, int y, void *param)
{
	t_renderer	*renderer;

	renderer = (t_renderer *)param;
	x -= 500;
	y -= 500;
	if (button)
		ft_putchar('\0');
	renderer->last_click.x = -99;
	renderer->last_click.y = -99;
	return (0);
}

int					mouse_motion_hook(int x, int y, void *param)
{
	t_renderer	*renderer;
	t_vec3fc	*rotation;

	renderer = (t_renderer *)param;
	rotation = &(renderer->scene->camera->loc.rotation);
	x -= 500;
	y -= 500;
	if (renderer->last_click.x != -99 && renderer->last_click.y != -99)
	{
		if (renderer->scene->projection_method == orthographic_projection)
			*rotation = translate_point(*rotation,
					vec3f(0.0, ((x - renderer->last_click.x) / 1000.0), 0.0));
		else
			*rotation = translate_point(*rotation,
					vec3f(0.0, -((x - renderer->last_click.x) / 1000.0), 0.0));
		*rotation = translate_point(*rotation,
					vec3f(-((y - renderer->last_click.y) / 1000.0), 0.0, 0.0));
		renderer->last_click.x = x;
		renderer->last_click.y = y;
	}
	return (0);
}

static int			load_into_list(int fd, t_list **lines, int *max_column_cnt)
{
	char		*line;
	int			line_cnt;
	int			col_cnt;
	char		**column_arr;

	line_cnt = 0;
	*max_column_cnt = -99;
	line = NULL;
	while (get_next_line(fd, &line) == 1)
	{
		column_arr = ft_strsplit(line, ' ');
		col_cnt = ft_cntwords(line, ' ');
		*max_column_cnt =
				(col_cnt > *max_column_cnt) ? col_cnt : *max_column_cnt;
		ft_lstadd(lines, ft_lstnew((void *)column_arr,
							sizeof(char *) * (col_cnt + 1)));
		ft_memdel((void **)&line);
		line_cnt++;
	}
	return (line_cnt);
}

void				**new_2darray(int rows, int columns, size_t element_size)
{
	void	**array_2d;
	int		row;

	if (!(array_2d = ft_memalloc(sizeof(void *) * (rows + 1))))
		return (NULL);
	row = 0;
	while (row < rows)
	{
		array_2d[row] = ft_memalloc(element_size * (columns + 1));
		row++;
	}
	return (array_2d);
}

static void			convert_list2array(t_list *lines, int **arr2d,
									int rows, int columns)
{
	t_list	*tmp;
	int		valid_col;
	int		row;
	int		col;

	row = 0;
	while (lines != NULL && row < rows)
	{
		tmp = lines->next;
		col = 0;
		valid_col = (((char **)lines->content)[col] == 0) ? 0 : 1;
		while (col < columns)
		{
			if (valid_col)
			{
				valid_col = (((char **)lines->content)[col + 1] == 0) ? 0 : 1;
				arr2d[row][col] = ft_atoi(((char **)lines->content)[col]);
			}
			else
				arr2d[row][col] = -2147483648;
			col++;
		}
		lines = tmp;
		row++;
	}
}

t_3d_object			*new_3dobject(int faces, int verticies, int verts_per_face)
{
	t_3d_object *obj;

	if (!(obj = (t_3d_object *)ft_memalloc(sizeof(t_3d_object))))
		return (NULL);
	obj->face_cnt = faces;
	obj->vertex_cnt = verticies;
	if (!(obj->faces_arr = (int *)ft_memalloc(sizeof(int) * obj->face_cnt))
		|| !(obj->vertex_ind = (int *)ft_memalloc(sizeof(int)
													* obj->face_cnt
													* verts_per_face))
		|| !(obj->vertices = (t_vec3fc *)ft_memalloc(sizeof(t_vec3fc)
														* obj->vertex_cnt)))
		return (NULL);
	return (obj);
}

static void			find_min_max_z(t_3d_object *obj, int x, int y, int z)
{
	if ((z > obj->z_max || (x == 0 && y == 0))
		&& z != -2147483648)
		obj->z_max = z;
	if ((z < obj->z_min || (x == 0 && y == 0))
		&& z != -2147483648)
		obj->z_min = z;
}

static void			array2d_to_object(int **arr2d, t_3d_object *obj, int rows,
											int cols)
{
	int	cur_face_vert;
	int	cur_vert;
	int	color;
	int	y;
	int	x;

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
			if (arr2d[y][x] == -2147483648)
				color = 0x4F000000;
			find_min_max_z(obj, x, y, arr2d[y][x]);
			obj->vertices[cur_vert] = vec3fc(x, y, arr2d[y][x] * 1.0f, color);
			if (x < cols - 1 && y < rows - 1)
			{
				obj->faces_arr[cur_face_vert / 4] = 4;
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

void				center_obj_originxy(t_3d_object *object)
{
	float	max_x;
	float	max_y;
	int		i;

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
	i = object->vertex_cnt - 1;
	while (i >= 0)
	{
		object->vertices[i].x -= (max_x / 2.0);
		object->vertices[i].y -= (max_y / 2.0);
		i--;
	}
}

void				apply_z_gradient(t_3d_object *obj, int color_low,
										int color_high)
{
	int		color;
	float	magnitude;
	float	percent;
	int		i;

	magnitude = fabsf(obj->z_max - obj->z_min);
	i = obj->vertex_cnt - 1;
	while (i >= 0)
	{
		percent = (obj->vertices[i].z + fabsf(obj->z_min)) / magnitude;
		color = blend(color_low, color_high, percent);
		obj->vertices[i].color = color;
		i--;
	}
}

t_3d_object			*load_wireframe(char *filename)
{
	t_3d_object	*obj;
	t_list		*lines;
	int			**array2d;
	t_vec2i		row_col;
	int			file;

	if (!(obj = (t_3d_object *)ft_memalloc(sizeof(t_3d_object))))
		return (NULL);
	if ((file = open(filename, O_RDONLY)) == -1)
		return (NULL);
	row_col.y = load_into_list(file, &lines, &row_col.x);
	array2d = (int **)new_2darray(row_col.y, row_col.x, sizeof(int));
	convert_list2array(lines, array2d, row_col.y, row_col.x);
	array2d_to_object(array2d, obj, row_col.y, row_col.x);
	center_obj_originxy(obj);
	apply_z_gradient(obj, 0x00FFFFFF, 0x00FF0000);
	obj->pos_vector.position = vec3f(0, 0, -2 * obj->z_max);
	return (obj);
}

void				setup_hooks(t_renderer *renderer)
{
	mlx_hook(renderer->window, 2, 0, key_pressed, renderer);
	mlx_hook(renderer->window, 4, 0, mouse_press_hook, renderer);
	mlx_hook(renderer->window, 5, 0, mouse_release_hook, renderer);
	mlx_hook(renderer->window, 6, 0, mouse_motion_hook, renderer);
	mlx_loop_hook(renderer->mlx, render_loop, renderer);
	mlx_loop(renderer->mlx);
}

int					main(int argc, char **argv)
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
	add_window(fdf_renderer, 1000, 1000, "cyildiri's fdf");
	scene1 = new_scene(perspective_projection, 1000, 1000);
	scene1->camera = new_camera(vec6f(vec3f(0, 0, 150),
									vec3f(0.0, 0.0, 0.0)), vec3f(0, 0, 4));
	obj = load_wireframe(argv[1]);
	add_object(scene1, obj);
	fdf_renderer->scene = scene1;
	setup_hooks(fdf_renderer);
	return (0);
}
