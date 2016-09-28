// **********************************************************************
//	DesenhaPontos-Linhas.cpp
//  Programa de testes com OpenGL 3D
//	Marcio Sarroglia Pinho
//  pinho@pucrs.br
// **********************************************************************

#include <stdio.h>
#include <stdlib.h>
#include "filemanipulation.h"
#include "global.h"

#ifdef WIN32
#include <windows.h>    // includes only in MSWindows not in UNIX
#include "gl/glut.h"
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

typedef struct
{
    float x,y,z;
} Ponto;

GLfloat ratio;
GLfloat angY, angX;

Ponto Marcadores3D[TAM_MAX];
Segmento Ligacoes[TAM_MAX];
Ponto Obs, Alvo;
Ponto ObsIni;

Frames* frames;
Segments* segments;

double actualTime = 0;
int actualFrame = 0;
int segmentsQtn;
int lastFrame;
int markers;

int width,height;
float deltax=0, deltay=0;
GLfloat angle=60, fAspect=1.0;
GLfloat rotX=0, rotY=0, rotX_ini=0, rotY_ini=0;
int x_ini=0,y_ini=0,bot=0;

// **********************************************************************
//  Monta os marcadores do frame atual
// **********************************************************************
void MountActualFrame(Frames* frames)
{
    memset(Marcadores3D,0,sizeof(Marcadores3D));

    lastFrame = frames->numberOfFrames -1;
    Frame f;

    f = *frames->frames[actualFrame];
    markers = f.numberOfMarkers;

    for(int i = markers-1; i>=0; i--)
    {
        int j = f.markers[i]->id;
        Marcadores3D[j].x = (float)f.markers[i]->x;
        Marcadores3D[j].y = (float)f.markers[i]->y;
        Marcadores3D[j].z = (float)f.markers[i]->z;
    }
}

// **********************************************************************
//  Define os segmentos para o filme escolhido
// **********************************************************************
void setSegments(Segments* segments)
{
    segmentsQtn = segments->numberOfSegments;

    for(int i = segmentsQtn-1; i>=0; i--)
    {
        Ligacoes[i].verticeFinal = segments->segments[i]->verticeFinal;
        Ligacoes[i].verticeInicial = segments->segments[i]->verticeInicial;
    }
}

// **********************************************************************
//  Define a posição de observação inicial
// **********************************************************************
void setObservatorPosition()
{
    Obs.x = 0;
    Obs.y = 0.5;
    Obs.z = -2;
    rotX = 50;
    rotY = 180;
}

// **********************************************************************
//  Desenha um ponto em 3D
// **********************************************************************
void DesenhaPonto(Ponto P)
{
    glPointSize(3);
    glBegin(GL_POINTS);
    glVertex3f(P.x, P.y, P.z);
    glEnd();
}
// **********************************************************************
//  Desenha um vetor de pontos
// **********************************************************************
void DesenhaVetorPontos(Ponto V[], int qtd)
{
    int i;
    for (i=0; i<qtd; i++)
    {
        DesenhaPonto(V[i]);
    }
}
// **********************************************************************
//  Desenha linha entre dois pontos
// **********************************************************************
void DesenhaSegmento(Ponto P1, Ponto P2)
{
    glBegin(GL_LINES);
    glVertex3f(P1.x, P1.y, P1.z);
    glVertex3f(P2.x, P2.y, P2.z);
    glEnd();

}
// **********************************************************************
//  Desenha um vetor de pontos
// **********************************************************************
void DesenhaVetorSegmentos(Segmento V[], int qtd)
{
    //printf("=========\n");
    Ponto P1, P2;
    int inicio, fim;
    int i;
    for (i=0; i<qtd; i++)
    {
        inicio = Ligacoes[i].verticeInicial;
        fim = Ligacoes[i].verticeFinal;
        P1 = Marcadores3D[inicio];
        P2 = Marcadores3D[fim];
        DesenhaSegmento(P1, P2);
        //printf("%d %d\n", inicio, fim);
    }
}

