/*ITESM CCV GRÁFICAS COMPUTACIONALES */
#define NULL 0
#define PI 3.1416
#include <GL/glut.h>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

void menu_derecho( int );
void menu_color( int );
void menu_relleno( int );
void menu_size( int );

void mouse( int, int, int, int );
void display( void );
void clear_window();
void myReshape( GLsizei, GLsizei );

void myinit( void );
void drawLineLoop( int, int [], int [] );
void screen_box( int, int, int );
int pick( int, int );

/*  Funciones para dibujar del usuario */

void userDrawLine(int , int, GLfloat);
void userDrawCircle(int, int, GLfloat);
void userDrawSquare(int, int, GLfloat);
void userDrawTriangle(int, int, GLfloat);
void userDrawPencil(int, int, GLfloat);


/*  END Funciones para dibujar del usuario */

/***	Variables Globales	***/
GLsizei windowHeight = 700, windowWidth = 1200; 	//Tamaño Inicial
GLfloat red = 0.0, green = 0.0, blue = 0.0;    	//Color de dibujado
int fill = 0;			               			//Bandera de relleno
void *currentfont;								//Fuente Actual
GLfloat size = 1.0;                             //Tamaño de grosor de dibujo -> ACTUALMENTE SOLO TRABAJA EN userDrawLine


/***	Dibujar Texto	***/
void drawstring(float x,float y,float z,char *string){
    char *c;
    glRasterPos3f(x,y,z);

    for(c=string;*c!='\0';c++)
    {
        glutBitmapCharacter(currentfont,*c);
    }
}
void setFont(void *font){
    currentfont=font;
}

/***	Dibuja un Poligono con relleno	***/
void drawPolygon( int n, int *x, int *y ) {

        glBegin( GL_POLYGON ); 
           for (int i = 0; i < n; i++ ) {
              glVertex2i( x[i], y[i] );
           }
        glEnd( );
}

/***	Dibuja una linea x1, y1 to x2, y2 ***/
void drawLine( int x1, int y1, int x2, int y2) {
    glLineWidth (size);
    glBegin( GL_LINES );
    glVertex2i( x1, y1 );
    glVertex2i( x2, y2 );
    glEnd( );
}

void drawCircle(GLfloat x, GLfloat y, GLfloat radius){
  int i;
  int lineAmount = 100; //# of triangles used to draw circle
  
  //GLfloat radius = 0.8f; //radius
  GLfloat twicePi = 2.0f * PI;
  glBegin(GL_LINE_LOOP);
    for(i = 0; i <= lineAmount;i++) { 
      glVertex2f(
          x + (radius * cos(i *  twicePi / lineAmount)), 
          y + (radius* sin(i * twicePi / lineAmount))
      );
    }
  glEnd();
}

/***	Llamada de retorno para la remodelación de la ventana cuando se redimensionan o movido	***/
void myReshape( GLsizei width, GLsizei height ) {

   /* ajusta el área de dibujado y herramientas */
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity( ); 
   glOrtho( 0.0, (GLdouble) width, 0.0, (GLdouble) height, -1.0, 1.0 );

   /* Ajustar ventana */
   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity( ); 
   glViewport( 0, 0, width, height );

   /* Limpiar Ventana */
   glClearColor ( 1.0, 1.0, 1.0, 1.0 );
   glClear( GL_COLOR_BUFFER_BIT );

   //glutPostRedisplay( );
   glFlush( );

   /* Establecer el tamaño global para el uso de rutina de dibujo */
   windowWidth = width;
   windowHeight = height; 
}

/***	Inicialización	***/
void myinit( void ) {
   /* Set viewport, GL_MODELVIEW matrix is default */
   glViewport( 0, 0, windowWidth, windowHeight );

   /* Establecer zona de dibujo en 2D para que coincida con el tamaño 
	de la ventana de X, esta opción evita tener que escalar las coordenadas
	del objeto de cada ventana de tiempo se cambia el tamaño 
    */
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity( ); 
   glOrtho( 0.0, (GLdouble) windowWidth , 0.0, 
          (GLdouble) windowHeight , -1.0, 1.0 );

   /* Establecer color claro a negro y clara ventana de la pantalla */
   glClearColor ( 0.0, 0.0, 0.0, 1.0 );
   glClear( GL_COLOR_BUFFER_BIT );

   /* Flush GL buffers to the display (screen window) */
   glFlush( );
}

/***	Dibuja ciclo de linea con n lineas ***/
void drawLineLoop( int n, int *x, int *y ) {

   int i;

   drawLine( x[n-1], y[n-1], x[0], y[0] );
   for (i = 0; i < n-1; i++ ) {
      drawLine( x[i], y[i], x[i+1], y[i+1] );
   }
}

