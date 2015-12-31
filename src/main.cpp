/*****************************************************************************\
 * TP CPE, ETI, TP synthese d'images
 * --------------
 * Auth : Lucile Madoulaud
 * Programme principal des appels OpenGL
\*****************************************************************************/



#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GLUT/glut.h>

#include "glutils.hpp"

#include "mat4.hpp"
#include "vec3.hpp"
#include "vec2.hpp"
#include "triangle_index.hpp"
#include "vertex_opengl.hpp"
#include "image.hpp"
#include "mesh.hpp"


/*****************************************************************************\
 * Variables globales
 *
 *
\*****************************************************************************/


//identifiant du shader
GLuint shader_program_id;

//identifiants pour object 1
GLuint vbo_object_1=0;
GLuint vboi_object_1 = 0;
GLuint texture_id_object_1=0;
int nbr_triangle_object_1;

//identifiants pour object 2
GLuint vbo_object_2=0;
GLuint vboi_object_2=0;
GLuint texture_id_object_2=0;
int nbr_triangle_object_2;

//identifiants pour object 3
GLuint vbo_object_3=0;
GLuint vboi_object_3=0;
GLuint texture_id_object_3=0;
int nbr_triangle_object_3;

//identifiants pour object ville
GLuint vbo_object_ville=0;
GLuint vboi_object_ville = 0;
GLuint texture_id_object_ville=0;
int nbr_triangle_object_ville;

int count_time;

//Matrice de transformation
struct transformation
{
    mat4 rotation;
    vec3 rotation_center;
    vec3 translation;

    transformation():rotation(),rotation_center(),translation(){}
};

//Transformation des modeles
transformation transformation_model_1;
transformation transformation_model_2;
transformation transformation_model_3;

//Transformation de la vue (camera)
transformation transformation_view;

//Matrice de projection
mat4 projection;

//angle de deplacement
float angle_x_model_1 = 0.0f;
float angle_y_model_1 = 0.0f;
float angle_view = 0.0f;
float angle_view2 = 0.0f;


void load_texture(const char* filename,GLuint *texture_id);

void init_model_1();
void init_model_2();
void init_model_3();

//void init_model_route();
void init_model_voiture();

void draw_model_1();
void draw_model_2();
void draw_model_3();

//void draw_model_route();
void draw_model_voiture();

static void init()
{

    // Chargement du shader
    shader_program_id = read_shader("shader.vert", "shader.frag");

    //matrice de projection
    projection = matrice_projection(60.0f*M_PI/180.0f,1.0f,0.01f,100.0f);
    glUniformMatrix4fv(get_uni_loc(shader_program_id,"projection"),1,false,pointeur(projection)); PRINT_OPENGL_ERROR();

    //centre de rotation de la 'camera' (les objets sont centres en z=-2)
    transformation_view.rotation_center = vec3(0.0f,0.0f,0.0f);
    transformation_view.rotation = matrice_rotation(90.0f*M_PI/180.0f , 0.0f,1.0f,0.0f);

    transformation_view.translation += vec3(0.0f,-1.0f,-2.0f);
    //activation de la gestion de la profondeur
    glEnable(GL_DEPTH_TEST); PRINT_OPENGL_ERROR();

    // Charge modele 1 sur la carte graphique
    //init_model_1();
    // Charge modele 2 sur la carte graphique
    init_model_2();
    // Charge modele 3 sur la carte graphique
    //init_model_3();

    // Charge model route
    //init_model_route();
    // Charge model voiture
    init_model_voiture();
    //transformation_model_1.translation.x=-10.0f;
}


