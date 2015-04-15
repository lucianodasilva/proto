
#include <GL/glew.h>
#include <GL/freeglut.h>

//#include <SFML/Graphics.hpp>
//#include <SFML/OpenGL.hpp>

void render_callback () {

}

int main(int arg_c, char * arg_v[]) {

    glutInit(&arg_c, arg_v);

    glutInitWindowPosition (-1, -1);
    glutInitWindowSize (600, 600);

    glutInitDisplayMode (GLUT_RGBA);

    glutCreateWindow ("Tilling Demo");

    glutDisplayFunc (&render_callback);

    glutMainLoop();

    return 0;
}