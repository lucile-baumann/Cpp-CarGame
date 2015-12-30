/*****************************************************************************\
 * TP CPE                                                                    *
 * ------
 *
 * Fonctions d'aides pour OpenGL (affichage d'erreur, chargement de shaders, etc)
 * Code repris de David Odin
 *
 *
\*****************************************************************************/


#ifndef GL_UTILS_H
#define GL_UTILS_H

#include <vector>
#include <string>

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <OpenGL/gl.h>


//Macro permettant la recherche et l'affichage des erreurs OpenGL, indique le fichier et la ligne au moment de l'appel.
#define PRINT_OPENGL_ERROR() print_opengl_error(__FILE__, __LINE__)




//Affichage des erreurs OpenGL
bool print_opengl_error(const char *file, int line);

//Retourne la version d'OpenGL
void get_gl_version(int *major, int *minor);

//Recherche une variable uniform dans le shader
GLint get_uni_loc(GLuint program, const GLchar *name);

//Chargement d'un shader un partir d'un fichier
// 1er argument: fichier contenant le vertex shader
// 2eme argument: fichier contenant le fragment shader
// Si on ne souhaite charge qu'un shader, on laissera la chaine vide "" pour l'autre nom.
GLuint read_shader(const std::string &vertex_filename,
                   const std::string &fragment_filename);

//Decallage d'offset sur pour les buffers des VBO (un simple cast vers un pointeur)
// Le parametre doit contenir l'offset en octets
GLubyte* buffer_offset(int offset);

#endif