//Fonction d'affichage
static void display_callback()
{
    //effacement des couleurs du fond d'ecran
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);                 PRINT_OPENGL_ERROR();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   PRINT_OPENGL_ERROR();

    // Affecte les parametres uniformes de la vue (identique pour tous les modeles de la scene)
    {
        //envoie de la rotation
        glUniformMatrix4fv(get_uni_loc(shader_program_id,"rotation_view"),1,false,pointeur(transformation_view.rotation)); PRINT_OPENGL_ERROR();

        //envoie du centre de rotation
        vec3 cv = transformation_view.rotation_center;
        glUniform4f(get_uni_loc(shader_program_id,"rotation_center_view") , cv.x,cv.y,cv.z , 0.0f); PRINT_OPENGL_ERROR();

        //envoie de la translation
        vec3 tv = transformation_view.translation;
        glUniform4f(get_uni_loc(shader_program_id,"translation_view") , tv.x,tv.y,tv.z , 0.0f); PRINT_OPENGL_ERROR();

        glUniform1i(get_uni_loc(shader_program_id,"time"),0);                                                                  PRINT_OPENGL_ERROR();

    }


    // Affiche le modele numero 1 (dinosaure)
    //draw_model_1();
    // Affiche le modele numero 2 (sol)
    draw_model_2();
    // Affiche le modele numero 3 (monstre)
    //draw_model_3();

    // Affiche le modele de la route
    //draw_model_route();
    // Affiche le modele de la voiture
    draw_model_voiture();

    //Changement de buffer d'affichage pour eviter un effet de scintillement
    glutSwapBuffers();
}



/*****************************************************************************\
 * keyboard_callback                                                         *
\*****************************************************************************/
static void keyboard_callback(unsigned char key, int, int)
{
    float d_angle=0.1f;
    float dz=0.5f;
    vec3 axe = vec3(0.0f, 0.0f,0.0f);
    //quitte le programme si on appuie sur les touches 'q', 'Q', ou 'echap'
    switch (key)
    {
    case 'q':
    case 'Q':
    case 27:
        exit(0);
        break;
    case 's':
        break;
    }
}

/*****************************************************************************\
 * special_callback                                                          *
\*****************************************************************************/
static void special_callback(int key, int,int)
{
    float dL=0.1f;
    switch (key)
    {
    case GLUT_KEY_UP:
        count_time += 5; //rotation avec la touche du haut
        break;
    case GLUT_KEY_DOWN:
        count_time --; //rotation avec la touche du bas
        break;
    case GLUT_KEY_LEFT:
        transformation_model_1.translation.z += dL; //rotation avec la touche de gauche
        break;
    case GLUT_KEY_RIGHT:
        transformation_model_1.translation.z -= dL; //rotation avec la touche de droite
        break;
    }

    //reactualisation de l'affichage
    glutPostRedisplay();
}


/*****************************************************************************\
 * timer_callback                                                            *
\*****************************************************************************/
static void timer_callback(int)
{
    //demande de rappel de cette fonction dans 25ms
    glutTimerFunc(25, timer_callback, 0);

    count_time++;

    //reactualisation de l'affichage
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    //**********************************************//
    //Lancement des fonctions principales de GLUT
    //**********************************************//

    //initialisation
    glutInit(&argc, argv);

    //Mode d'affichage (couleur, gestion de profondeur, ...)
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    //Taille de la fenetre a l'ouverture
    glutInitWindowSize(600, 600);

    //Titre de la fenetre
    glutCreateWindow("OpenGL");

    //Fonction de la boucle d'affichage
    glutDisplayFunc(display_callback);

    //Fonction de gestion du clavier
    glutKeyboardFunc(keyboard_callback);

    //Fonction des touches speciales du clavier (fleches directionnelles)
    glutSpecialFunc(special_callback);

    //Fonction d'appel d'affichage en chaine
    glutTimerFunc(25, timer_callback, 0);

    //Initialisation des fonctions OpenGL
    glewInit();

    //Notre fonction d'initialisation des donnees et chargement des shaders
    init();


    //Lancement de la boucle (infinie) d'affichage de la fenetre
    glutMainLoop();

    //Plus rien n'est execute apres cela

    return 0;
}


