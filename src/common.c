/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 13:47:56 by overetou          #+#    #+#             */
/*   Updated: 2021/03/10 13:58:59 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	*secure_realloc(void *to_realloc, size_t new_size)
{
	void	*temp;

	temp = to_realloc;
	to_realloc = realloc(to_realloc, new_size);
	if (to_realloc)
		return (to_realloc);
	free(temp);
	puts("A realloc failed.");
	exit(0);
}

void	*secure_malloc(const size_t len)
{
	void *res;

	res = malloc(len);
	if (res)
		return (res);
	puts("A malloc failed.");
	exit(0);
}
