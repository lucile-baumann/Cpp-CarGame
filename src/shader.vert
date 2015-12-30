#version 120

uniform mat4 rotation_model;
uniform vec4 rotation_center_model;
uniform vec4 translation_model;

uniform mat4 rotation_view;
uniform vec4 rotation_center_view;
uniform vec4 translation_view;

uniform mat4 projection;


varying vec3 coordonnee_3d;
varying vec3 coordonnee_3d_locale;
varying vec3 normale;
varying vec4 color;


//Un Vertex Shader minimaliste
void main (void)
{
    //Les coordonnees 3D du sommet
    coordonnee_3d = gl_Vertex.xyz;

    //application de la deformation du model
    vec4 p_model = rotation_model*(gl_Vertex-rotation_center_model)+rotation_center_model+translation_model;
    //application de la deformation de la vue
    vec4 p_modelview = rotation_view*(p_model-rotation_center_view)+rotation_center_view+translation_view;

    coordonnee_3d_locale = p_modelview.xyz;


    //Projection du sommet
    vec4 p_proj = projection*p_modelview;

    //Gestion des normales
    vec4 n = rotation_view*rotation_model*vec4(gl_Normal,0.0);
    normale=n.xyz;

    //Couleur du sommet
    color=gl_Color;

    //position dans l'espace ecran
    gl_Position = p_proj;

    //coordonnees de textures
    gl_TexCoord[0]=gl_MultiTexCoord0;
}
