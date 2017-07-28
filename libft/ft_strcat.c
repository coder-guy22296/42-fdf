/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyildiri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/25 12:24:43 by cyildiri          #+#    #+#             */
/*   Updated: 2016/09/26 13:51:02 by cyildiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strcat(char *s1, const char *s2)
{
	int	index;
	int i;

	index = 0;
	while (s1[index] != '\0')
		index++;
	i = 0;
	while (s2[i] != '\0')
	{
		s1[index] = s2[i];
		index++;
		i++;
	}
	s1[index] = '\0';
	return (s1);
}
