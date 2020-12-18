#include "scop.h"

void error_check_sdl(char val)
{
	if (val)
		return ;
	puts(SDL_GetError());
	exit(0);
}

void error_check(char val, const char *msg)
{
	if (val)
		return ;
	puts(msg);
	exit(0);
}

static void render(SDL_Window *win)
{
	SDL_Event	event;

	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(win);
	SDL_WaitEvent(&event);
	while (event.key.keysym.sym != SDLK_ESCAPE)
		SDL_WaitEvent(&event);
}

int		main(int argc, char const *argv[])
{
	SDL_Window *win;
	SDL_GLContext glcontext;

	(void)argc;(void)argv;
	error_check_sdl(SDL_Init(SDL_INIT_VIDEO) >= 0);
	win = SDL_CreateWindow(
    "Scop", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 
    SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
	error_check_sdl(win != NULL);
	glcontext = SDL_GL_CreateContext(win);
	error_check_sdl(glcontext != NULL);
	error_check_sdl(SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1) == 0);
	error_check_sdl(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4) == 0);
	error_check_sdl(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4) == 0);
	error_check_sdl(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1));
	error_check_sdl(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24));
	error_check(glewInit() == GLEW_OK, "glew init failed.");
	render(win);
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
