
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "proto.debug.h"
#include "proto.renderer.h"

proto::renderer r;

void render_callback () {
	r.clear ({ .0F, .2F, .3F }, proto::clear_mask::color | proto::clear_mask::depth);


	r.present ();
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