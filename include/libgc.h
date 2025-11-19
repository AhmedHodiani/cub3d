/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libgc.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 16:02:22 by ataher            #+#    #+#             */
/*   Updated: 2025/06/29 16:14:17 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBGC_H
# define LIBGC_H

# include <stddef.h>
# include <stdlib.h>

# define GC_INITIAL_CAPACITY	16
# define GC_GROWTH_FACTOR		2

typedef struct s_gc_context
{
	void	**pointers;
	size_t	count;
	size_t	capacity;
}	t_gc_context;

int				gc_init(void);
void			*gc_malloc(size_t size);
int				gc_free(void *ptr);
void			gc_clean(void);
size_t			gc_count(void);

/* Internal helper functions */
t_gc_context	*gc_get_context(void);
t_gc_context	**gc_get_context_ptr(void);
int				gc_resize(t_gc_context *gc);
int				gc_track_pointer(t_gc_context *gc, void *ptr);
int				gc_untrack_pointer(t_gc_context *gc, void *ptr);
void			*ft_realloc(void *ptr, size_t old_size, size_t new_size);

#endif
