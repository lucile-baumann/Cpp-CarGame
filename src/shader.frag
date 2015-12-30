#version 120

varying vec3 coordonnee_3d;
varying vec3 coordonnee_3d_locale;
varying vec3 normale;
varying vec4 color;

uniform sampler2D texture;

vec3 light=vec3(0.5,0.5,5.0);


void main (void)
{
    //vecteurs pour le calcul d'illumination
    vec3 n = normalize(normale);
    vec3 d = normalize(light-coordonnee_3d_locale);
    vec3 r = reflect(d,n);
    vec3 o = normalize(-coordonnee_3d_locale);

    //calcul d'illumination
    float diffuse  = 0.7*clamp(dot(n,d),0.0,1.0);
    float specular = 0.2*pow(clamp(dot(r,o),0.0,1.0),128.0);
    float ambiant  = 0.2;

    vec4 white = vec4(1.0,1.0,1.0,0.0);

    //recuperation de la texture
    vec2 tex_coord     = gl_TexCoord[0].xy;
    vec4 color_texture = texture2D(texture,tex_coord);
    vec4 color_final   = color*color_texture;

    //couleur finale
    gl_FragColor = (ambiant+diffuse)*color_final+specular*white;

}
