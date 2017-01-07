/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_operations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyildiri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/22 09:17:41 by cyildiri          #+#    #+#             */
/*   Updated: 2016/12/22 09:17:42 by cyildiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/*
**	Centers the origin of a 3d model in the x and y axis
*/

void	center_obj_originxy(t_3d_object *object)
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

/*
**	Applies a color gradient to the points based on altitude
*/

void	apply_z_gradient(t_3d_object *obj, int color_low, int color_high)
{
	int		color;
	float	magnitude;
	float	percent;
	int		i;

	magnitude = fabsf(obj->z_max - obj->z_min);
	i = obj->vertex_cnt - 1;
	while (i >= 0)
	{
		percent = (magnitude != 0.000000)
				? (obj->vertices[i].z + fabsf(obj->z_min)) / magnitude : 0.000;
		color = blend(color_low, color_high, percent);
		obj->vertices[i].color = (obj->vertices[i].z != -2147483648)
								? color : 0x4F000000;
		i--;
	}
}
