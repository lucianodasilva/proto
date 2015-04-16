
#define FREEGLUT_STATIC

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "proto.debug.h"

void render_callback () {

}

int main(int arg_c, char * arg_v[]) {

	debug_print << "this is a message";


	return 0;

    glutInit(&arg_c, arg_v);

    glutInitWindowPosition (-1, -1);
    glutInitWindowSize (600, 600);

    glutInitDisplayMode (GLUT_RGBA);

    glutCreateWindow ("Tilling Demo");

    glutDisplayFunc (&render_callback);

    glutMainLoop();

    return 0;
}