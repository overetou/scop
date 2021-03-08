#include "scop.h"

static int	str_len(const char *s)
{
	int i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

/*
**Returns true if string are the same.
*/
static char	str_cmp_n(const char *s1, const char *s2, int len1, int len2)
{
	int i;

	if (len1 != len2)
		return (0);
	i = 0;
	while (i != len1)
	{
		if (s1[i] != s2[i])
			return(0);
		i++;
	}
	return (1);
}

void		check_and_fill_args(const char *argv[], t_master *m)
{
	size_t	len;

	len = str_len(argv[1]);
	error_check(argv[1][len - 1] == 'j' &&
	argv[1][len - 2] == 'b' && argv[1][len - 3] == 'o' &&
	argv[1][len - 4] == '.', "Expected a file with the extension '.obj' as first argument.");
	m->obj_file_path = argv[1];
}

void		process_args(const int argc, const char *argv[], t_master *m)
{
	int len;

	if (argc == 2)
	{
		len = str_len(argv[1]);
		if (str_cmp_n("specs", argv[1], 5, len))
			printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
		else if (str_cmp_n("vertex_shader_source", argv[1], 20, len))
			puts("Vertex shader source:\n" VERTEX_SHADER_SOURCE);
		else if (str_cmp_n("fragment_shader_source", argv[1], 22, len))
			puts("Fragment shader source:\n" FRAGMENT_SHADER_SOURCE);
		else
			puts("No match for given argument." USAGE);
		exit(0);
	}
	error_check(argc == 3, USAGE);
	check_and_fill_args(argv, m);
}
