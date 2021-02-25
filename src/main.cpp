#include "scop.h"
#include "stb_image.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

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

GLfloat	square(GLfloat f)
{
	return (f * f);
}

void	normalize(GLfloat *vec)
{
	GLfloat w = sqrt( vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2] );
    vec[0] /= w;
    vec[1] /= w;
    vec[2] /= w;
}

void	rotation_mat4(GLfloat *mat4, GLfloat radian_angle, GLfloat *axis)
{
	float angle_rad = radian_angle * (3.14159265359/180.0f);
    float c = cos(angle_rad);
    float s = sin(angle_rad);
    float t = 1 - c;
	normalize(axis);

	mat4[0] = c + axis[0] * axis[0] * t;
	mat4[1] = axis[1] * axis[0] * t + axis[2] * s;
	mat4[2] = axis[2] * axis[0] * t-axis[1] * s;
	mat4[3] = 0;
    mat4[4] = axis[0] * axis[1] * t - axis[2] * s;
	mat4[5] = c + axis[1] *axis[1] * t;
	mat4[6] = axis[2] * axis[1] * t+axis[0] * s;
	mat4[7] = 0;
    mat4[8] = axis[0] * axis[2] * t + axis[1] * s;
	mat4[9] = axis[1] * axis[2] * t - axis[0] * s;
	mat4[10] = axis[2] * axis[2] * t + c;
	mat4[11] = 0;
    mat4[12] = 0;
	mat4[13] = 0;
	mat4[14] = 0;
	mat4[15] = 1;
}

static void render_frame(UINT *handles, UINT texture)
{
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;
	GLfloat	vec[] = {1.0f, 0.5f, 0.3f};
	GLfloat	home_model[16];
	GLfloat *home_view;
	GLfloat *home_proj;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, texture);
	//create transformations
	rotation_mat4(home_model, ((float)SDL_GetTicks() / 1000) * 50.0f, vec);
	home_view = translation_mat4(0, 0, -3);
	home_proj = perspective_mat4((float)SCR_WIDTH / (float)SCR_HEIGHT, glm::radians(45.0f), 0.1f, 100.0f);
	// pass them to the shaders (3 different ways)
	glUniformMatrix4fv(glGetUniformLocation(handles[3], "model"), 1, GL_FALSE, home_model);
	glUniformMatrix4fv(glGetUniformLocation(handles[3], "view"), 1, GL_FALSE, home_view);
	glUniformMatrix4fv(glGetUniformLocation(handles[3], "projection"), 1, GL_FALSE, home_proj);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	free(home_view);
	free(home_proj);
	//loop_nb++;//TO_DELETE
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
	glEnable(GL_DEPTH_TEST);
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