/*   asads   */
void userDrawLine(int x, int y){
    y=windowHeight-y;
    //glColor3ub( (char) rand()%256, (char) rand()%256, (char) rand()%256); -> Color interno aleatorio
    //glColor3ub(r, g, b);
    glBegin(GL_POLYGON);
    glVertex2f(x+size, y+size);
    glVertex2f(x-size, y+size);
    glVertex2f(x-size, y-size);
    glVertex2f(x+size, y-size);
    glEnd();
    glFlush();
}



/***	Llamada de retorno para la interacción del ratón ***/
void mouse( int button, int state, int x, int y ) {
   static int draw_mode = 6;    //Modo de dibujado
   static int count;			//Contador de clic realizados en este modo de dibujado
   static int xp[4], yp[4];		//Mantiene Ubicación de clics realizados en este modo de dibujo
   int where;					//Donde fueron hechos los clics (Panel de dibujo, Panel Herramientas)

   if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {

        //Guarda todos los atributos actuales
        glPushAttrib( GL_ALL_ATTRIB_BITS );

        where = pick( x, y );

        //Usa el color actual
        glColor3f( red, green, blue );

        //No hay un modo de dibujo seleccionado
        if ( where != 0 ) {

            count = 0;
            draw_mode = where;
            glutMotionFunc(0);          //Cerramos el motion
        } else if ( draw_mode == 1  &&  count == 0 ) {//Dibuja una linea en el punto de partida
            count = 1;
            xp[0] = x;
            yp[0] = windowHeight - y;
            //Dibuja linea punto final
        } else if ( draw_mode == 1  &&  count != 0 ) {
            drawLine( xp[0], yp[0], x, windowHeight - y );
            count = 0;

            /* Dibujar rectángulo - Punto de partida */
        } else if ( draw_mode == 2  &&  count == 0 ) {
            count = 1;
            xp[0] = x;
            yp[0] = windowHeight - y;

            /* Dibujar rectángulo - el punto final - dibujar rectángulo  */
        } else if ( draw_mode == 2  &&  count != 0 ) {
            /* Rellene puntos en las esquinas del rectángulo */
            xp[1] = xp[0]; yp[1] = windowHeight - y;
            xp[2] = x;     yp[2] = windowHeight - y;
            xp[3] = x;     yp[3] = yp[0];
            if ( fill ) {
                drawPolygon( 4, xp, yp );
            } else {
                drawLineLoop( 4, xp, yp );
            }

            /* Desactivar el modo de drenaje cuando se hace */
            count = 0;

            /* Dibujar triángulo punto inicial*/
        } else if ( draw_mode == 3  &&  count == 0 ) {
            count = 1;
            xp[0] = x;
            yp[0] = windowHeight - y;

            /* Dibujar triángulo punto medio */
        } else if ( draw_mode == 3  &&  count == 1 ) {
            count = 2;
            xp[1] = x;
            yp[1] = windowHeight - y;

            /* Dibuje el triángulo - el punto final - elaborar triángulo */
        } else if ( draw_mode == 3  &&  count == 2 ) {
            xp[2] = x;     yp[2] = windowHeight - y;
            if ( fill ) {
                drawPolygon( 3, xp, yp );
            } else {
                drawLineLoop( 3, xp, yp );
            }
            /* Desactivar el modo de drenaje cuando se hace */
            count = 0;
            /*** Dibuja Puntos ***/
        } else if ( draw_mode == 4 ) {
            y = windowHeight - y;
            //glPointSize( 3.0 );
            glBegin( GL_POINTS );
            glVertex2f( x , y );
            glEnd( );

            count++;
        }else if(draw_mode == 5 && count == 0){
            count = 1;
            xp[0]=x;
            yp[0] = windowHeight - y;
        }else if(draw_mode == 5 && count == 1){
            xp[1]=x;
            yp[1] = windowHeight - y;
            int rx= xp[1]-xp[0];
            int ry = yp[1]-yp[0];
            rx= pow(rx,2);
            ry = pow (ry,2);
            int radio = sqrt(rx+ry);

            drawCircle(xp[0],yp[0],radio);
            count = 0;
            /* INCIO DE DIBUJO CON LAPIZ */
        }else if ( draw_mode == 6) {
            //glColor3ub(red, blue, green);
            userDrawLine(x, y);                  // Funcion de dibujo de lineas
            glutMotionFunc(userDrawLine);       // Abrimos el motion sobre la funcion de userDrawLine
        }
        /* FIN DE DIBUJO CON LAPIZ */
        glPopAttrib( );
        glFlush( );
    }

}

