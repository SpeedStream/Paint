/*ITESM CCV GRÁFICAS COMPUTACIONALES */
#define NULL 0
#include <GL/glut.h>
#include <fstream>

void menu_derecho( int );
void menu_color( int );
void menu_relleno( int );

void mouse( int, int, int, int );
void display( void );
void clear_window();
void myReshape( GLsizei, GLsizei );

void myinit( void );
void drawLineLoop( int, int [], int [] );
void screen_box( int, int, int );
int pick( int, int);1

/***    Variables Globales  ***/
GLsizei windowHeight = 500, windowWidth = 500;  //Tamaño Inicial
GLfloat red = 1.0, green = 1.0, blue = 1.0;     //Color de dibujado
int fill = 1;                                   //Bandera de relleno

/* Aaron Santos */
int first_pick_color = 99;                      //Primer color a elegir cuando se dibuje.
/* ./Aaron Santos */

/***    Dibuja un Poligono con relleno  ***/
void drawPolygon( int n, int *x, int *y ) {
        int i;
        glBegin( GL_POLYGON ); 
           for (i = 0; i < n; i++ ) {
              glVertex2i( x[i], y[i] );
           }
        glEnd( );
}


/***    Dibuja una linea x1, y1 to x2, y2 ***/
void drawLine( int x1, int y1, int x2, int y2) {

        /* Your task is to rewrite this using only
        glBegin( GL_POINTS );
            glVertex2i( x, y );
        glEnd();
        */

        glBegin( GL_LINES ); 
           glVertex2i( x1, y1 );
           glVertex2i( x2, y2 );
        glEnd( );
}

/***    Llamada de retorno para la remodelación de la ventana cuando se redimensionan o movido  ***/
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

/***    Inicialización  ***/
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

/***    Dibuja ciclo de linea con n lineas ***/
void drawLineLoop( int n, int *x, int *y ) {

   int i;

   drawLine( x[n-1], y[n-1], x[0], y[0] );
   for (i = 0; i < n-1; i++ ) {
      drawLine( x[i], y[i], x[i+1], y[i+1] );
   }
}

/***    Llamada de retorno para la interacción del ratón ***/
void mouse( int button, int state, int x, int y ) {

   static int draw_mode = 4;    //Modo de dibujado
   static int count;            //Contador de clic realizados en este modo de dibujado
   static int xp[4], yp[4];     //Mantiene Ubicación de clics realizados en este modo de dibujo
   int where;                   //Donde fueron hechos los clics (Panel de dibujo, Panel Herramientas)
   printf("draw_mode: %i\n", draw_mode);

   if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {

      //Guarda todos los atributos actuales
      glPushAttrib( GL_ALL_ATTRIB_BITS );
       
      where = pick( x, y);

      //Usa el color actual
      glColor3f( red, green, blue );

      //No hay un modo de dibujo seleccionado
      if ( where != 0 ) {
         count = 0;
         draw_mode = where;
      } else if ( draw_mode == 1  &&  count == 0 ) {//Dibuja una linea en el punto de partida
         count = 1;
         xp[0] = x;
         yp[0] = windowHeight - y;
        //Dibuja linea punto final
      
      } else if ( draw_mode == 1  &&  count != 0 ) {

         drawLine( xp[0], yp[0], x, windowHeight - y );

         /* Desactivar el modo de drenaje cuando se hace */
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
         glPointSize( 3.0 );
         glBegin( GL_POINTS );
            glVertex2f( x , y );
         glEnd( );

         count++;
      }

      glPopAttrib( );
      glFlush( );
   }

}

/***    Seleccion de la herramienta a dibujar   ***/
int pick( int x, int y) {
    y = windowHeight - y;
    /* Aaron Santos */
    if(first_pick_color == 99){
        menu_color(8);
        first_pick_color = 100;
    }
    /* ./Aaron Santos */
    if ( y < windowHeight - windowWidth / 10 )  //No se dibuja nada PANEL HERRAMIENTAS
        return 0;
    else if ( x < windowWidth / 10)             //Dibuja Linea
        return 1;
    else if ( x < windowWidth / 5)              //Dibuja Rectángulo
        return 2;
    else if ( x < 3 * windowWidth/ 10)          //Dibuja Triángulo
        return 3;
    else if ( x < 2 * windowWidth/ 5)           //Dibuja Punto
        return 4;
    else                                        //Nada que dibujar
        return 0;

}

/***    Dibuja las cajas que representan las opciones a elegir para dibujar ***/
void screen_box( int x, int y, int length ) {

    glBegin( GL_QUADS );
        glVertex2i( x, y );
        glVertex2i( x + length, y );
        glVertex2i( x + length, y + length );
        glVertex2i( x, y + length );
    glEnd( );

}