void draw_model_1()
{

    //envoie des parametres uniformes
    {
        glUniformMatrix4fv(get_uni_loc(shader_program_id,"rotation_model"),1,false,pointeur(transformation_model_1.rotation));    PRINT_OPENGL_ERROR();

        vec3 c = transformation_model_1.rotation_center;
        glUniform4f(get_uni_loc(shader_program_id,"rotation_center_model") , c.x,c.y,c.z , 0.0f);                                 PRINT_OPENGL_ERROR();

        vec3 t = transformation_model_1.translation;
        glUniform4f(get_uni_loc(shader_program_id,"translation_model") , t.x,t.y,t.z , 0.0f);                                     PRINT_OPENGL_ERROR();
    }

    //placement des VBO
    {
        //selection du VBO courant
        glBindBuffer(GL_ARRAY_BUFFER,vbo_object_1);                                                    PRINT_OPENGL_ERROR();

        // mise en place des differents pointeurs
        glEnableClientState(GL_VERTEX_ARRAY);                                                          PRINT_OPENGL_ERROR();
        glVertexPointer(3, GL_FLOAT, sizeof(vertex_opengl), 0);                                        PRINT_OPENGL_ERROR();

        glEnableClientState(GL_NORMAL_ARRAY); PRINT_OPENGL_ERROR();                                    PRINT_OPENGL_ERROR();
        glNormalPointer(GL_FLOAT, sizeof(vertex_opengl), buffer_offset(sizeof(vec3)));                 PRINT_OPENGL_ERROR();

        glEnableClientState(GL_COLOR_ARRAY); PRINT_OPENGL_ERROR();                                     PRINT_OPENGL_ERROR();
        glColorPointer(3,GL_FLOAT, sizeof(vertex_opengl), buffer_offset(2*sizeof(vec3)));              PRINT_OPENGL_ERROR();

        glEnableClientState(GL_TEXTURE_COORD_ARRAY); PRINT_OPENGL_ERROR();                             PRINT_OPENGL_ERROR();
        glTexCoordPointer(2,GL_FLOAT, sizeof(vertex_opengl), buffer_offset(3*sizeof(vec3)));           PRINT_OPENGL_ERROR();

    }

    //affichage
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi_object_1);                           PRINT_OPENGL_ERROR();
        glBindTexture(GL_TEXTURE_2D, texture_id_object_1);                             PRINT_OPENGL_ERROR();
        glDrawElements(GL_TRIANGLES, 3*nbr_triangle_object_1, GL_UNSIGNED_INT, 0);     PRINT_OPENGL_ERROR();
    }

}

void draw_model_2()
{

    //envoie des parametres uniformes
    {
        glUniformMatrix4fv(get_uni_loc(shader_program_id,"rotation_model"),1,false,pointeur(transformation_model_2.rotation));    PRINT_OPENGL_ERROR();

        vec3 c = transformation_model_2.rotation_center;
        glUniform4f(get_uni_loc(shader_program_id,"rotation_center_model") , c.x,c.y,c.z , 0.0f);                                 PRINT_OPENGL_ERROR();

        vec3 t = transformation_model_2.translation;
        glUniform4f(get_uni_loc(shader_program_id,"translation_model") , t.x,t.y,t.z , 0.0f);                                     PRINT_OPENGL_ERROR();

        glUniform1i(get_uni_loc(shader_program_id,"time"),count_time);                                                                  PRINT_OPENGL_ERROR();
    }

    //placement des VBO
    {
        //selection du VBO courant
        glBindBuffer(GL_ARRAY_BUFFER,vbo_object_2);                                                    PRINT_OPENGL_ERROR();

        // mise en place des differents pointeurs
        glEnableClientState(GL_VERTEX_ARRAY);                                                          PRINT_OPENGL_ERROR();
        glVertexPointer(3, GL_FLOAT, sizeof(vertex_opengl), 0);                                        PRINT_OPENGL_ERROR();

        glEnableClientState(GL_NORMAL_ARRAY); PRINT_OPENGL_ERROR();                                    PRINT_OPENGL_ERROR();
        glNormalPointer(GL_FLOAT, sizeof(vertex_opengl), buffer_offset(sizeof(vec3)));                 PRINT_OPENGL_ERROR();

        glEnableClientState(GL_COLOR_ARRAY); PRINT_OPENGL_ERROR();                                     PRINT_OPENGL_ERROR();
        glColorPointer(3,GL_FLOAT, sizeof(vertex_opengl), buffer_offset(2*sizeof(vec3)));              PRINT_OPENGL_ERROR();

        glEnableClientState(GL_TEXTURE_COORD_ARRAY); PRINT_OPENGL_ERROR();                             PRINT_OPENGL_ERROR();
        glTexCoordPointer(2,GL_FLOAT, sizeof(vertex_opengl), buffer_offset(3*sizeof(vec3)));           PRINT_OPENGL_ERROR();

    }

    //affichage
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi_object_2);                           PRINT_OPENGL_ERROR();
        glBindTexture(GL_TEXTURE_2D, texture_id_object_2);                             PRINT_OPENGL_ERROR();
        glDrawElements(GL_TRIANGLES, 3*nbr_triangle_object_2, GL_UNSIGNED_INT, 0);     PRINT_OPENGL_ERROR();
    }
}

