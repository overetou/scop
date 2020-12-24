#include "scop.h"
#include "stb_image.h"

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

void handle_events(char *params)
{
	SDL_Event e;

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_WINDOWEVENT)
		{
			if (e.window.event == SDL_WINDOWEVENT_CLOSE)
			{
				*params = 0;
				break;
			}
		}
		else if (e.type == SDL_KEYDOWN)
		{
			if (e.key.keysym.sym == SDLK_ESCAPE)
				*params = 0;
			else if (e.key.keysym.sym == SDLK_w)
			{
				if (params[1])
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				else
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				params[1] = !(params[1]);
			}
			break;
		}
	}
}

static void render_frame(UINT *handles, UINT texture)
{
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;
	glClear(GL_COLOR_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, texture);
	// create transformations
	glm::mat4 model	= glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	glm::mat4 view	= glm::mat4(1.0f);
	glm::mat4 projection	= glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// retrieve the matrix uniform locations
	unsigned int modelLoc = glGetUniformLocation(handles[3], "model");
	unsigned int viewLoc= glGetUniformLocation(handles[3], "view");
	// pass them to the shaders (3 different ways)
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(handles[3], "projection"), 1, GL_FALSE, &projection[0][0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

/*
**handles: 0-raw_vertices_data_handle, 1-vertex_shader_handle, 2-fragment
**_shader_handle, 3-shader_program, 4-vertexArrayObject	5-EBO
**Those handles are handles on the graphic card memory.
**params[0] = keep_going
**params[1] = wireframe mode enabled
*/
void init_render(SDL_Window *win)
{
	UINT handles[6];
	char params[2];
	//int uniform_location;

	params[0] = 1;
	params[1] = 0;
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glClearColor(0.3, 0.3, 0.3, 1);
	allocate_graphic_side_objects(handles);
	/* uniform_location = glGetUniformLocation(handles[3], "fixedColor");
	glUniform4f(uniform_location, 0.7, 0, 0, 1); */
	
	unsigned int texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		puts("Failed to load texture");
	stbi_image_free(data);

	while (params[0])
	{
		render_frame(handles, texture);
		SDL_GL_SwapWindow(win);
		handle_events(params);
	}
	desallocate_graphic_side_objects(handles);
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