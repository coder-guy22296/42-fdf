/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wireframe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyildiri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/22 09:17:57 by cyildiri          #+#    #+#             */
/*   Updated: 2016/12/22 09:17:59 by cyildiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <fcntl.h>

/*
**	3d object constructor for a wireframe of specified vertices per face
*/

t_3d_object	*new_fdf_wireframe(int faces, int verticies, int verts_per_face)
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

/*
**	loads fdf wireframe from file into a 3d object (Main FDF Loading function)
*/

t_3d_object	*load_wireframe(char *filename)
{
	t_3d_object	*obj;
	t_list		*lines;
	int			**array2d;
	t_vec2i		row_col;
	int			file;

	if ((file = open(filename, O_RDONLY)) == -1)
		return (NULL);
	row_col.y = load_into_list(file, &lines, &row_col.x);
	array2d = (int **)new_2darray(row_col.y, row_col.x, sizeof(int));
	convert_list2array(lines, array2d, row_col.y, row_col.x);
	if (!(obj = array2d_to_object(array2d, row_col.y, row_col.x)))
		return (NULL);
	center_obj_originxy(obj);
	apply_z_gradient(obj, 0x00FFFFFF, 0x00FF0000);
	obj->pos_vector.position = vec3f(0, 0, -2 * obj->z_max);
	return (obj);
}