// **********************************************************************
//  Desenha um quadriculado para representar um piso
// **********************************************************************
void DesenhaPiso()
{
    const float LARG = 4.0;
    int qtd = 40;
    float delta = (2*LARG)/(qtd-1);
    float z = -LARG;

    int i;
    for (i=0; i<qtd; i++)
    {
        glBegin(GL_LINES);
        glVertex3f(-LARG,0,z);
        glVertex3f(LARG,0,z);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(z,0,-LARG);
        glVertex3f(z,0,LARG);
        glEnd();

        z += delta;
    }

}
// **********************************************************************
//  Desenha os eixos coordenados
// **********************************************************************
void DesenhaEixos()
{
    glBegin(GL_LINES);
    glColor3f(1,0,0); // vermelho

    glVertex3f(0,0,0); // Eixo X
    glVertex3f(0,0,0);

    glVertex3f(0,0,0); // Eixo Y
    glVertex3f(0,0,0);

    glVertex3f(0,0,0); // Eixo X
    glVertex3f(0,0,0);
    glEnd();

}

// Função callback para eventos de botões do mouse
void mouse(int button, int state, int x, int y)
{
    if(state==GLUT_DOWN)
    {
        // Salva os parâmetros atuais
        x_ini = x;
        y_ini = y;
        ObsIni = Obs;
        rotX_ini = rotX;
        rotY_ini = rotY;
        bot = button;
    }
    else bot = -1;
}

// Função callback para eventos de movimento do mouse
#define SENS_ROT	5.0
#define SENS_OBS	20.0
void move(int x, int y)
{
    // Botão esquerdo ?
    if(bot==GLUT_LEFT_BUTTON)
    {
        // Calcula diferenças
        int deltax = x_ini - x;
        int deltay = y_ini - y;
        // E modifica ângulos
        rotY = rotY_ini - deltax/SENS_ROT;
        rotX = rotX_ini - deltay/SENS_ROT;
    }
    // Botão direito ?
    else if(bot==GLUT_RIGHT_BUTTON)
    {
        // Calcula diferença
        int deltaz = y_ini - y;
        // E modifica distância do observador
        //Obs.x = x;
        //Obs.y = y;
        Obs.z = ObsIni.z - deltaz/SENS_OBS;
    }
    //PosicionaObservador();
    glutPostRedisplay();
}

// **********************************************************************
//  void PosicUser()
//
// **********************************************************************
void PosicUser()
{
    // Set the clipping volume
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90,ratio,0.01,20);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Especifica posição do observador e do alvo
    glTranslatef(Obs.x, Obs.y, Obs.z);
    glRotatef(rotX,1,0,0);
    glRotatef(rotY,0,1,0);

}
// **********************************************************************
//  void reshape( int w, int h )
//		trata o redimensionamento da janela OpenGL
//
// **********************************************************************
void reshape( int w, int h )
{
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h == 0)
        h = 1;

    ratio = 1.0f * w / h;
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    PosicUser();

}

// **********************************************************************
//  void display( void )
//
//
// **********************************************************************
void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    PosicUser();

    glMatrixMode(GL_MODELVIEW);

    glRotatef(30,0,1,0); // Gira o cenario todo
    DesenhaEixos();
    glColor3f(0.0f,0.0,1.0f); // azul
    DesenhaPiso();

    glPushMatrix();
    //glRotatef(angX,1,0,0);
    glRotatef(angY,0,1,0);
    glColor3f(1.0f,1.0f,0.0f); // Verde

    /* == Passando a quantidade de marcadores do frame por parametro
        para evitar problema nos frames com marcadores a menos == */
    DesenhaVetorPontos(Marcadores3D, markers);
    glColor3f(1.0f,0.0f,0.0f);
    DesenhaVetorSegmentos(Ligacoes, segmentsQtn);
    glPopMatrix();

    glutSwapBuffers();
}

// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
//
//
// **********************************************************************
void keyboard ( unsigned char key, int x, int y )
{
    switch ( key )
    {
    case 27:        // Termina o programa qdo
        exit ( 0 );   // a tecla ESC for pressionada
        break;

    default:
        break;
    }
}

// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
//
//
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y )
{
    float passo = 3.0;
    switch ( a_keys )
    {
    case GLUT_KEY_RIGHT:
        angY += passo;
        if (angY > 360)
            angY = 0;
        glutPostRedisplay();
        break;
    case GLUT_KEY_LEFT:
        angY -= passo;
        if (angY < 0)
            angY = 360;
        glutPostRedisplay();
        break;
    case GLUT_KEY_F1:
        actualFrame = 0;
        MountActualFrame(frames);
        display();
        break;
    case GLUT_KEY_F2:
        if(actualFrame > 0)
        {
            actualFrame--;
            MountActualFrame(frames);
            display();
        }
        break;
    case GLUT_KEY_F3:
        actualFrame = 0;
        while(actualFrame <= lastFrame)
        {
            int sleepTime = (int)((frames->frames[actualFrame]->time - actualTime) * 1000);

            actualTime = frames->frames[actualFrame]->time;

            MountActualFrame(frames);
            display();
            actualFrame++;

            Sleep(sleepTime);
        }
        break;
    case GLUT_KEY_F4:
        if(actualFrame < lastFrame)
        {
            actualFrame++;
            MountActualFrame(frames);
            display();
        }
        break;
    case GLUT_KEY_F5:
        actualFrame = lastFrame;
        MountActualFrame(frames);
        display();
        break;
    default:
        break;
    }
}
// **********************************************************************
//  void init(void)
//		Inicializa os parâmetros globais de OpenGL
//
// **********************************************************************
void init(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Fundo de tela preto

    glShadeModel(GL_SMOOTH);
    glColorMaterial ( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
    glEnable(GL_DEPTH_TEST);
    glEnable ( GL_CULL_FACE );

    angX = 0.0;
    angY = 0.0;

}

// **********************************************************************
//  void main ( int argc, char** argv )
//
//
// **********************************************************************
int main ( int argc, char** argv )
{
    //check the example file path
    checkFilePath();

    //load file and initialize the vars
    loadFileAndInitializeVars();

    //get the references
    frames = getFrames();
    segments = getSegments();

    printf("Number of frames: %d.\n", frames->numberOfFrames);
    printf("Number of segments: %d.\n", segments->numberOfSegments);

    glutInit            ( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition (0,0);

    // Define o tamanho inicial da janela grafica do programa
    glutInitWindowSize  ( 650, 500);

    // Cria a janela na tela, definindo o nome da
    // que aparecera na barra de título da janela.
    glutCreateWindow    ("Point Loader" );

    // executa algumas inicializações
    init ();

    setObservatorPosition();
    setSegments(segments);
    MountActualFrame(frames);

    // Define que o tratador de evento para
    // o redesenho da tela. A funcao "display"
    // será chamada automaticamente quando
    // for necessário redesenhar a janela
    glutDisplayFunc ( display );

    // Define que a função que irá rodar a
    // continuamente. Esta função é usada para fazer animações
    // A funcao "display" será chamada automaticamente
    // sempre que for possível
    //glutIdleFunc ( display );

    // Define que o tratador de evento para
    // o redimensionamento da janela. A funcao "reshape"
    // será chamada automaticamente quando
    // o usuário alterar o tamanho da janela
    glutReshapeFunc ( reshape );

    // Define que o tratador de evento para
    // as teclas. A funcao "keyboard"
    // será chamada automaticamente sempre
    // o usuário pressionar uma tecla comum
    glutKeyboardFunc ( keyboard );

    // Define que o tratador de evento para
    // as teclas especiais(F1, F2,... ALT-A,
    // ALT-B, Teclas de Seta, ...).
    // A funcao "arrow_keys" será chamada
    // automaticamente sempre o usuário
    // pressionar uma tecla especial
    glutSpecialFunc ( arrow_keys );

    // Registra a função callback para eventos de botões do mouse
    glutMouseFunc(mouse);

    // Registra a função callback para eventos de movimento do mouse
    glutMotionFunc(move);

    // inicia o tratamento dos eventos
    glutMainLoop ( );
}
