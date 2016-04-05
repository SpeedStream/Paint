/* This program illustrates the use of the glut library for
interfacing with a Window System */

/* The program opens a window, clears it to black,
then draws a box at the location of the mouse each time the
left button is clicked. The right button exits the program

The program also reacts correctly when the window is
moved or resized by clearing the new window to black*/

#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/* globals */

GLsizei wh = 500, ww = 500; /* initial window size */
GLfloat size = 3.0;   /* half side length of square */


void drawSquare(int x, int y)
{

	  y=wh-y;
	  glColor3ub( (char) rand()%256, (char) rand()%256, (char) rand()%256);
	  glBegin(GL_POLYGON);
	 	    glVertex2f(x+size, y+size);
		    glVertex2f(x-size, y+size);
	 	    glVertex2f(x-size, y-size);
	    	glVertex2f(x+size, y-size);
	  glEnd();
	  glFlush();
}


/* rehaping routine called whenever window is resized
or moved */

void reshape(GLsizei w, GLsizei h)
{

/* adjust clipping box */

   	glMatrixMode(GL_PROJECTION);
   	glLoadIdentity(); 
   	glOrtho(0.0, (GLdouble)w, 0.0, (GLdouble)h, -1.0, 1.0);
   	glMatrixMode(GL_MODELVIEW);
   	glLoadIdentity(); 

/* adjust viewport and clear */

	glViewport(0,0,w,h);
    glClearColor (0.0, 0.0, 0.0, 1.0);
   	glClear(GL_COLOR_BUFFER_BIT);
   	glFlush();

/* set global size for use by drawing routine */

	ww = w;
   	wh = h; 
}

void init(void)
{

   	glViewport(0,0,ww,wh);

/* Pick 2D clipping window to match size of screen window 
This choice avoids having to scale object coordinates
each time window is resized */

   	glMatrixMode(GL_PROJECTION);
   	glLoadIdentity(); 
   	glOrtho(0.0, (GLdouble) ww , 0.0, (GLdouble) wh , -1.0, 1.0);

/* set clear color to black and clear window */

   	glClearColor (0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

void mouse(int btn, int state, int x, int y)
{
//    if(btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)   exit(0);
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN)   drawSquare(x,y);
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
      case 'Q':
      case 'q':
         exit(0);
         break;
   }
}

/* display callback required by GLUT 3.0 */

void display(void)
{}

void demo_menu(int id)
{
    switch(id)
    {
    case 0: exit(0);
        break;
    case 1: glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
        break;
    case 2: size = 2 * size;
        break;
    case 3: if (size > 1) size = size/2;
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    int sub_menu;

   	glutInit(&argc,argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow("square");
   	init();
   	glutReshapeFunc (reshape);
   	glutMouseFunc (mouse);
  	glutMotionFunc(drawSquare);
	glutDisplayFunc(display);
    glutKeyboardFunc (keyboard);
 
    sub_menu = glutCreateMenu(demo_menu); 		
    glutAddMenuEntry("increase square size",2); 	
    glutAddMenuEntry("decrease square size",3); 	
	
	glutCreateMenu(demo_menu); 	
    glutAddMenuEntry("quit",0); 
    glutAddMenuEntry("clear",1); 
    glutAddSubMenu("resize", sub_menu); 	
    glutAttachMenu(GLUT_RIGHT_BUTTON); 

   	glutMainLoop();
    return 0;
}