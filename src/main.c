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

void handle_events(char *keep_going)
{
	SDL_Event e;

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_WINDOWEVENT)
		{
			if (e.window.event == SDL_WINDOWEVENT_CLOSE)
			{
				*keep_going = 0;
				break;
			}
		}
		else if (e.type == SDL_KEYDOWN)
		{
			if (e.key.keysym.sym == SDLK_ESCAPE)
				*keep_going = 0;
			break;
		}
	}
}

static void render_frame(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

/*
**handles: 0-raw_vertices_data_handle, 1-vertex_shader_handle, 2-fragment
**_shader_handle, 3-shader_program, 4-vertexArrayObject
**Those handles are handles on the graphic card memory.
*/
void init_render(SDL_Window *win)
{
	//puts("step 1");
	UINT handles[5];
	//puts("step 2");
	char keep_going = 1;

	//puts("step 3");
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	//puts("step 4");
	glClearColor(0.3, 0.3, 0.3, 1);
	//puts("step 5");
	allocate_graphic_side_objects(handles);
	//puts("step 6");
	while (keep_going)
	{
	//puts("step 7");
		render_frame();
	//puts("step 8");
		SDL_GL_SwapWindow(win);
	//puts("step 9");
		handle_events(&keep_going);
	}
		//puts("step 10");
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
	win = SDL_CreateWindow("Scop", SDL_WINDOWPOS_CENTERED,
	SDL_WINDOWPOS_CENTERED,	WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	error_check_sdl(win != NULL);
	glcontext = SDL_GL_CreateContext(win);
	error_check_sdl(glcontext != NULL);
	SDL_GL_MakeCurrent(win, glcontext);
	error_check_sdl(SDL_GL_SetSwapInterval(1) == 0);
	error_check(glewInit() == GLEW_OK, "Failed to load OpenGL Functions.");
	process_args(argc, argv);
	init_render(win);
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}