/***	Seleccion de la herramienta a dibujar	***/
int pick( int x, int y ) {
   if ( y < 40 ) 	//No se dibuja nada PANEL HERRAMIENTAS
      return 0;  
   else if ( x < 30 && x > 0)            		//Dibuja Linea
      return 1;  
   else if ( x < 60 && x > 30)             		//Dibuja Rectángulo
      return 2;  
   else if ( x < 90 && x > 60)         			//Dibuja Triángulo
      return 3;  
   else if ( x < 120 && x > 90)          		//Dibuja Punto
      return 4; 
   else if ( x < 150 && x > 120)				//Dibuja Circulo
      return 5;
   else if ( x < 180 && x > 150)                //Dibuja con lapiz
      return 6;
   else 										//Nada que dibujar
      return 0; 
}

/***	Dibuja las cajas que representan las opciones a elegir para dibujar ***/
void screen_box( int x, int y, int length ) {

    glBegin( GL_QUADS );
        glVertex2i( x, y );
        glVertex2i( x + length, y );
        glVertex2i( x + length, y + length );
        glVertex2i( x, y + length );
    glEnd( );
}

/***	Guarda la imagen	***/ //FALTA REVISAR FORMATO
void screenshot (char filename[160],int x, int y){
	// get the image data
    long imageSize = x * y * 3;
    unsigned char *data = new unsigned char[imageSize];
    glReadPixels(0,0,x,y, GL_BGR,GL_UNSIGNED_BYTE,data);// split x and y sizes into bytes
    int xa= x % 256;
    int xb= (x-xa)/256;int ya= y % 256;
    int yb= (y-ya)/256;//assemble the header
    unsigned char header[18]={0,0,2,0,0,0,0,0,0,0,0,0,(char)xa,(char)xb,(char)ya,(char)yb,24,0};

    // write header and data to file
    std::fstream File;
    //File.open (filename, std::fstream::in | std::fstream::out | std::fstream::app);
    File.open(filename, std::ios::out | std::ios::binary);
    //File.write (reinterpret_cast(header), sizeof (char)*18);
    //File.write (reinterpret_cast(data), sizeof (char)*imageSize);
    //File.write(reinterpret_cast (header), sizeof(char)*18);
    //File.write(reinterpret_cast (data), sizeof(char)*imageSize);
    File.write(reinterpret_cast<char *>(header), sizeof (char)*18);
    File.write(reinterpret_cast<char *>(data), sizeof (char)*imageSize);
    File.close();

    delete[] data;
    data=NULL;
}

/***	Toma los valores de clic derecho ***/
void menu_derecho( int id ) {
   if ( id == 1 ) {
      exit(1);  	/* Terminar Programa */
   } else if ( id == 3 ) {
		screenshot("test.tga",800,600);	
		}else if ( id == 2 ){
			clear_window();	/* Limpiar dibujo*/
   }
	glutPostRedisplay();
}

/***	Toma la seleccion del color	***/
void menu_color( int id ) {
   if ( id == 1 ) { 
      red = 1.0; green = 0.0; blue = 0.0; /* red     */
   } else if ( id == 2) { 
      red = 0.0; green = 1.0; blue = 0.0; /* green   */
   } else if ( id == 3) { 
      red = 0.0; green = 0.0; blue = 1.0; /* blue    */
   } else if ( id == 4) { 
      red = 0.0; green = 1.0; blue = 1.0; /* cyan    */
   } else if ( id == 5) { 
      red = 1.0; green = 0.0; blue = 1.0; /* magenta */
   } else if ( id == 6) { 
      red = 1.0; green = 1.0; blue = 0.0; /* yellow  */
   } else if ( id == 7) { 
      red = 1.0; green = 1.0; blue = 1.0; /* white   */
   } else if ( id == 8) { 
      red = 0.0; green = 0.0; blue = 0.0; /* black   */
   }
}

/***	Toma la seleccion del relleno	***/
void menu_relleno( int id ) {

   if ( id == 1 ) {
       fill = 1; 	/* Encendido */
   } else {
       fill = 0;	/* Apagado */
   }
}

void clear_window(){
	/***	Limpia la pantalla ***/
	glClearColor ( 1.0, 1.0, 1.0, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT );
}

void menu_size( int id){
    printf("Actual size: %f\n", size);
    if( id == 0){
        size = size * 2;
    }
    if( id == 1){
        if(size > 1){
            size = size / 2;
        }
    }
    printf("New size: %f\n", size);
}