void draw_model_3()
{
    //envoie des parametres uniformes
    {
        glUniformMatrix4fv(get_uni_loc(shader_program_id,"rotation_model"),1,false,pointeur(transformation_model_3.rotation));    PRINT_OPENGL_ERROR();

        vec3 c = transformation_model_3.rotation_center;
        glUniform4f(get_uni_loc(shader_program_id,"rotation_center_model") , c.x,c.y,c.z , 0.0f);                                 PRINT_OPENGL_ERROR();

        vec3 t = transformation_model_3.translation;
        glUniform4f(get_uni_loc(shader_program_id,"translation_model") , t.x,t.y,t.z , 0.0f);                                     PRINT_OPENGL_ERROR();
    }

    //placement des VBO
    {
        //selection du VBO courant
        glBindBuffer(GL_ARRAY_BUFFER,vbo_object_3);                                                    PRINT_OPENGL_ERROR();

        // mise en place des differents pointeurs
        glEnableClientState(GL_VERTEX_ARRAY);                                                          PRINT_OPENGL_ERROR();
        glVertexPointer(3, GL_FLOAT, sizeof(vertex_opengl), 0);                                        PRINT_OPENGL_ERROR();

        glEnableClientState(GL_NORMAL_ARRAY); PRINT_OPENGL_ERROR();                                    PRINT_OPENGL_ERROR();
        glNormalPointer(GL_FLOAT, sizeof(vertex_opengl), buffer_offset(sizeof(vec3)));                 PRINT_OPENGL_ERROR();

        glEnableClientState(GL_COLOR_ARRAY); PRINT_OPENGL_ERROR();                                     PRINT_OPENGL_ERROR();
        glColorPointer(3,GL_FLOAT, sizeof(vertex_opengl), buffer_offset(2*sizeof(vec3)));              PRINT_OPENGL_ERROR();

        glEnableClientState(GL_TEXTURE_COORD_ARRAY); PRINT_OPENGL_ERROR();                             PRINT_OPENGL_ERROR();
        glTexCoordPointer(2,GL_FLOAT, sizeof(vertex_opengl), buffer_offset(3*sizeof(vec3)));           PRINT_OPENGL_ERROR();

    }

    //affichage
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi_object_3);                           PRINT_OPENGL_ERROR();
        glBindTexture(GL_TEXTURE_2D, texture_id_object_3);                             PRINT_OPENGL_ERROR();
        glDrawElements(GL_TRIANGLES, 3*nbr_triangle_object_3, GL_UNSIGNED_INT, 0);     PRINT_OPENGL_ERROR();
    }
}