/***    Guarda la imagen    ***/ //FALTA REVISAR FORMATO
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

/***    Toma los valores de clic derecho ***/
void menu_derecho( int id ) {
   if ( id == 1 ) {
      exit(1);      /* Terminar Programa */
   } else if ( id == 3 ) {
        screenshot("test.tga",800,600);
        }else if ( id == 2 ){
            clear_window(); /* Limpiar dibujo*/
   }
    glutPostRedisplay();
}

/***    Toma la seleccion del color ***/
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

/***    Toma la seleccion del relleno   ***/
void menu_relleno( int id ) {

   if ( id == 1 ) {
       fill = 1;    /* Encendido */
   } else {
       fill = 0;    /* Apagado */
   }

}

void clear_window(){
    /***    Limpia la pantalla ***/
    glClearColor ( 1.0, 1.0, 1.0, 1.0 );
    glClear( GL_COLOR_BUFFER_BIT );
}

/***    Inicializa y borra la pantalla  ***/ 
void display( void ) {

   /*** Guardar los atributos ***/
   glPushAttrib( GL_ALL_ATTRIB_BITS );

   /*** Cuadro seleccion dibuja Lineas ***/
   glColor3f( 1.0, 1.0, 1.0 );
   screen_box( 0, windowHeight - windowWidth / 10, windowWidth / 10 );

   /*** Cuadro seleccion dibujar Rectangulos***/ 
   glColor3f( 1.0, 0.0, 0.0 );
   screen_box( windowWidth / 10, windowHeight - windowWidth / 10, windowWidth / 10 );

   /*** Cuadro seleccion dibujar Triangulos ***/ 
   glColor3f( 0.0, 1.0, 0.0 );
   screen_box( windowWidth / 5, windowHeight - windowWidth / 10, windowWidth / 10 );

   /*** Cuadro seleccion dibujar Puntos ***/ 
   glColor3f( 0.0, 0.0, 1.0 );
   screen_box( 3 * windowWidth / 10, windowHeight - windowWidth / 10, windowWidth / 10 );



   /***Cuadrado en cuadro seleccion, indica dibujado de rectangulo***/
   glColor3f( 0.0, 0.0, 0.0 );
   screen_box( windowWidth / 10 + windowWidth / 40, 
               windowHeight - windowWidth / 10 + windowWidth / 40, 
               windowWidth / 20 );


    /***Linea en cuadro seleccion, indica dibujado de linea***/
   glBegin( GL_LINES );
      glVertex2i( windowHeight / 40, windowHeight - windowWidth / 20 );
      glVertex2i( windowHeight / 40 + windowWidth / 20, 
                  windowHeight - windowWidth / 20 );
   glEnd( );

   /***Triángulo en cuadro seleccion, indica dibujado de triángulo***/
   glBegin( GL_TRIANGLES );
      glVertex2i( windowWidth / 5 + windowWidth / 40, 
                  windowHeight - windowWidth / 10 + windowWidth / 40 );
      glVertex2i( windowWidth / 5 + windowWidth / 20, 
                  windowHeight - windowWidth / 40 );
      glVertex2i( windowWidth / 5 + 3 * windowWidth / 40, 
                  windowHeight - windowWidth / 10 + windowWidth / 40 );
   glEnd( );

   /***Linea en cuadro seleccion, indica dibujado de punto***/
   glPointSize( 3.0 );
   glBegin( GL_POINTS );
      glVertex2i( 3 * windowWidth / 10 + windowWidth / 20, 
                  windowHeight - windowWidth / 20 );
   glEnd( );

   glFlush( );
   glPopAttrib( );

}

/*******************************************
*************PROGRAMA PRINCIPAL*************    
*******************************************/
int main( int argc, char** argv ) {

   int c_menu, f_menu;

   glutInit( &argc, argv );
   glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );
   glutCreateWindow( "Paint" );
   myinit();
   glutReshapeFunc ( myReshape ); 
   glutMouseFunc ( mouse );
    //MODIFICAR FUNCION DISPLAY
   glutDisplayFunc( display );

   /*** Crea el submenu de colores***/
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

   /*** Crea Menu con click derecho ***/
   glutCreateMenu( menu_derecho );
   glutAddSubMenu( "Colores", c_menu );
   glutAddSubMenu( "Relleno", f_menu );
   glutAddMenuEntry( "Borrar", 2 );
   glutAddMenuEntry( "Quitar",  1 );
   glutAddMenuEntry( "Guardar",  3 );
   glutAttachMenu( GLUT_RIGHT_BUTTON );

   
   glutMainLoop( );

   return 0;
}
