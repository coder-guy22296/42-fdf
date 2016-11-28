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

#include "minilibx_macos/mlx.h"
#include "libg/libgraphics.h"
#include "libft/includes/libft.h"
#include "stdio.h"
#include <stdlib.h>
#include <math.h>

int main()
{
	t_renderer fdf_renderer;
	
	t_3d_object obj;
	t_3d_scene scene;

	fdf_renderer = renderer(render_scene);
	fdf_renderer.window = mlx_new_window(fdf_renderer.mlx, 1000, 1000, "line drawing");

	int faces[] = {4, 4, 4, 4, 4, 4};
	int vertex_ind[] = {	0,1,2,3,		//FRONT
							4,5,6,7,		//BACK
							4,5,1,0,		//TOP
							7,6,2,3,		//BOTTOM
							1,5,6,2,		//LEFT
							0,4,7,3			//RIGHT
						 };
	t_3d_point vertices[] = {	{ 50,  50,  50 },
								{-50,  50,  50 },
								{-50, -50,  50 },
								{ 50, -50,  50 },
								{ 50,  50,   -50 },
								{-50,  50,   -50 },
								{-50, -50,   -50 },
								{ 50, -50,   -50 }	};

	obj.faces_arr = &faces[0];
	obj.face_cnt = 6;
	obj.vertex_ind = &vertex_ind[0];
	obj.vertices = &vertices[0];

	t_3d_vector pos = {0, -0, -53, 0.0, 0.0, 0.0};
	//t_3d_vector pos = {55, -55, -53, 0.0, 0.0, 0.0};
	//t_3d_vector pos = {0, 0, -130, 0.0, 0.0, 0.0};
	obj.pos_vector = pos;

	add_object(&scene, &obj);
	fdf_renderer.scene = &scene;
	fdf_renderer.render(fdf_renderer, *(fdf_renderer.scene));

	mlx_key_hook(fdf_renderer.window, key_pressed, &fdf_renderer);
	//mlx_loop_hook(renderer.mlx, render_loop, &renderer);
	mlx_loop(fdf_renderer.mlx);
	return (0);
}
