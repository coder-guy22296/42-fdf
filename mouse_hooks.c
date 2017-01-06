/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hooks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyildiri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/22 09:17:25 by cyildiri          #+#    #+#             */
/*   Updated: 2016/12/22 09:17:27 by cyildiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/*
**	Hooks for mouse related events
**	these events control camera rotation about x and y
*/

int		mouse_press_hook(int button, int x, int y, void *param)
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

int		mouse_release_hook(int button, int x, int y, void *param)
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

int		mouse_motion_hook(int x, int y, void *param)
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