void draw_model_voiture()
{

    //envoie des parametres uniformes
    {
        glUniformMatrix4fv(get_uni_loc(shader_program_id,"rotation_model"),1,false,pointeur(transformation_model_1.rotation));    PRINT_OPENGL_ERROR();

        vec3 c = transformation_model_1.rotation_center;
        glUniform4f(get_uni_loc(shader_program_id,"rotation_center_model") , c.x,c.y,c.z , 0.0f);                                 PRINT_OPENGL_ERROR();

        vec3 t = transformation_model_1.translation;
        glUniform4f(get_uni_loc(shader_program_id,"translation_model") , t.x,t.y,t.z , 0.0f);                                     PRINT_OPENGL_ERROR();

        glUniform1i(get_uni_loc(shader_program_id,"time"),0);                                                                  PRINT_OPENGL_ERROR();
    }

    //placement des VBO
    {
        //selection du VBO courant
        glBindBuffer(GL_ARRAY_BUFFER,vbo_object_1);                                                    PRINT_OPENGL_ERROR();

        // mise en place des differents pointeurs
        glEnableClientState(GL_VERTEX_ARRAY);                                                          PRINT_OPENGL_ERROR();
        glVertexPointer(3, GL_FLOAT, sizeof(vertex_opengl), 0);                                        PRINT_OPENGL_ERROR();

        glEnableClientState(GL_NORMAL_ARRAY); PRINT_OPENGL_ERROR();                                    PRINT_OPENGL_ERROR();
        glNormalPointer(GL_FLOAT, sizeof(vertex_opengl), buffer_offset(sizeof(vec3)));                 PRINT_OPENGL_ERROR();

        glEnableClientState(GL_COLOR_ARRAY); PRINT_OPENGL_ERROR();                                     PRINT_OPENGL_ERROR();
        glColorPointer(3,GL_FLOAT, sizeof(vertex_opengl), buffer_offset(2*sizeof(vec3)));              PRINT_OPENGL_ERROR();

        glEnableClientState(GL_TEXTURE_COORD_ARRAY); PRINT_OPENGL_ERROR();                             PRINT_OPENGL_ERROR();
        glTexCoordPointer(2,GL_FLOAT, sizeof(vertex_opengl), buffer_offset(3*sizeof(vec3)));           PRINT_OPENGL_ERROR();

    }

    //affichage
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi_object_1);                           PRINT_OPENGL_ERROR();
        glBindTexture(GL_TEXTURE_2D, texture_id_object_1);                             PRINT_OPENGL_ERROR();
        glDrawElements(GL_TRIANGLES, 3*nbr_triangle_object_1, GL_UNSIGNED_INT, 0);     PRINT_OPENGL_ERROR();
    }

}

void init_model_1()
{
    // Chargement d'un maillage a partir d'un fichier
    mesh m = load_obj_file("../data/ville.obj");

    // Affecte une transformation sur les sommets du maillage
    float s = 1.0f;
    mat4 transform = mat4(   s, 0.0f, 0.0f, 0.0f,
                          0.0f,    s, 0.0f,-0.9f,
                          0.0f, 0.0f,   s ,-2.0f,
                          0.0f, 0.0f, 0.0f, 1.0f);
    apply_deformation(&m,transform);

    // Centre la rotation du modele 1 autour de son centre de gravite approximatif
    transformation_model_1.rotation_center = vec3(0.0f,-0.5f,-2.0f);

    // Calcul automatique des normales du maillage
    update_normals(&m);
    // Les sommets sont affectes a une couleur blanche
    fill_color(&m,vec3(1.0f,1.0f,1.0f));

    //attribution d'un buffer de donnees (1 indique la création d'un buffer)
    glGenBuffers(1,&vbo_object_1); PRINT_OPENGL_ERROR();
    //affectation du buffer courant
    glBindBuffer(GL_ARRAY_BUFFER,vbo_object_1); PRINT_OPENGL_ERROR();
    //copie des donnees des sommets sur la carte graphique
    glBufferData(GL_ARRAY_BUFFER,m.vertex.size()*sizeof(vertex_opengl),&m.vertex[0],GL_STATIC_DRAW); PRINT_OPENGL_ERROR();


    //attribution d'un autre buffer de donnees
    glGenBuffers(1,&vboi_object_1); PRINT_OPENGL_ERROR();
    //affectation du buffer courant (buffer d'indice)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi_object_1); PRINT_OPENGL_ERROR();
    //copie des indices sur la carte graphique
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,m.connectivity.size()*sizeof(triangle_index),&m.connectivity[0],GL_STATIC_DRAW); PRINT_OPENGL_ERROR();

    // Nombre de triangles de l'objet 1
    nbr_triangle_object_1 = m.connectivity.size();

    // Chargement de la texture
    load_texture("../data/white.tga",&texture_id_object_1);


}

