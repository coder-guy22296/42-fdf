/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_map_loading.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyildiri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/22 09:16:46 by cyildiri          #+#    #+#             */
/*   Updated: 2016/12/22 09:16:51 by cyildiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/*
**	Load an fdf file into a linked list of arrays of strings
*/

int			load_into_list(int fd, t_list **lines, int *max_column_cnt)
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

/*
**	Converts a linked list of arrays of strings into a 2d array of integers
*/

void		convert_list2array(t_list *lines, int **arr2d,
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

/*
**	Sets the minimum and maximum z values for a specified object
*/

static void	set_min_max_z(t_3d_object *obj, int x, int y, int z)
{
	if ((z > obj->z_max || (x == 0 && y == 0))
		&& z != -2147483648)
		obj->z_max = z;
	if ((z < obj->z_min || (x == 0 && y == 0))
		&& z != -2147483648)
		obj->z_min = z;
}

/*
**	sets the line draw order for a square in the wireframe
*/

static void	set_object_vert_indices(t_3d_object *obj, int cur_vert,
											int cols, int *cur_face_vert)
{
	obj->faces_arr[*cur_face_vert / 4] = 4;
	obj->vertex_ind[(*cur_face_vert)++] = cur_vert + 1;
	obj->vertex_ind[(*cur_face_vert)++] = cur_vert;
	obj->vertex_ind[(*cur_face_vert)++] = cur_vert + cols;
	obj->vertex_ind[(*cur_face_vert)++] = cur_vert + cols + 1;
}

/*
**	converts 2d int array into my 3d model struct
*/

t_3d_object	*array2d_to_object(int **arr2d, int rows, int cols)
{
	t_3d_object	*obj;
	int			cur_face_vert;
	int			cur_vert;
	int			y;
	int			x;

	obj = new_fdf_wireframe((rows - 1) * (cols - 1), rows * cols, 4);
	cur_face_vert = 0;
	cur_vert = 0;
	y = 0;
	while (y < rows)
	{
		x = 0;
		while (x < cols)
		{
			set_min_max_z(obj, x, y, arr2d[y][x]);
			obj->vertices[cur_vert] = vec3fc(x, y, arr2d[y][x], 0x0);
			if (x < cols - 1 && y < rows - 1)
				set_object_vert_indices(obj, cur_vert, cols, &cur_face_vert);
			cur_vert++;
			x++;
		}
		y++;
	}
	return (obj);
}
