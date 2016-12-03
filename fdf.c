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
#include <math.h>

int render_loop(void *param)
{
	t_renderer renderer = *((t_renderer *)param);
	if(renderer.scene)
	{
		printf("render start!\n");
		renderer.render(renderer, *renderer.scene);
		mlx_clear_window(renderer.mlx, renderer.window);
		printf("render complete!\n");
	}
	return (0);
}

int key_pressed(int keycode, void *param)
{
	t_renderer *renderer = (t_renderer *)param;
	t_3d_object *obj = ((t_3d_object *)renderer->scene->objects->content);
	if (keycode == 13)		//W
	{
		obj->pos_vector.position = translate_point(obj->pos_vector.position, vec3f(0, 0, -1));
	}
	else if (keycode == 1)	//S
	{
		obj->pos_vector.position = translate_point(obj->pos_vector.position, vec3f(0, 0, 1));
	}
	else if (keycode == 126)	//UP
	{
		obj->pos_vector.position = translate_point(obj->pos_vector.position, vec3f(0, 1, 0));
	}
	else if (keycode == 125)	//DOWN
	{
		obj->pos_vector.position = translate_point(obj->pos_vector.position, vec3f(0, -1, 0));
	}
	else if (keycode == 123)	//LEFT
	{
		obj->pos_vector.position = translate_point(obj->pos_vector.position, vec3f(-1, 0, 0));
	}
	else if (keycode == 124)	//RIGHT
	{
		obj->pos_vector.position = translate_point(obj->pos_vector.position, vec3f(1, 0, 0));
	}
	else if (keycode == 0)	//A
	{
		//translate(obj, vec3f(-1, 0, 0));
		rotate(obj, vec3f(0.0, 0.0, 0.1));
	}
	else if (keycode == 2)	//D
	{
		//translate(obj, vec3f(1, 0, 0));
		rotate(obj, vec3f(0.0, 0.0, -0.1));
	}
	else if (keycode == 91)	//NUM_8
	{
		rotate(obj, vec3f(-0.1, 0.0, 0.0));
	}
	else if (keycode == 87)	//NUM_5
	{
		rotate(obj, vec3f(0.1, 0.0, 0.0));
	}
	else if (keycode == 86)	//NUM_4
	{
		rotate(obj, vec3f(0.0, 0.1, 0.0));
	}
	else if (keycode == 88)	//NUM_6
	{
		rotate(obj, vec3f(0.0, -0.1, 0.0));
	}

	printf("key pressed: %d\n", keycode);
	render_loop(renderer);
	return (0);
}

int main()
{
	t_renderer *fdf_renderer = new_renderer(render_scene);
	
	t_3d_object *obj = (t_3d_object *)ft_memalloc(sizeof(t_3d_object));
	t_scene *scene1 = new_scene(perspective_projection);
	scene1->camera = new_camera(vec6f(vec3f(0, 0, 0), vec3f(0.0, 0.0, 0.0)), vec3f(0, 0, 1));
	scene1->origin_point = vec3f(0,0,0);
	fdf_renderer->window = mlx_new_window(fdf_renderer->mlx, 1000, 1000, "line drawing");

	int faces[] = {4, 4, 4, 4, 4, 4};
	int vertex_ind[] = {	0,1,2,3,		//FRONT
							4,5,6,7,		//BACK
							4,5,1,0,		//TOP
							7,6,2,3,		//BOTTOM
							1,5,6,2,		//LEFT
							0,4,7,3			//RIGHT
						 };
	t_vec3f		 vertices[] = {	{ 50,  50,    50 },
								{-50,  50,    50 },
								{-50, -50,    50 },
								{ 50, -50,    50 },
								{ 50,  50,   -50 },
								{-50,  50,   -50 },
								{-50, -50,   -50 },
								{ 50, -50,   -50 }	};

	obj->faces_arr = (int *)ft_memalloc(sizeof(int) * 6);
	obj->face_cnt = 6;
	obj->vertex_ind = (int *)ft_memalloc(sizeof(int) * 24);
	obj->vertices = (t_vec3f *)ft_memalloc(sizeof(t_vec3f) * 8);
	obj->vertex_cnt = 8;

	ft_memcpy(obj->faces_arr, &faces[0], sizeof(int) * 6);
	ft_memcpy(obj->vertex_ind, &vertex_ind[0], sizeof(int) * 24);
	ft_memcpy(obj->vertices, &vertices[0], sizeof(t_vec3f) * 8);

	//translate(obj, vec3f(0, 0, -150));
	obj->pos_vector.position = vec3f(0, 0, -150);
	//rotate(obj, vec3f(0.0,0.0,0));

	add_object(scene1, obj);
	fdf_renderer->scene = scene1;

	fdf_renderer->render(*fdf_renderer, *(fdf_renderer->scene));
	mlx_key_hook(fdf_renderer->window, key_pressed, fdf_renderer);
	mlx_do_key_autorepeaton(fdf_renderer->mlx);
	//mlx_loop_hook(fdf_renderer->mlx, render_loop, fdf_renderer);
	mlx_loop(fdf_renderer->mlx);
	return (0);
}
