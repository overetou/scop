
#ifndef SCOP_H
# define SCOP_H

# include <SDL2/SDL.h>
# include <GL/glew.h>
# include <math.h>
# include <stdio.h>

# define UINT unsigned int
# define WINDOW_HEIGHT 800
# define WINDOW_WIDTH 600
# define USAGE "\nUsage:\t./scop <file>.obj <file>.bmp\n\tOr\n\t./scop [specs"\
" / vertex_shader_source / fragment_shader_source]"
# define VERTEX_SHADER_SOURCE "#version 400\n"\
"layout(location = 0) in vec3 vertexPosition_modelspace;"\
"layout(location = 1) in vec3 vertexColor;"\
"out vec3 fragmentColor;"\
"uniform mat4 MVP;"\
"void main(){"\
"	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);"\
	"fragmentColor = vertexColor;"\
"}"
# define FRAGMENT_SHADER_SOURCE "#version 400\n"\
"in vec3 fragmentColor;"\
"out vec3 color;"\
"void main(){"\
"	color = fragmentColor;"\
''}"

static const double PI = 3.14159265358979323846;

typedef struct Vertex
{
  float Position[4];
  float Color[4];
} Vertex;

typedef struct Matrix
{
  float m[16];
} Matrix;

extern const Matrix IDENTITY_MATRIX;

float Cotangent(float angle);
float DegreesToRadians(float degrees);
float RadiansToDegrees(float radians);

Matrix MultiplyMatrices(const Matrix* m1, const Matrix* m2);
void RotateAboutX(Matrix* m, float angle);
void RotateAboutY(Matrix* m, float angle);
void RotateAboutZ(Matrix* m, float angle);
void ScaleMatrix(Matrix* m, float x, float y, float z);
void TranslateMatrix(Matrix* m, float x, float y, float z);

Matrix CreateProjectionMatrix(
  float fovy,
  float aspect_ratio,
  float near_plane,
  float far_plane
);

void ExitOnGLError(const char* error_message);
GLuint LoadShader(const char* filename, GLenum shader_type);

void	process_args(const int argc, const char *argv[]);
void	error_check(char val, const char *msg);

#endif
