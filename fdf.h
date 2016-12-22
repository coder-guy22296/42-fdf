/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyildiri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/08 18:57:43 by cyildiri          #+#    #+#             */
/*   Updated: 2016/11/08 18:57:45 by cyildiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H
# include "mlx.h"
# include "libgraphics.h"

void		object_translation_controls(int keycode, t_renderer *renderer);
void		object_rotation_controls(int keycode, t_renderer *renderer);
void		camera_translation_controls(int keycode, t_renderer *renderer);
int			key_pressed(int keycode, void *param);
int			mouse_press_hook(int button, int x, int y, void *param);
int			mouse_release_hook(int button, int x, int y, void *param);
int			mouse_motion_hook(int x, int y, void *param);
t_3d_object	*new_fdf_wireframe(int faces, int verticies, int verts_per_face);
t_3d_object	*load_wireframe(char *filename);
int			load_into_list(int fd, t_list **lines, int *max_column_cnt);
void		convert_list2array(t_list *lines, int **arr2d, int rows,
								int columns);
t_3d_object	*array2d_to_object(int **arr2d, int rows, int cols);
void		center_obj_originxy(t_3d_object *object);
void		apply_z_gradient(t_3d_object *obj, int color_low, int color_high);
void		**new_2darray(int rows, int columns, size_t element_size);
int			render_loop(void *param);
void		setup_hooks(t_renderer *renderer);

#endif
