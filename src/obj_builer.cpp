#include "scop.h"

void	store_if_min_or_max(GLfloat *tab, GLfloat val)
{
	if (val < tab[0])
		tab[0] = val;
	else if (val > tab[1])
		tab[1] = val;
}

void	normalize_obj(t_parsing_storage *storage)
{
	GLfloat			x[3];
	GLfloat			y[3];
	GLfloat			z[3];
	unsigned int	i;

	check_vertices_number(storage);
	assign_max_min_initial_values(x, storage->vert[0]);
	assign_max_min_initial_values(y, storage->vert[1]);
	assign_max_min_initial_values(z, storage->vert[2]);
	i = 3;
	while (i != storage->v_nb)
	{
		store_if_min_or_max(x, storage->vert[i]);
		store_if_min_or_max(y, storage->vert[i + 1]);
		store_if_min_or_max(z, storage->vert[i + 2]);
		i += 3;
	}
	do_for_each(x, y, z, determine_tab_diff);
	i = 0;
	while (i != storage->v_nb)
	{
		affect_diffs(storage->vert + i, x[2], y[2], z[2]);
		i += 3;
	}
}

void	fill_vec3(GLfloat *vec, GLfloat v1, GLfloat v2, GLfloat v3)
{
	vec[0] = v1;
	vec[1] = v2;
	vec[2] = v3;
}

GLfloat	*load_vertex_data_from_storage(t_parsing_storage *store,
GLfloat **color_data, GLfloat **uv_data)
{
	GLfloat			*res;
	unsigned int	i;
	GLfloat			shade[9];

	i = 0;
	fill_vec3(shade, 0.2, 0.2, 0.2);
	fill_vec3(shade + 3, 0.5, 0.5, 0.5);
	fill_vec3(shade + 6, 0.8, 0.8, 0.8);
	res = (GLfloat*)malloc(sizeof(GLfloat) * store->v_ind_size * 3);
	*uv_data = (GLfloat*)malloc(sizeof(GLfloat) * store->v_ind_size * 2);
	*color_data = (GLfloat*)malloc(sizeof(GLfloat) * store->v_ind_size * 3);
	while (i != store->v_ind_size)
	{
		memcpy(res + i * 3, store->vert + (store->v_ind[i] - 1) * 3,
				3 * sizeof(GLfloat));
		memcpy((*color_data) + i * 3, shade + (i % 3) * 3, 3 * sizeof(GLfloat));
		(*uv_data)[i * 2] = store->vert[(store->v_ind[i] - 1) * 3]
							+ store->vert[(store->v_ind[i] - 1) * 3 + 2];
		(*uv_data)[i * 2 + 1] = store->vert[(store->v_ind[i] - 1) * 3 + 1]
							+ store->vert[(store->v_ind[i] - 1) * 3 + 2];
		i++;
	}
	return (res);
}