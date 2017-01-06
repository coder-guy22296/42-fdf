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

#include "fdf.h"

/*
**	allocates and zeros a 2d array of a specified element
*/

void	**new_2darray(int rows, int columns, size_t element_size)
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

/*
**	Event Hook for rendering
*/

int		render_loop(void *param)
{
	t_renderer	*renderer;

	renderer = (t_renderer *)param;
	if (renderer->scene)
	{
		renderer->render(renderer, *renderer->scene);
	}
	return (0);
}

/*
**	Set all the event hooks
*/

void	setup_hooks(t_renderer *renderer)
{
	mlx_hook(renderer->window, 2, 0, key_pressed, renderer);
	mlx_hook(renderer->window, 4, 0, mouse_press_hook, renderer);
	mlx_hook(renderer->window, 5, 0, mouse_release_hook, renderer);
	mlx_hook(renderer->window, 6, 0, mouse_motion_hook, renderer);
	mlx_loop_hook(renderer->mlx, render_loop, renderer);
	mlx_loop(renderer->mlx);
}

/*
**	FDF Entry Point
*/

int		main(int argc, char **argv)
{
	t_renderer	*fdf_renderer;
	t_scene		*scene1;
	t_3d_object	*obj;
	t_3d_object	*obj2;

	if (argc != 2 && argc != 3)
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
	obj2 = load_wireframe(argv[2]);
	obj2->pos_vector.position = vec3f(100, 0, -150);
	add_object(scene1, obj2);
	fdf_renderer->scene = scene1;
	setup_hooks(fdf_renderer);
	return (0);
}
