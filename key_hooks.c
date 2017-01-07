/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyildiri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/22 09:17:08 by cyildiri          #+#    #+#             */
/*   Updated: 2016/12/22 09:17:10 by cyildiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <stdlib.h>

/*
**	Controls for moving the object in 3d space
**	only x and y movement controls for now
*/

void	object_translation_controls(int keycode, t_renderer *renderer)
{
	t_3d_object	*obj;
	t_vec3fc	*pos;

	obj = (renderer->scene->active_obj)
		? ((t_3d_object *)renderer->scene->objects->content)
		: ((t_3d_object *)renderer->scene->objects->next->content);
	pos = &(obj->pos_vector.position);
	if (keycode == UP)
		*pos = translate_point(*pos, vec3f(0, 5, 0));
	else if (keycode == DOWN)
		*pos = translate_point(*pos, vec3f(0, -5, 0));
	else if (keycode == LEFT)
		*pos = translate_point(*pos, vec3f(-5, 0, 0));
	else if (keycode == RIGHT)
		*pos = translate_point(*pos, vec3f(5, 0, 0));
	else if (keycode == PAGE_UP)
		*pos = translate_point(*pos, vec3f(0, 0, -5));
	else if (keycode == PAGE_DOWN)
		*pos = translate_point(*pos, vec3f(0, 0, 5));
}

/*
**	Controls for rotating an object about its origin
*/

void	object_rotation_controls(int keycode, t_renderer *renderer)
{
	t_3d_object *obj;

	obj = (renderer->scene->active_obj)
		? ((t_3d_object *)renderer->scene->objects->content)
		: ((t_3d_object *)renderer->scene->objects->next->content);
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

/*
**	Controls for moving the camera in full 3d space
*/

void	camera_translation_controls(int keycode, t_renderer *renderer)
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

/*
**	Hook that handles key presses and delegates to functions above
*/

int		key_pressed(int keycode, void *param)
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
	if (keycode == NUM_1 && renderer->scene->active_obj != -99)
		renderer->scene->active_obj = (renderer->scene->active_obj) ? 0 : 1;
	if (keycode == NUM_3)
		renderer->scene->scale = translate_point(renderer->scene->scale,
													vec3f(1, 1, 1));
	if (keycode == NUM_DOT)
		renderer->scene->scale = translate_point(renderer->scene->scale,
													vec3f(-1, -1, -1));
	if (keycode == ESC)
		exit(1);
	return (0);
}
