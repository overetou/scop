#include "scop.h"

void	try_four_face_parsing(const char *content,
unsigned int *pos, t_parsing_storage *storage)
{
	int	temp[4];

	if (sscanf(content + *pos, "%d %d %d %d\n", temp, temp + 1,
	temp + 2, temp + 3) != 4)
	{
		puts("Problem while scanning a face of 4 members.");
		exit(0);
	}
	storage->v_ind = (unsigned int*)realloc(storage->v_ind,
	(storage->v_ind_size + 6) * sizeof(int));
	storage->v_ind[storage->v_ind_size] = temp[0];
	storage->v_ind[storage->v_ind_size + 1] = temp[1];
	storage->v_ind[storage->v_ind_size + 2] = temp[2];
	storage->v_ind[storage->v_ind_size + 3] = temp[2];
	storage->v_ind[storage->v_ind_size + 4] = temp[3];
	storage->v_ind[storage->v_ind_size + 5] = temp[0];
	storage->v_ind_size += 6;
}

void	try_three_face_parsing(const char *content,
unsigned int *pos, t_parsing_storage *storage)
{
	int	item_parsed_number;

	storage->v_ind = (unsigned int*)realloc(storage->v_ind,
	(storage->v_ind_size + 3) * sizeof(int));
	item_parsed_number = sscanf(content + *pos, "%d %d %d\n",
	storage->v_ind + storage->v_ind_size,
	storage->v_ind + storage->v_ind_size + 1,
	storage->v_ind + storage->v_ind_size + 2);
	if (item_parsed_number != 3)
	{
		puts("Problem while scanning a face of 3 members.");
		exit(0);
	}
	storage->v_ind_size += 3;
}

int				count_sep_till_endline(const char *s, int *till_next_line)
{
	int sep_number;
	int	i;

	i = 0;
	sep_number = 0;
	while (s[i] && s[i] != '\n')
	{
		while (s[i] != ' ')
		{
			i++;
			if (s[i] == '\0' || s[i] == '\n')
			{
				*till_next_line = i;
				return (sep_number + 1);
			}
		}
		if (i)
			sep_number++;
		while (s[i] == ' ')
			i++;
	}
	*till_next_line = i + 1;
	return (sep_number + 1);
}

int				size_till_endline(const char *str)
{
	int result;

	result = 0;
	while (str[result] != '\n' && str[result])
		result++;
	return (result);
}