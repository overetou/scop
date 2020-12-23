#include "scop.h"

static void	error_check_sdl(char val)
{
	if (val)
		return ;
	puts(SDL_GetError());
	exit(0);
}

void		error_check(char val, const char *msg)
{
	if (val)
		return ;
	puts(msg);
	exit(0);
}

void	render(SDL_Window *win)
{
	glClearColor(0.2, 0.2, 0.2, 1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
}

int			main(int argc, char const *argv[])
{
	SDL_Window		*win;
	SDL_GLContext	glcontext;

	error_check_sdl(SDL_Init(SDL_INIT_VIDEO) >= 0);
	error_check_sdl(SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1) == 0);
	error_check_sdl(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4) == 0);
	error_check_sdl(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4) == 0);
	error_check_sdl(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
	SDL_GL_CONTEXT_PROFILE_CORE) == 0);
	error_check_sdl(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) == 0);
	win = SDL_CreateWindow("Scop", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	error_check_sdl(win != NULL);
	glcontext = SDL_GL_CreateContext(win);
	error_check_sdl(glcontext != NULL);
	SDL_GL_MakeCurrent(win, glcontext);
	error_check_sdl(SDL_GL_SetSwapInterval(1) == 0);
	error_check(glewInit() == GLEW_OK, "glew init failed.");
	process_args(argc, argv);
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
