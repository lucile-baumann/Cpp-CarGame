/*****************************************************************************\
 * TP CPE                                                                    *
 * ------
 *
 * Fonctions d'aides pour OpenGL (affichage d'erreur, chargement de shaders, etc)
 * Code repris de David Odin
 *
 *
\*****************************************************************************/




#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>

#define GLEW_STATIC 1
#include <GL/glew.h>

#include <OpenGL/gl.h>

#include "glutils.hpp"

/*****************************************************************************\
 * print_opengl_error                                                        *
\*****************************************************************************/
bool print_opengl_error(const char *file, int line)
{
  // Returns true if an OpenGL error occurred, false otherwise.
  GLenum error;
  bool   ret_code = false;

  error = glGetError();
  while (error != GL_NO_ERROR)
  {
    std::cerr << "glError in file " << file << ", line " << line << ": " <<
      gluErrorString(error) << std::endl;
    ret_code = true;
    error = glGetError();
  }
  return ret_code;
}

/*****************************************************************************\
 * get_gl_version                                                            *
\*****************************************************************************/
void get_gl_version(int *major, int *minor)
{
  const GLubyte *version = glGetString(GL_VERSION); PRINT_OPENGL_ERROR();

  if (!version || (sscanf(reinterpret_cast<const char*>(version), "%d.%d", major, minor) != 2))
  {
    *major = *minor = 0;
    std::cerr << "Invalid GL_VERSION format!!!" << std::endl;
  }
}

/*****************************************************************************\
 * print_shader_info_log                                                     *
\*****************************************************************************/
static void print_shader_info_log(GLuint shader)
{
  int     info_log_length = 0;
  int     chars_written  = 0;
  GLchar *info_log;

  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length); PRINT_OPENGL_ERROR();

  if (info_log_length > 1)
  {
    info_log = new GLchar[info_log_length];
    glGetShaderInfoLog(shader, info_log_length, &chars_written, info_log); PRINT_OPENGL_ERROR();
    std::cout << "Shader InfoLog:" << std::endl << info_log << std::endl;
    delete[] info_log;
  }
}

/*****************************************************************************\
 * print_program_info_log                                                    *
\*****************************************************************************/
static void print_program_info_log(GLuint program)
{
  int     info_log_length = 0;
  int     chars_written  = 0;
  GLchar *info_log;

  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length); PRINT_OPENGL_ERROR();

  if (info_log_length > 1)
  {
    info_log = new GLchar[info_log_length];
    glGetProgramInfoLog(program, info_log_length, &chars_written, info_log); PRINT_OPENGL_ERROR();
    std::cout << "Program InfoLog:" << std::endl << info_log << std::endl;
    delete[] info_log;
  }
  PRINT_OPENGL_ERROR();  // Check for OpenGL errors
}

/*****************************************************************************\
 * get_uni_loc                                                               *
\*****************************************************************************/
GLint get_uni_loc(GLuint program, const GLchar *name)
{
  GLint loc;

  loc = glGetUniformLocation(program, name); PRINT_OPENGL_ERROR();
  if (loc == -1)
    std::cerr << "No such uniform named \"" << name << "\"" << std::endl;

  return loc;
}

/*****************************************************************************\
 * get_file_content                                                          *
\*****************************************************************************/
static std::string get_file_content(const std::string &filename)
{
  std::string ret;
  if (FILE *fp = fopen(filename.c_str(), "r"))
  {
    char buf[1024];
    while (size_t len = fread(buf, 1, sizeof buf, fp))
      ret += std::string(buf, buf + len);
    fclose(fp);
  }
  return ret;
}

/*****************************************************************************\
 * read_shader                                                               *
\*****************************************************************************/
GLuint read_shader(const std::string &vertex_filename,
                   const std::string &fragment_filename)
{
  GLint        status;
  std::string  source;
  const char  *cstring;
  GLuint       program;

  if(access(vertex_filename.data(),F_OK) == -1)
  {
      std::cerr<<"Fichier "<<vertex_filename<<" non accessible "<<std::endl;
      std::cerr<<"Executez vous le programme dans le bon repertoire ?"<<std::endl;
      abort();
  }
  if(access(fragment_filename.data(),F_OK) == -1)
  {
      std::cerr<<"Fichier "<<vertex_filename<<" non accessible "<<std::endl;
      std::cerr<<"Executez vous le programme dans le bon repertoire ?"<<std::endl;
      abort();
  }


  program = glCreateProgram(); PRINT_OPENGL_ERROR();

  // Create the vertex shader.
  if (vertex_filename != "")
    {
      GLuint handle = glCreateShader(GL_VERTEX_SHADER); PRINT_OPENGL_ERROR();
      source = get_file_content(vertex_filename);
      cstring = source.c_str();
      glShaderSource(handle, 1, &cstring, NULL);   PRINT_OPENGL_ERROR();
      // Compile the vertex shader, and print out the compiler log file.
      glCompileShader(handle); PRINT_OPENGL_ERROR();
      glGetShaderiv(handle, GL_COMPILE_STATUS, &status); PRINT_OPENGL_ERROR();
      print_shader_info_log(handle);
      glAttachShader(program, handle); PRINT_OPENGL_ERROR();
      if (!status) return 0;
    }

  // Create the fragment shader
  if (fragment_filename != "")
    {
      GLuint handle = glCreateShader(GL_FRAGMENT_SHADER); PRINT_OPENGL_ERROR();
      source = get_file_content(fragment_filename);
      cstring = source.c_str();
      glShaderSource(handle, 1, &cstring, NULL); PRINT_OPENGL_ERROR();
      // Compile the fragment shader, and print out the compiler log file.
      glCompileShader(handle); PRINT_OPENGL_ERROR();
      glGetShaderiv(handle, GL_COMPILE_STATUS, &status); PRINT_OPENGL_ERROR();
      print_shader_info_log(handle);
      glAttachShader(program, handle); PRINT_OPENGL_ERROR();
      if (!status) return 0;
    }

  // Link the program object and print out the info log.
  glLinkProgram(program); PRINT_OPENGL_ERROR();
  glGetProgramiv(program, GL_LINK_STATUS, &status); PRINT_OPENGL_ERROR();
  print_program_info_log(program);
  if (!status) return 0;

  // Install program object as part of current state.
  glUseProgram(program); PRINT_OPENGL_ERROR();

  return program;
}


/*****************************************************************************\
 * Buffer offset                                                              *
\*****************************************************************************/
GLubyte* buffer_offset(int offset)
{
    return (GLubyte*)NULL+offset;
}