/***	Inicializa y borra la pantalla 	***/ 
void display( void ) {
	/* SIMULA PANEL*/
    static int px[4], py[4];
	
	px[0]=0; py[0]=0;
	px[1]=0; py[1]=50;
	px[2]=windowWidth; py[2]=50;
	px[3]=windowWidth; py[3]=0;
	
    glColor3f( 0.91, 0.91, 0.91 );
    drawPolygon( 4, px, py);
   
   setFont(GLUT_BITMAP_HELVETICA_18);
   
   glColor3f( 1.0, 0.0, 1.0 );
   screen_box( 0, 10, 30 );

   /*** Guardar los atributos ***/
   glPushAttrib( GL_ALL_ATTRIB_BITS );

   /*CUADROS DE HERRAMIENTAS*/

        /*** Herramienta de dibuja Lineas ***/
        glColor3f( 1.0, 0.0, 1.0 );
        screen_box( 0, 10, 30 );

        /*** Herramienta de dibujar Rectangulos***/ 
        glColor3f( 1.0, 0.0, 0.0 );
        screen_box( 30, 10, 30  );

        /*** Herramienta de dibujar Triangulos ***/ 
        glColor3f( 0.0, 1.0, 0.0 );
        screen_box( 60, 10, 30 );

        /*** Herramienta de dibujar Puntos ***/ 
        glColor3f( 0.0, 0.0, 1.0 );
        screen_box( 90, 10, 30  );

        /*** Herramienta de dibujar Circulo***/
        glColor3f( 0.2, 0.7, 0.3 );
        screen_box( 120,10,30 );

        /*** Herramienta de dibujar a mano***/
        glColor3f( 0.5, 0.7, 0.3 );
        screen_box( 150,10,30 );

    /*END CUADROS DE HERRAMIENTAS*/


	glColor3f( 0.0, 0.0, 0.0 );
	drawstring(300,10, 0.0, "PAINT ITESM CCV");

    /*DIBUJOS EN CUADROS DE HERRAMIENTAS*/

        /***Cuadrado para dibujado de rectangulo***/
        glColor3f( 0.0, 0.0, 0.0 );
        screen_box( 33,12,24 );

        /***Linea para dibujado de linea***/
        glBegin( GL_LINES );
        glVertex2i( 1,11 );
        glVertex2i( 29,39 );
        glEnd( );

        /***Triángulo para dibujado de triángulo***/
        glBegin( GL_TRIANGLES );
        glVertex2i( 63, 12  );
        glVertex2i( 87, 12 );
        glVertex2i( 87, 38 );
        glEnd( );

        /***Punto para dibujado de punto***/
        //glPointSize( 3.0 );
        glBegin( GL_POINTS );
        glVertex2i( 105,20 );
        glEnd( );

        /***Circulo para dibujado de circulo***/
        //glPointSize( sizeD );
        drawCircle(135,25,10);

        /***Lapiz para dibujado a mano***/
        //glPointSize( sizeD );
        drawCircle(165,25,10);

    /*END DIBUJOS EN CUADROS DE HERRAMIENTAS*/

   glFlush( );
   glPopAttrib( );
}

/*******************************************
*************PROGRAMA PRINCIPAL*************	
*******************************************/
int main( int argc, char** argv ) {

    int c_menu, f_menu, s_menu;

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(windowWidth,windowHeight);
    glutInitWindowPosition(10,10);
    glutCreateWindow( "Paint" );
    myinit();
    glutReshapeFunc ( myReshape ); 
	//MODIFICAR FUNCION DISPLAY
    glutMouseFunc ( mouse );
    glutDisplayFunc( display );

    s_menu = glutCreateMenu( menu_size );
    glutAddMenuEntry ("Incrementar", 0);
    glutAddMenuEntry ("Disminuir", 1);

   /***	Crea el submenu de colores***/
    c_menu = glutCreateMenu( menu_color );
    glutAddMenuEntry( "Red",     1 );
    glutAddMenuEntry( "Green",   2 );
    glutAddMenuEntry( "Blue",    3 );
    glutAddMenuEntry( "Cyan",    4 );
    glutAddMenuEntry( "Magenta", 5 );
    glutAddMenuEntry( "Yellow",  6 );
    glutAddMenuEntry( "White",   7 );
    glutAddMenuEntry( "Black",   8 );

   /*** Crea el submenu de relleno ***/
   f_menu = glutCreateMenu( menu_relleno );
   glutAddMenuEntry( "Relleno Encendido", 1 );
   glutAddMenuEntry( "Relleno Apagado", 2 );

   /*** Crea Menu con click derecho	***/
   glutCreateMenu( menu_derecho );
   glutAddSubMenu( "Colores", c_menu );
   glutAddSubMenu( "Relleno", f_menu );
   glutAddSubMenu ( "Size", s_menu);
   glutAddMenuEntry( "Borrar", 2 );
   glutAddMenuEntry( "Quitar",  1 );
   glutAddMenuEntry( "Guardar",  3 );
   glutAttachMenu( GLUT_RIGHT_BUTTON );

   
   glutMainLoop( );

   return 0;
}