void init_model_2()
{
    //Creation manuelle du model 2

    //coordonnees geometriques des sommets
    vec3 p0 = vec3(0.0f,-3.0f,-5.0f);
    vec3 p1 = vec3( 80.0f,10.0f,-5.0f);
    vec3 p2 = vec3( 80.0f,10.0f, 5.0f);
    vec3 p3 = vec3(0.0f,-3.0f, 5.0f);

    //normales pour chaque sommet
    vec3 n0=vec3(0.0f,1.0f,0.0f);
    vec3 n1=n0;
    vec3 n2=n0;
    vec3 n3=n0;

    //couleur pour chaque sommet
    vec3 c0=vec3(1.0f,1.0f,1.0f);
    vec3 c1=c0;
    vec3 c2=c0;
    vec3 c3=c0;

    //texture du sommet
    vec2 t0=vec2(0.0f,0.0f);
    vec2 t1=vec2(1.0f,0.0f);
    vec2 t2=vec2(1.0f,1.0f);
    vec2 t3=vec2(0.0f,1.0f);

    vertex_opengl v0=vertex_opengl(p0,n0,c0,t0);
    vertex_opengl v1=vertex_opengl(p1,n1,c1,t1);
    vertex_opengl v2=vertex_opengl(p2,n2,c2,t2);
    vertex_opengl v3=vertex_opengl(p3,n3,c3,t3);


    //tableau entrelacant coordonnees-normales
    vertex_opengl geometrie[]={v0,v1,v2,v3};


    //indice des triangles
    triangle_index tri0=triangle_index(0,1,2);
    triangle_index tri1=triangle_index(0,2,3);
    triangle_index index[]={tri0,tri1};
    nbr_triangle_object_2 = 2;

    //attribution d'un buffer de donnees (1 indique la création d'un buffer)
    glGenBuffers(1,&vbo_object_2);                                             PRINT_OPENGL_ERROR();
    //affectation du buffer courant
    glBindBuffer(GL_ARRAY_BUFFER,vbo_object_2);                                PRINT_OPENGL_ERROR();
    //copie des donnees des sommets sur la carte graphique
    glBufferData(GL_ARRAY_BUFFER,sizeof(geometrie),geometrie,GL_STATIC_DRAW);  PRINT_OPENGL_ERROR();


    //attribution d'un autre buffer de donnees
    glGenBuffers(1,&vboi_object_2);                                            PRINT_OPENGL_ERROR();
    //affectation du buffer courant (buffer d'indice)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi_object_2);                       PRINT_OPENGL_ERROR();
    //copie des indices sur la carte graphique
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(index),index,GL_STATIC_DRAW);  PRINT_OPENGL_ERROR();

    // Chargement de la texture
    load_texture("../data/road.tga",&texture_id_object_2);

}

void init_model_3()
{
    // Chargement d'un maillage a partir d'un fichier
    mesh m = load_off_file("../data/armadillo_light.off");

    // Affecte une transformation sur les sommets du maillage
    float s = 0.01f;
    mat4 transform = mat4(   s, 0.0f, 0.0f, 1.0f,
                          0.0f,    s, 0.0f,-0.2f,
                          0.0f, 0.0f,   s ,-2.0f,
                          0.0f, 0.0f, 0.0f, 1.0f);
    apply_deformation(&m,matrice_rotation(M_PI/2.0f,1.0f,0.0f,0.0f));
    apply_deformation(&m,matrice_rotation(1.5*M_PI/2.0f,0.0f,1.0f,0.0f));
    apply_deformation(&m,transform);

    // Calcul automatique des normales du maillage
    update_normals(&m);
    // Les sommets sont affectes a une couleur blanche
    fill_color(&m,vec3(1.0f,1.0f,1.0f));

    //attribution d'un buffer de donnees (1 indique la création d'un buffer)
    glGenBuffers(1,&vbo_object_3);                                 PRINT_OPENGL_ERROR();
    //affectation du buffer courant
    glBindBuffer(GL_ARRAY_BUFFER,vbo_object_3); PRINT_OPENGL_ERROR();
    //copie des donnees des sommets sur la carte graphique
    glBufferData(GL_ARRAY_BUFFER,m.vertex.size()*sizeof(vertex_opengl),&m.vertex[0],GL_STATIC_DRAW); PRINT_OPENGL_ERROR();


    //attribution d'un autre buffer de donnees
    glGenBuffers(1,&vboi_object_3); PRINT_OPENGL_ERROR();
    //affectation du buffer courant (buffer d'indice)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi_object_3); PRINT_OPENGL_ERROR();
    //copie des indices sur la carte graphique
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,m.connectivity.size()*sizeof(triangle_index),&m.connectivity[0],GL_STATIC_DRAW); PRINT_OPENGL_ERROR();

    // Nombre de triangles de l'objet 3
    nbr_triangle_object_3 = m.connectivity.size();

    // Chargement de la texture
    load_texture("../data/white.tga",&texture_id_object_3);


}

void init_model_voiture()
{
    // Chargement d'un maillage a partir d'un fichier
    mesh m = load_obj_file("../data/audi.obj");

    // Affecte une transformation sur les sommets du maillage
    float s = 1.0f;
    mat4 transform = mat4(   s, 0.0f, 0.0f, 3.0f,
                          0.0f,    s, 0.0f,-0.9f,
                          0.0f, 0.0f,   s , 0.0f,
                          0.0f, 0.0f, 0.0f, 1.0f);
    apply_deformation(&m,transform);

    // Centre la rotation du modele 1 autour de son centre de gravite approximatif
    transformation_model_1.rotation_center = vec3(3.0f,-0.5f,0.0f);

    // Calcul automatique des normales du maillage
    update_normals(&m);
    // Les sommets sont affectes a une couleur blanche
    fill_color(&m,vec3(1.0f,1.0f,1.0f));

    //attribution d'un buffer de donnees (1 indique la création d'un buffer)
    glGenBuffers(1,&vbo_object_1); PRINT_OPENGL_ERROR();
    //affectation du buffer courant
    glBindBuffer(GL_ARRAY_BUFFER,vbo_object_1); PRINT_OPENGL_ERROR();
    //copie des donnees des sommets sur la carte graphique
    glBufferData(GL_ARRAY_BUFFER,m.vertex.size()*sizeof(vertex_opengl),&m.vertex[0],GL_STATIC_DRAW); PRINT_OPENGL_ERROR();


    //attribution d'un autre buffer de donnees
    glGenBuffers(1,&vboi_object_1); PRINT_OPENGL_ERROR();
    //affectation du buffer courant (buffer d'indice)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi_object_1); PRINT_OPENGL_ERROR();
    //copie des indices sur la carte graphique
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,m.connectivity.size()*sizeof(triangle_index),&m.connectivity[0],GL_STATIC_DRAW); PRINT_OPENGL_ERROR();

    // Nombre de triangles de l'objet 1
    nbr_triangle_object_1 = m.connectivity.size();

    // Chargement de la texture
    load_texture("../data/AudiUV.tga",&texture_id_object_1);


}

void load_texture(const char* filename,GLuint *texture_id)
{
    // Chargement d'une texture (seul les textures tga sont supportes)
    Image  *image = image_load_tga(filename);
    if (image) //verification que l'image est bien chargee
    {

        //Creation d'un identifiant pour la texture
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); PRINT_OPENGL_ERROR();
        glGenTextures(1, texture_id); PRINT_OPENGL_ERROR();

        //Selection de la texture courante a partir de son identifiant
        glBindTexture(GL_TEXTURE_2D, *texture_id); PRINT_OPENGL_ERROR();

        //Parametres de la texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); PRINT_OPENGL_ERROR();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); PRINT_OPENGL_ERROR();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); PRINT_OPENGL_ERROR();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); PRINT_OPENGL_ERROR();

        //Envoie de l'image en memoire video
        if(image->type==IMAGE_TYPE_RGB){ //image RGB
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data); PRINT_OPENGL_ERROR();}
        else if(image->type==IMAGE_TYPE_RGBA){ //image RGBA
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data); PRINT_OPENGL_ERROR();}
        else{
            std::cout<<"Image type not handled"<<std::endl;}

        delete image;
    }
    else
    {
        std::cerr<<"Erreur chargement de l'image, etes-vous dans le bon repertoire?"<<std::endl;
        abort();
    }

    glUniform1i (get_uni_loc(shader_program_id, "texture"), 0); PRINT_OPENGL_ERROR();
}
