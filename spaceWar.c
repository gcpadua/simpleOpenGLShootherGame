/*-------------------------------------------------------------------------------------------------
Link Texturas:  https://opengameart.org/content/space-ships-0
                https://opengameart.org/content/grid-hud
                https://opengameart.org/content/stars-0
                https://opengameart.org/content/asteroid-explosions-rocket-mine-and-laser
-------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>

#define TRUE 1 
#define FALSE 0
#define worldXStart -100
#define worldXEnd 100
#define worldYStart -100
#define worldYEnd 100
#define PI 3.141592654
#define pontMax 5000

int resolutionX=500;
int resolutionY=500;
unsigned long int frames=0;
unsigned long int levelStartFrame;
int gameState=0;
int points=0;
int nivel1Completo;
int bossSpawned;

GLuint idTexturaBackground;
GLuint idTexturaPlayer;
GLuint idTexturaMenuJogar;
GLuint idtexturaMenuComoJogar;
GLuint idTexturaMenuInstrucoes;
GLuint idTexturaTitulo;
GLuint idTexturaBalaPlayer;
GLuint idTexturaBalaInimigo;
GLuint idTexturaInimigoBasico;
GLuint idTexturaEnemyMine;
GLuint idtexturaHudPontuacao;
GLuint idTexturaEstrelaAcesa;
GLuint idTexturaEstrelaApagada;
GLuint idTexturaEnemySpy;
GLuint idTexturaEdgeShooter;
GLuint idTexturaInimigoAsteroide;
GLuint idTexturaBoss;
GLuint idTexturaLazerBoss;
GLuint idTexturaNivel1Completo;

typedef struct pointer{
    GLdouble x, y, z;
    int lmbState;
}pointer;
pointer worldMouse = {.x=0, .y=0, .z=0, .lmbState=0};

typedef struct sprite{
    unsigned char isRendered;
    GLuint idTextura;
    int type;
    GLfloat x, y, speedX, speedY;
    unsigned int timerA, timerB;
    int hp, attack;
    int largura, altura;
    GLfloat angle, scale;
}sprite;

sprite spriteVazia = {.type=-1, .angle=0, .scale=0};
sprite allies[100];
sprite enemies[100];

sprite spriteTemplate[100];
int playerHP;

void drawBrackground(int timer)
{
//Insere uma textura de espaço no fundo e faz scroll infinitamente
//resolução 600x900
//tamanho do retangulo do background: 200x300
    glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaBackground);
	glBegin(GL_TRIANGLE_FAN);
//canto inferior esquerdo
        glTexCoord2f(0, 0);
        glVertex3f(-100, -200-timer,  0);

//canto inferior direito
        glTexCoord2f(1, 0);
        glVertex3f( 100, -200-timer,  0);

//canto superior direito
        glTexCoord2f(1, 1);
        glVertex3f( 100,  100-timer,  0);

//canto superior esquerdo
        glTexCoord2f(0, 1);
        glVertex3f(-100,  100-timer,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    timer -=300;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaBackground);
	glBegin(GL_TRIANGLE_FAN);
//canto inferior esquerdo
        glTexCoord2f(0, 0);
        glVertex3f(-100, -200-timer,  0);

//canto inferior direito
        glTexCoord2f(1, 0);
        glVertex3f( 100, -200-timer,  0);

//canto superior direito
        glTexCoord2f(1, 1);
        glVertex3f( 100,  100-timer,  0);

//canto superior esquerdo
        glTexCoord2f(0, 1);
        glVertex3f(-100,  100-timer,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void drawPlayer(float x, float y)
{
    glPushMatrix();
    glTranslatef(x, y, 0);
    glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaPlayer);
	glBegin(GL_TRIANGLE_FAN);
//canto inferior esquerdo
        glTexCoord2f(0, 0);
        glVertex3f(-10, -10,  0);

//canto inferior direito
        glTexCoord2f(1, 0);
        glVertex3f( 10, -10,  0);

//canto superior direito
        glTexCoord2f(1, 1);
        glVertex3f( 10,  10,  0);

//canto superior esquerdo
        glTexCoord2f(0, 1);
        glVertex3f(-10,  10,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void drawMenu()
{
    glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaTitulo);
	glBegin(GL_TRIANGLE_FAN);
//canto inferior esquerdo
        glTexCoord2f(0, 0);
        glVertex3f(-30, 59,  0);

//canto inferior direito
        glTexCoord2f(1, 0);
        glVertex3f( 30, 59,  0);

//canto superior direito
        glTexCoord2f(1, 1);
        glVertex3f( 30,  99,  0);

//canto superior esquerdo
        glTexCoord2f(0, 1);
        glVertex3f(-30,  99,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaMenuJogar);
	glBegin(GL_TRIANGLE_FAN);
//canto inferior esquerdo
        glTexCoord2f(0, 0);
        glVertex3f(-50, -2,  0);

//canto inferior direito
        glTexCoord2f(1, 0);
        glVertex3f( 50, -2,  0);

//canto superior direito
        glTexCoord2f(1, 1);
        glVertex3f( 50,  62,  0);

//canto superior esquerdo
        glTexCoord2f(0, 1);
        glVertex3f(-50,  62,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    if(((worldMouse.x>-50)&&(worldMouse.x<50))&&((worldMouse.y>-72)&&(worldMouse.y<-12)))
    {
        glColor3f(1.0, 1.0, 1.0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, idTexturaMenuInstrucoes);
	    glBegin(GL_TRIANGLE_FAN);
    //canto inferior esquerdo
            glTexCoord2f(0, 0);
            glVertex3f(-50, -72,  0);

    //canto inferior direito
            glTexCoord2f(1, 0);
            glVertex3f( 50, -72,  0);

    //canto superior direito
            glTexCoord2f(1, 1);
            glVertex3f( 50,  -12,  0);

    //canto superior esquerdo
            glTexCoord2f(0, 1);
            glVertex3f(-50,  -12,  0);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
    else
    {
        glColor3f(1.0, 1.0, 1.0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, idtexturaMenuComoJogar);
	    glBegin(GL_TRIANGLE_FAN);
    //canto inferior esquerdo
            glTexCoord2f(0, 0);
            glVertex3f(-50, -72,  0);

    //canto inferior direito
            glTexCoord2f(1, 0);
            glVertex3f( 50, -72,  0);

    //canto superior direito
            glTexCoord2f(1, 1);
            glVertex3f( 50,  -12,  0);

    //canto superior esquerdo
            glTexCoord2f(0, 1);
            glVertex3f(-50,  -12,  0);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
}

void drawHUD()
{   
    //Desenho do contador de vidas
    glPushMatrix(); 
    glTranslatef(-90, -90, 0);
    if(allies[0].hp>0)
    {
        glScalef(0.5, 0.5, 1.0);
        glRotatef(-20, 0, 0, 1);
        
        glColor3f(1.0, 1.0, 1.0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, idTexturaPlayer);
	    glBegin(GL_TRIANGLE_FAN);
    //canto inferior esquerdo
        glTexCoord2f(0, 0);
        glVertex3f(-10, -10,  0);

    //canto inferior direito
        glTexCoord2f(1, 0);
        glVertex3f( 10, -10,  0);

    //canto superior direito
        glTexCoord2f(1, 1);
        glVertex3f( 10,  10,  0);

    //canto superior esquerdo
        glTexCoord2f(0, 1);
        glVertex3f(-10,  10,  0);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glRotatef(20, 0, 0, 1);
        glScalef(2.0, 2.0, 1.0);
    }
    glTranslatef(15, 0, 0);
    if(allies[0].hp>100)
    {
        glScalef(0.5, 0.5, 1.0);
        glRotatef(-20, 0, 0, 1);
        
        glColor3f(1.0, 1.0, 1.0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, idTexturaPlayer);
	    glBegin(GL_TRIANGLE_FAN);
    //canto inferior esquerdo
        glTexCoord2f(0, 0);
        glVertex3f(-10, -10,  0);

    //canto inferior direito
        glTexCoord2f(1, 0);
        glVertex3f( 10, -10,  0);

    //canto superior direito
        glTexCoord2f(1, 1);
        glVertex3f( 10,  10,  0);

    //canto superior esquerdo
        glTexCoord2f(0, 1);
        glVertex3f(-10,  10,  0);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glRotatef(20, 0, 0, 1);
        glScalef(2.0, 2.0, 1.0);
    }
    glTranslatef(15, 0, 0);
    if(allies[0].hp>200)
    {
        glScalef(0.5, 0.5, 1.0);
        glRotatef(-20, 0, 0, 1);
        
        glColor3f(1.0, 1.0, 1.0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, idTexturaPlayer);
	    glBegin(GL_TRIANGLE_FAN);
    //canto inferior esquerdo
        glTexCoord2f(0, 0);
        glVertex3f(-10, -10,  0);

    //canto inferior direito
        glTexCoord2f(1, 0);
        glVertex3f( 10, -10,  0);

    //canto superior direito
        glTexCoord2f(1, 1);
        glVertex3f( 10,  10,  0);

    //canto superior esquerdo
        glTexCoord2f(0, 1);
        glVertex3f(-10,  10,  0);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glRotatef(20, 0, 0, 1);
        glScalef(2.0, 2.0, 1.0);
    }
    glPopMatrix();
    //Desenho do contador de pontos
    glPushMatrix();
    glTranslatef(80, -90, 0);
    if(gameState>0)
    {
        glColor3f(1.0, 1.0, 1.0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, idtexturaHudPontuacao);
	    glBegin(GL_TRIANGLE_FAN);
    //canto inferior esquerdo
        glTexCoord2f(0, 0);
        glVertex3f(-20, -10,  0);

    //canto inferior direito
        glTexCoord2f(1, 0);
        glVertex3f( 20, -10,  0);

    //canto superior direito
        glTexCoord2f(1, 1);
        glVertex3f( 20,  10,  0);

    //canto superior esquerdo
        glTexCoord2f(0, 1);
        glVertex3f(-20,  10,  0);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        GLfloat temp;
        temp = 36*((float)points/pontMax);
        if(temp>36) {temp=36;}
        glBegin(GL_TRIANGLES);
        glVertex3f(-18, -7,  0);
        glVertex3f(-18, +3,  0);
        glVertex3f( 18, -7,  0);//
        glVertex3f( 18, -7,  0);//
        glVertex3f( 18, +3,  0);//
        glVertex3f(-18, +3,  0);
        glEnd();
        glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_TRIANGLES);
        glVertex3f(-18, -7,  0);
        glVertex3f(-18, +3,  0);
        glVertex3f( -18+temp, -7,  0);//
        glVertex3f( -18+temp, -7,  0);//
        glVertex3f( -18+temp, +3,  0);//
        glVertex3f(-18, +3,  0);
        glEnd();
        if (points>(0.25*pontMax))
        {
            glTranslatef(-12, -2, 0);
            glColor3f(1.0, 1.0, 1.0);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, idTexturaEstrelaAcesa);
	        glBegin(GL_TRIANGLE_FAN);
        //canto inferior esquerdo
            glTexCoord2f(0, 0);
            glVertex3f(-5, -5,  0);

        //canto inferior direito
            glTexCoord2f(1, 0);
            glVertex3f( 5, -5,  0);

        //canto superior direito
            glTexCoord2f(1, 1);
            glVertex3f( 5,  5,  0);

        //canto superior esquerdo
            glTexCoord2f(0, 1);
            glVertex3f(-5,  5,  0);
            glEnd();
            glDisable(GL_TEXTURE_2D);
        }
        else
        {
            glTranslatef(-12, -2, 0);
            glColor3f(1.0, 1.0, 1.0);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, idTexturaEstrelaApagada);
	        glBegin(GL_TRIANGLE_FAN);
        //canto inferior esquerdo
            glTexCoord2f(0, 0);
            glVertex3f(-5, -5,  0);

        //canto inferior direito
            glTexCoord2f(1, 0);
            glVertex3f( 5, -5,  0);

        //canto superior direito
            glTexCoord2f(1, 1);
            glVertex3f( 5,  5,  0);

        //canto superior esquerdo
            glTexCoord2f(0, 1);
            glVertex3f(-5,  5,  0);
            glEnd();
            glDisable(GL_TEXTURE_2D);
        }
        if (points>(0.5*pontMax))
        {
            glTranslatef(12, 0, 0);
            glColor3f(1.0, 1.0, 1.0);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, idTexturaEstrelaAcesa);
	        glBegin(GL_TRIANGLE_FAN);
        //canto inferior esquerdo
            glTexCoord2f(0, 0);
            glVertex3f(-5, -5,  0);

        //canto inferior direito
            glTexCoord2f(1, 0);
            glVertex3f( 5, -5,  0);

        //canto superior direito
            glTexCoord2f(1, 1);
            glVertex3f( 5,  5,  0);

        //canto superior esquerdo
            glTexCoord2f(0, 1);
            glVertex3f(-5,  5,  0);
            glEnd();
            glDisable(GL_TEXTURE_2D);
        }
        else
        {
            glTranslatef(12, 0, 0);
            glColor3f(1.0, 1.0, 1.0);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, idTexturaEstrelaApagada);
	        glBegin(GL_TRIANGLE_FAN);
        //canto inferior esquerdo
            glTexCoord2f(0, 0);
            glVertex3f(-5, -5,  0);

        //canto inferior direito
            glTexCoord2f(1, 0);
            glVertex3f( 5, -5,  0);

        //canto superior direito
            glTexCoord2f(1, 1);
            glVertex3f( 5,  5,  0);

        //canto superior esquerdo
            glTexCoord2f(0, 1);
            glVertex3f(-5,  5,  0);
            glEnd();
            glDisable(GL_TEXTURE_2D);
        }
        if (points>(0.75*pontMax))
        {
            glTranslatef(12, 0, 0);
            glColor3f(1.0, 1.0, 1.0);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, idTexturaEstrelaAcesa);
	        glBegin(GL_TRIANGLE_FAN);
        //canto inferior esquerdo
            glTexCoord2f(0, 0);
            glVertex3f(-5, -5,  0);

        //canto inferior direito
            glTexCoord2f(1, 0);
            glVertex3f( 5, -5,  0);

        //canto superior direito
            glTexCoord2f(1, 1);
            glVertex3f( 5,  5,  0);

        //canto superior esquerdo
            glTexCoord2f(0, 1);
            glVertex3f(-5,  5,  0);
            glEnd();
            glDisable(GL_TEXTURE_2D);
        }
        else
        {
            glTranslatef(12, 0, 0);
            glColor3f(1.0, 1.0, 1.0);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, idTexturaEstrelaApagada);
	        glBegin(GL_TRIANGLE_FAN);
        //canto inferior esquerdo
            glTexCoord2f(0, 0);
            glVertex3f(-5, -5,  0);

        //canto inferior direito
            glTexCoord2f(1, 0);
            glVertex3f( 5, -5,  0);

        //canto superior direito
            glTexCoord2f(1, 1);
            glVertex3f( 5,  5,  0);

        //canto superior esquerdo
            glTexCoord2f(0, 1);
            glVertex3f(-5,  5,  0);
            glEnd();
            glDisable(GL_TEXTURE_2D);
        }
    }
    glPopMatrix();
}

void desenhaNivel1Completo()
{
    glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaNivel1Completo);
	glBegin(GL_TRIANGLE_FAN);
    //canto inferior esquerdo
    glTexCoord2f(0, 0);
    glVertex3f(-60, -40,  0);

    //canto inferior direito
    glTexCoord2f(1, 0);
    glVertex3f( 60, -40,  0);

    //canto superior direito
    glTexCoord2f(1, 1);
    glVertex3f( 60,  40,  0);

    //canto superior esquerdo
    glTexCoord2f(0, 1);
    glVertex3f(-60,  40,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void desenhaBossHPBar()
{
    glPushMatrix();
    glTranslatef(0, 95, 0);
    GLfloat temp;
    temp = 180*((float)enemies[0].hp/spriteTemplate[8].hp);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex3f(-90, -4,  0);
    glVertex3f(-90, +4,  0);
    glVertex3f( 90, -4,  0);//
    glVertex3f( 90, -4,  0);//
    glVertex3f( 90, +4,  0);//
    glVertex3f(-90, +4,  0);
    glEnd();
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex3f(-90, -4,  0);
    glVertex3f(-90, +4,  0);
    glVertex3f(-90+temp, -4,  0);//
    glVertex3f(-90+temp, -4,  0);//
    glVertex3f(-90+temp, +4,  0);//
    glVertex3f(-90, +4,  0);
    glEnd();
    glPopMatrix();
}

void drawSprite(GLfloat x, GLfloat y, GLfloat largura, GLfloat altura, GLuint idTextura, GLfloat angle)
{
    glPushMatrix();
    glTranslatef(x, y, 0);
    glColor3f(1.0, 1.0, 1.0);
    glRotatef(angle, 0, 0, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTextura);
	glBegin(GL_TRIANGLE_FAN);
//canto inferior esquerdo
        glTexCoord2f(0, 0);
        glVertex3f( -(largura/2), -(altura/2),  0);

//canto inferior direito
        glTexCoord2f(1, 0);
        glVertex3f( +(largura/2), -(altura/2),  0);

//canto superior direito
        glTexCoord2f(1, 1);
        glVertex3f( +(largura/2),  +(altura/2),  0);

//canto superior esquerdo
        glTexCoord2f(0, 1);
        glVertex3f( -(largura/2),  +(altura/2),  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glRotatef(-angle, 0, 0, 1);
    glPopMatrix();
}

void displayDraw() 
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    drawBrackground(frames%300);
    if (gameState==0) {drawMenu();}
    else 
    {
        drawPlayer(allies[0].x, allies[0].y);
        for(int i=0; i<100; i++)
        {
            if(allies[i].type>0)
            {
                drawSprite(allies[i].x, allies[i].y, allies[i].largura, allies[i].altura, allies[i].idTextura, 0);
            }
            if(enemies[i].type>0)
            {
                drawSprite(enemies[i].x, enemies[i].y, enemies[i].largura, enemies[i].altura, enemies[i].idTextura, enemies[i].angle);
            }
        }
        if(nivel1Completo) {desenhaNivel1Completo();}
        if(bossSpawned) {desenhaBossHPBar();}
        drawHUD();
    }
    //drawPlayer(0, 0);
    glFlush();
}

void windowResized(int x, int y)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(worldXStart, worldXEnd, worldYStart, worldYEnd, -1, 1);

    float windowAspectRatio = ((float)x)/y;     
    float worldAspectRatio = ((float)(abs(worldXStart)+abs(worldXEnd)))/(abs(worldYStart)+abs(worldYEnd));
    //na linha anterior a partir ddos parametros do mundo definidos no começo do código calculamos a razão do mundo
    if (windowAspectRatio > worldAspectRatio) 
    {                                                        //caso o mundo seja mais alto que a janela
        float windowWidth = ((float)y) * worldAspectRatio;   //cria barras horizontais
        float viewportStartX = (x - windowWidth) / 2;
        glViewport(viewportStartX, 0, windowWidth, y);
        resolutionX = windowWidth;
        resolutionY = y;
    }
    else if (windowAspectRatio < worldAspectRatio)      //caso o mundo seja mais largo que a janela
    {                                                   //cria barras verticais
        float windowHeight = x / worldAspectRatio;
        float viewportStartY = (y - windowHeight)/2;
        glViewport(0, viewportStartY, x, windowHeight);
        resolutionX = x;
        resolutionY = windowHeight;
    } 
    else                        //caso a janela esteja com o aspecto correto, apenas aumentar a viewport
    {
        glViewport(0, 0, x, y);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


//--------------------------- Callbacks de mouse --------------------------------------------------
void mouseEvent(int button, int state, int x, int y)
{
    //printf("Posição glut: %d %d\n", x, y);
    GLint viewport[4];
    GLdouble modelview[16],projection[16];
    GLfloat wx=x,wy,wz;
    glGetIntegerv(GL_VIEWPORT,viewport);
    y=viewport[3]-y;
    wy=y;
    glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
    glGetDoublev(GL_PROJECTION_MATRIX,projection);
    glReadPixels(x,y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz);
    gluUnProject(wx,wy,wz,modelview,projection,viewport,&worldMouse.x,&worldMouse.y,&worldMouse.z);
    //printf("Posição mundo: %f %f\n", worldMouse.x, worldMouse.y);
    //glutPostRedisplay();
    if(button==0)
    {
        if(state==0) {worldMouse.lmbState=1;}
        else {worldMouse.lmbState=0;}
    }
    if((button==0)&&(state==0)&&((worldMouse.x>-50)&&(worldMouse.x<50))&&((worldMouse.y>-2)&&(worldMouse.y<62))&&(gameState==0))
    {
        gameState=1;
        levelStartFrame = frames;
        int temp=0;
        while(temp<100)
        {
            allies[temp].altura =0;
            allies[temp].angle=0;
            allies[temp].attack=0;
            allies[temp].hp=0;
            allies[temp].idTextura=0;
            allies[temp].isRendered=0;
            allies[temp].largura=0;
            allies[temp].speedX=0;
            allies[temp].speedY=0;
            allies[temp].timerA=0;
            allies[temp].timerB=0;
            allies[temp].type=-1;
            allies[temp].x=-500;
            allies[temp].y=-500;
        
            enemies[temp].altura =0;
            enemies[temp].angle=0;
            enemies[temp].attack=0;
            enemies[temp].hp=0;
            enemies[temp].idTextura=0;
            enemies[temp].isRendered=0;
            enemies[temp].largura=0;
            enemies[temp].speedX=0;
            enemies[temp].speedY=0;
            enemies[temp].timerA=0;
            enemies[temp].timerB=0;
            enemies[temp].type=-1;
            enemies[temp].x=-500;
            enemies[temp].y=-500;
            temp++;
    }
        allies[0]=spriteTemplate[0];
    }
}

void pointerMovedActive(int x, int y)
{ 
    //printf("Posição glut: %d %d\n", x, y);
    GLint viewport[4];
    GLdouble modelview[16],projection[16];
    GLfloat wx=x,wy,wz;
    glGetIntegerv(GL_VIEWPORT,viewport);
    y=viewport[3]-y;
    wy=y;
    glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
    glGetDoublev(GL_PROJECTION_MATRIX,projection);
    glReadPixels(x,y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz);
    gluUnProject(wx,wy,wz,modelview,projection,viewport,&worldMouse.x,&worldMouse.y,&worldMouse.z);
    //printf("Posição mundo: %f %f\n", worldMouse.x, worldMouse.y);
    //glutPostRedisplay();
}

void pointerMovedPassive(int x, int y)
{
    //printf("Posição glut: %d %d\n", x, y);
    GLint viewport[4];
    GLdouble modelview[16],projection[16];
    GLfloat wx=x,wy,wz;
    glGetIntegerv(GL_VIEWPORT,viewport);
    y=viewport[3]-y;
    wy=y;
    glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
    glGetDoublev(GL_PROJECTION_MATRIX,projection);
    glReadPixels(x,y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz);
    gluUnProject(wx,wy,wz,modelview,projection,viewport,&worldMouse.x,&worldMouse.y,&worldMouse.z);
    //printf("Posição mundo: %f %f\n", worldMouse.x, worldMouse.y);
    //glutPostRedisplay();
}


//------------------------ Funções que adicionam itens ao array -----------------------------------
void spawnPlayerBullet(GLfloat x, GLfloat y)
{
    int feito=0;
    for (int j=1; j<100; j++)
    {
        if(feito==0)
        {
            if(allies[j].type==-1)
            {
                allies[j]=spriteTemplate[1];
                allies[j].x = x;
                allies[j].y = y;
                feito++;
            }
        }
    }
}

void spawnBasicEnemy(GLfloat x, GLfloat y, GLfloat orientationX, GLfloat orientationY)
{
    GLfloat angle;
    int feito=0;
    for (int j=0; j<100; j++)
    {
        if(feito==0)
        {
            if(enemies[j].type==-1)
            {
                enemies[j]=spriteTemplate[2];
                enemies[j].x = x;
                enemies[j].y = y;
                enemies[j].speedX*= orientationX;
                enemies[j].speedY*= orientationY;
                angle = atan2(enemies[j].speedY, enemies[j].speedX);
                enemies[j].angle= (angle * 180.0/PI)+90.0;
                feito++;
            }
        }
    }
}

void spawnEnemyBullet(GLfloat x, GLfloat y, GLfloat orientationX, GLfloat orientationY)
{
    GLfloat angle;
    int feito=0;
    for (int j=0; j<100; j++)
    {
        if(feito==0)
        {
            if(enemies[j].type==-1)
            {
                enemies[j]=spriteTemplate[3];
                enemies[j].x = x;
                enemies[j].y = y;
                enemies[j].speedX*= orientationX;
                enemies[j].speedY*= orientationY;
                angle = atan2(enemies[j].speedY, enemies[j].speedX);
                enemies[j].angle= (angle * 180.0/PI)+90.0;
                feito++;
            }
        }
    }
}

void spawnEnemyMine(GLfloat x, GLfloat y)
{
    int feito=0;
    for (int j=1; j<100; j++)
    {
        if(feito==0)
        {
            if(enemies[j].type==-1)
            {
                enemies[j]=spriteTemplate[4];
                enemies[j].x = x;
                enemies[j].y = y;
                feito++;
            }
        }
    }
}

void spawnEnemySpy(GLfloat x, GLfloat y, GLfloat orientationX, GLfloat orientationY)
{
    GLfloat angle;
    int feito=0;
    for (int j=0; j<100; j++)
    {
        if(feito==0)
        {
            if(enemies[j].type==-1)
            {
                enemies[j]=spriteTemplate[5];
                enemies[j].x = x;
                enemies[j].y = y;
                enemies[j].speedX*= orientationX;
                enemies[j].speedY*= orientationY;
                angle = atan2(enemies[j].speedY, enemies[j].speedX);
                enemies[j].angle= (angle * 180.0/PI)+270.0;
                feito++;
            }
        }
    }
}

void spawnEdgeShooter(GLfloat x, GLfloat y, GLfloat orientationX, GLfloat orientationY)
{
    int feito=0;
    for (int j=1; j<100; j++)
    {
        if(feito==0)
        {
            if(enemies[j].type==-1)
            {
                enemies[j]=spriteTemplate[6];
                enemies[j].speedX*= orientationX;
                enemies[j].speedY*= orientationY;
                enemies[j].x = x;
                enemies[j].y = y;
                feito++;
            }
        }
    }
}

void spawnDvdEnemy(GLfloat x, GLfloat y, GLfloat orientationX, GLfloat orientationY)
{
    int feito=0;
    for (int j=1; j<100; j++)
    {
        if(feito==0)
        {
            if(enemies[j].type==-1)
            {
                enemies[j]=spriteTemplate[7];
                enemies[j].speedX*= orientationX;
                enemies[j].speedY*= orientationY;
                enemies[j].x = x;
                enemies[j].y = y;
                feito++;
            }
        }
    }
}

void spawnBoss(GLfloat x, GLfloat y)
{
    int feito=0;
    for (int j=0; j<100; j++)
    {
        if(feito==0)
        {
            if(enemies[j].type==-1)
            {
                enemies[j]=spriteTemplate[8];
                enemies[j].x = x;
                enemies[j].y = y;
                feito++;
            }
        }
    }
}

void spawnBossLazer(GLfloat x, GLfloat y)
{
    int feito=0;
    for (int j=0; j<100; j++)
    {
        if(feito==0)
        {
            if(enemies[j].type==-1)
            {
                enemies[j]=spriteTemplate[9];
                enemies[j].x = x;
                enemies[j].y = y+30;
                feito++;
            }
        }
    }
}

void colisionDetected(int i, int j)
{
    allies[i].hp-=enemies[j].attack;
    enemies[j].hp-=allies[i].attack;
    if(allies[i].hp<=0) {allies[i] = spriteVazia;}
    if(enemies[j].hp<=0) 
    {
        if(enemies[j].type != 3) {points+=100;}
        if(enemies[j].type == 8) {points+=5000;}
        enemies[j] = spriteVazia;
        
    }
    if(i==0) 
    {
        allies[0].timerB=0;
        //allies[0].hp-=100;
    }
}

void timerFunction(int period)
{
    frames++;
    if(allies[0].hp<=0) 
    {
        gameState=0;
        points=0;
    }
    if(gameState==1)
    {
        unsigned long int levelFrame = frames - levelStartFrame;
        if((levelFrame<3600)||(levelFrame>=3750&&levelFrame<7350))
        {
            nivel1Completo=0;
            if(levelFrame%60==0)
            {
            int random = rand()%5;
            if(random==0)           //spawna um inimigo basico
            {
                random=rand()%3;
                if(random==0)       //no canto esuqerdo da tela
                {
                    spawnBasicEnemy(-110,(rand()%150-50), 1, 0);
                }
                else if(random==1)  //no carto direito da tela
                {
                    spawnBasicEnemy(110,(rand()%150-50), -1, 0);
                }
                else if(random==2)  //no canto superior da tela
                {
                    spawnBasicEnemy((rand()%150-75),110, 0, -1);
                }
            }
            else if(random==1)      //spawna uma mina perseguidora
            {
                random=rand()%3;
                if(random==0)       //no canto esuqerdo da tela
                {
                    spawnEnemyMine(-110,(rand()%150-75));
                }
                else if(random==1)  //no carto direito da tela
                {
                    spawnEnemyMine(110,(rand()%150-75));
                }
                else if(random==2)  //no canto superior da tela
                {
                    spawnEnemyMine((rand()%150-75),110);
                }
            }
            else if(random==2)      //spawna um spy
            {
                random=rand()%4;
                if(random==0)       //no canto esuqerdo da tela
                {
                    spawnEnemySpy(-110,(rand()%150-75),1,0);
                }
                else if(random==1)  //no carto direito da tela
                {
                    spawnEnemySpy(110,(rand()%150-75),-1,0);
                }
                else if(random==2)  //no canto superior da tela
                {
                    spawnEnemySpy((rand()%150-75),110,0,-1);
                }
                else                //no canto inferior da tela
                {
                    spawnEnemySpy((rand()%150-75),-110,0,1);
                }
            }
            else if(random==3)      //spawna um edge shooter
            {
                random=rand()%4;
                if(random==0)       //no canto esuqerdo da tela
                {
                    spawnEdgeShooter(-110,100,1,0);
                }
                else if(random==1)  //no carto direito da tela
                {
                    spawnEdgeShooter(100,110,0,-1);
                }
                else if(random==2)  //no canto superior da tela
                {
                    spawnEdgeShooter(110,-100,-1,0);
                }
                else                //no canto inferior da tela
                {
                    spawnEdgeShooter(100,-110,0,1);
                }
            }
            else if(random==4)      //spawna um asteroide
            {
                random=rand()%4;
                if(random==0)       //no canto esuqerdo da tela
                {
                    spawnDvdEnemy(-110,(rand()%150-75),1,(rand()%3-1));
                }
                else if(random==1)  //no carto direito da tela
                {
                    spawnDvdEnemy(110,(rand()%150-75),-1,(rand()%3-1));
                }
                else if(random==2)  //no canto superior da tela
                {
                    spawnDvdEnemy((rand()%150-75),110,(rand()%3-1),-1);
                }
                else                //no canto inferior da tela
                {
                    spawnDvdEnemy((rand()%150-75),-110,(rand()%3-1),1);
                }
            }
            }
        }
        if(levelFrame==3600||levelFrame==7350)
        {
            int temp=0;
            while(temp<100)
            {
            enemies[temp].altura =0;
            enemies[temp].angle=0;
            enemies[temp].attack=0;
            enemies[temp].hp=0;
            enemies[temp].idTextura=0;
            enemies[temp].isRendered=0;
            enemies[temp].largura=0;
            enemies[temp].speedX=0;
            enemies[temp].speedY=0;
            enemies[temp].timerA=0;
            enemies[temp].timerB=0;
            enemies[temp].type=-1;
            enemies[temp].x=-500;
            enemies[temp].y=-500;
            temp++;
            }
        }
        if(levelFrame>=3600&&levelFrame<3750)
        {
            nivel1Completo=1;
        }
        if(levelFrame==7500)
        {
            if(enemies[0].type!=8)
            {
                enemies[0] = spriteTemplate[8];
                enemies[0].y=120;
                bossSpawned=1;
            }

        }
        if((levelFrame>=7500)&&(enemies[0].y>70))
        {
            enemies[0].y--;
        }






        if((worldMouse.x<90)&&(worldMouse.x>-90)) {allies[0].x=worldMouse.x;}
        if((worldMouse.y<90)&&(worldMouse.y>-90)) {allies[0].y=worldMouse.y;}
        //Este loop for é o responsável pela lógica do player e das balas e inimigos
        for(int i=0; i<100; i++)
        {
            if(allies[i].timerA>0) {allies[i].timerA--;}
            if(allies[i].timerB>0) {allies[i].timerB--;}
            if(enemies[i].timerA>0) {enemies[i].timerA--;}
            if(enemies[i].timerB>0) {enemies[i].timerB--;}
            //Este switch é o responsável pela lógica do player e das balas
            switch (allies[i].type)
            {
            case 0:     //caso tipo==player
                //printf(" ");
                if((allies[i].timerA==0)&&(worldMouse.lmbState==1))
                {
                    allies[i].timerA=3;
                    spawnPlayerBullet(allies[i].x+5.0, allies[i].y);
                    spawnPlayerBullet(allies[i].x-5.0, allies[i].y);
                    spawnPlayerBullet(allies[i].x, allies[i].y+allies[i].altura);
                    //printf(" ");
                }
            break;
            case 1:     //Caso tipo==bala
                if(allies[i].timerA==0) {allies[i]=spriteVazia;}
                else {allies[i].y+=allies[i].speedY;}
            break;
            default:
            break;
            }
            //Este switch é o responsável pela lógica dos inimigos
            switch (enemies[i].type)
            {
            case 2:     //Caso tipo==inimigoBasico
                enemies[i].x+=enemies[i].speedX;
                enemies[i].y+=enemies[i].speedY;
                if(enemies[i].x==allies[0].x) 
                    {spawnEnemyBullet(enemies[i].x, enemies[i].y-(enemies[i].altura/2), 0, -1);}
                if(enemies[i].timerB==0)
                {
                    spawnEnemyBullet(enemies[i].x, enemies[i].y-(enemies[i].altura/2), 0, -1);
                    enemies[i].timerB=20;
                }
                if(enemies[i].timerA==0) {enemies[i]=spriteVazia;}
            break;
            case 3:     //Caso tipo==balaInimigo
                enemies[i].x+=enemies[i].speedX;
                enemies[i].y+=enemies[i].speedY;
                if(enemies[i].timerA==0) {enemies[i]=spriteVazia;}
            break;
            case 4:     //Caso tipo==mina perseguidora
                if(enemies[i].x>allies[0].x) {enemies[i].x-=enemies[i].speedX;}
                else {enemies[i].x+=enemies[i].speedX;}
                if(enemies[i].y>allies[0].y) {enemies[i].y-=enemies[i].speedY;}
                else {enemies[i].y+=enemies[i].speedY;}
                if(enemies[i].timerA==0) {enemies[i]=spriteVazia;}
            break;
            case 5:     //Caso tipo==spy
                enemies[i].x+=enemies[i].speedX;
                enemies[i].y+=enemies[i].speedY;
                if(enemies[i].timerB==0)
                {
                    enemies[i].speedX*=40;
                    enemies[i].speedY*=40;
                    enemies[i].timerB=9999;
                }
                if(enemies[i].timerA==0) {enemies[i]=spriteVazia;}
            break;
            case 6:     //Caso tipo==
                if(enemies[i].x==100&&enemies[i].y==100)
                {
                    enemies[i].speedX=0;
                    enemies[i].speedY=-1;
                }
                else if(enemies[i].x==100&&enemies[i].y==-100)
                {
                    enemies[i].speedX=-1;
                    enemies[i].speedY=0;
                }
                else if(enemies[i].x==-100&&enemies[i].y==-100)
                {
                    enemies[i].speedX=0;
                    enemies[i].speedY=1;
                }
                else if(enemies[i].x==-100&&enemies[i].y==100)
                {
                    enemies[i].speedX=1;
                    enemies[i].speedY=0;
                }
                if((enemies[i].timerB==0)&&(enemies[i].speedX==1)&&(enemies[i].speedY==0))
                {
                    spawnEnemyBullet(enemies[i].x,enemies[i].y, 0, -1);
                    enemies[i].timerB=60;
                }
                else if((enemies[i].timerB==0)&&(enemies[i].speedX==0)&&(enemies[i].speedY==-1))
                {
                    spawnEnemyBullet(enemies[i].x,enemies[i].y, -1, 0);
                    enemies[i].timerB=60;
                }
                else if((enemies[i].timerB==0)&&(enemies[i].speedX==-1)&&(enemies[i].speedY==0))
                {
                    spawnEnemyBullet(enemies[i].x,enemies[i].y, 0, 1);
                    enemies[i].timerB=60;
                }
                else if((enemies[i].timerB==0)&&(enemies[i].speedX==0)&&(enemies[i].speedY==1))
                {
                    spawnEnemyBullet(enemies[i].x,enemies[i].y, 1, 0);
                    enemies[i].timerB=60;
                }
                
                enemies[i].x+=enemies[i].speedX;
                enemies[i].y+=enemies[i].speedY;
            break;
            case 7:     //Caso inimigo==asteroide dvd
                if(enemies[i].timerB==0)
                {
                    if(enemies[i].x==100) {enemies[i].speedX*=-1;}
                    if(enemies[i].x==-100) {enemies[i].speedX*=-1;}
                    if(enemies[i].y==100) {enemies[i].speedY*=-1;}
                    if(enemies[i].y==-100) {enemies[i].speedY*=-1;}
                }
                enemies[i].angle = frames%360;
                enemies[i].x+=enemies[i].speedX;
                enemies[i].y+=enemies[i].speedY;
            break;
            case 8:     //caso inimigo==boss
                if(enemies[i].timerA==30&&(allies[0].x>-70||allies[0].x<70))
                {
                    spawnEnemyBullet(0,60,0,-1);
                    spawnEnemyBullet(35,60,0,-1);
                    spawnEnemyBullet(70,60,0,-1);
                    spawnEnemyBullet(-35,60,0,-1);
                    spawnEnemyBullet(-70,60,0,-1);
                }
                if(enemies[i].timerA==0&&(allies[0].x>-70||allies[0].x<70))
                {
                    spawnEnemyBullet(17.5,60,0,-1);
                    spawnEnemyBullet(52.5,60,0,-1);
                    spawnEnemyBullet(-17.5,60,0,-1);
                    spawnEnemyBullet(-52.5,60,0,-1);
                    enemies[i].timerA=60;
                }
                else if(enemies[i].timerB==0&&(allies[0].x<-70||allies[0].x>70))
                {

                    spawnBossLazer(85,60);
                    spawnBossLazer(-85,60);
                    enemies[i].timerB=300;
                }
            break;
            case 9:     //caso inimigo==boss Laser
                enemies[i].y+=enemies[i].speedY;
                if(enemies[i].timerA==0) {enemies[i]=spriteVazia;}
            break;
            }
        }

        for(int i=0; i<100; i++)
        {
            if(allies[i].type>=0)
            {
                if(1)
                {
                    GLfloat cima, baixo, esq, dir;
                    esq = allies[i].x-(allies[i].largura/2);
                    dir = allies[i].x+(allies[i].largura/2);
                    cima = allies[i].y+(allies[i].altura/2)-10;
                    baixo = allies[i].y-(allies[i].altura/2);
                    for(int j=0; j<100; j++)
                    {
                        GLfloat esqIn = enemies[j].x-(enemies[j].largura/2);
                        GLfloat dirIn = enemies[j].x+(enemies[j].largura/2);
                        GLfloat cimaIn = enemies[j].y+(enemies[j].altura/2);
                        GLfloat baixoIn = enemies[j].y-(enemies[i].altura/2);
                        if(enemies[j].type>0)
                        {
                            if(     (((((esq)<=(dirIn))&&((esq)>=(esqIn)))||
                                    (((dir)>=(esqIn))&&((dir)<=(dirIn)))||
                                    (((esq)<=(esqIn))&&((dir)>=(dirIn)))||
                                    (((esq)>=(esqIn))&&((dir)<=(dirIn))))
                                    &&
                                    ((((baixo)<=(cimaIn))&&((baixo)>=(baixoIn)))||
                                    (((cima)>=(baixoIn))&&((cima)<=(cimaIn)))||
                                    (((baixo)<=(baixoIn))&&((cima)>=(cimaIn)))||
                                    (((baixo)>=(baixoIn))&&((cima)<=(cimaIn)))))
                                    ||
                                    ((((((esq+1)<=(dirIn))&&((esq+1)>=(esqIn)))||
                                    (((dir+1)>=(esqIn))&&((dir+1)<=(dirIn)))||
                                    (((esq+1)<=(esqIn))&&((dir+1)>=(dirIn)))||
                                    ((esq+1)>=(esqIn))&&((dir+1)<=(dirIn))))
                                    &&
                                    ((((baixo+1)<=(cimaIn))&&((baixo+1)>=(baixoIn)))||
                                    (((cima+1)>=(baixoIn))&&((cima+1)<=(cimaIn)))||
                                    (((baixo+1)<=(baixoIn))&&((cima+1)>=(cimaIn)))||
                                    (((baixo+1)>=(baixoIn))&&((cima+1)<=(cimaIn)))))
                                    ||
                                    ((((((esq-1)<=(dirIn))&&((esq-1)>=(esqIn)))||
                                    (((dir-1)>=(esqIn))&&((dir-1)<=(dirIn)))||
                                    (((esq-1)<=(esqIn))&&((dir-1)>=(dirIn)))||
                                    ((esq-1)>=(esqIn))&&((dir-1)<=(dirIn))))
                                    &&
                                    ((((baixo-1)<=(cimaIn))&&((baixo+1)>=(baixoIn)))||
                                    (((cima-1)>=(baixoIn))&&((cima-1)<=(cimaIn)))||
                                    (((baixo-1)<=(baixoIn))&&((cima-1)>=(cimaIn)))||
                                    (((baixo-1)>=(baixoIn))&&((cima-1)<=(cimaIn))))))
                                {colisionDetected(i, j);}
                        }
                    }
                }
                
            }
        }
    }
    glutPostRedisplay();
    glutTimerFunc(period, timerFunction, period);
}

GLuint carregaTextura(const char* arquivo) 
{
    GLuint idTextura = SOIL_load_OGL_texture(
                           arquivo,
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );

    if (idTextura == 0) {
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }
    return idTextura;
}

void setupPlayer()
{
    spriteTemplate[0].type=0;
    spriteTemplate[0].hp=300;
    spriteTemplate[0].attack=300;
    spriteTemplate[0].x=0;
    spriteTemplate[0].y=0;
    spriteTemplate[0].speedX=0;
    spriteTemplate[0].speedY=0;
    spriteTemplate[0].timerA=0;
    spriteTemplate[0].timerB=0;
    spriteTemplate[0].isRendered=TRUE;
    spriteTemplate[0].idTextura=idTexturaPlayer;
    spriteTemplate[0].altura=15;
    spriteTemplate[0].largura=5;
}

void setupPlayerBullet()
{
    spriteTemplate[1].type=1;
    spriteTemplate[1].hp=1;
    spriteTemplate[1].attack=15;
    spriteTemplate[1].x=0;
    spriteTemplate[1].y=0;
    spriteTemplate[1].speedX=0;
    spriteTemplate[1].speedY=5;
    spriteTemplate[1].timerA=60;
    spriteTemplate[1].timerB=0;
    spriteTemplate[1].isRendered=TRUE;
    spriteTemplate[1].idTextura=idTexturaBalaPlayer;
    spriteTemplate[1].altura=4;
    spriteTemplate[1].largura=2;
}

void setupBasicEnemy()
{
    spriteTemplate[2].type=2;
    spriteTemplate[2].hp=200;
    spriteTemplate[2].attack=100;
    spriteTemplate[2].x=0;
    spriteTemplate[2].y=0;
    spriteTemplate[2].speedX=1.5;
    spriteTemplate[2].speedY=1.5;
    spriteTemplate[2].timerA=150;
    spriteTemplate[2].timerB=20;
    spriteTemplate[2].isRendered=TRUE;
    spriteTemplate[2].idTextura=idTexturaInimigoBasico;
    spriteTemplate[2].altura=12;
    spriteTemplate[2].largura=12;
}

void setupEnemyBullet()
{
    spriteTemplate[3].type=3;
    spriteTemplate[3].hp=1;
    spriteTemplate[3].attack=100;
    spriteTemplate[3].x=0;
    spriteTemplate[3].y=0;
    spriteTemplate[3].speedX=4;
    spriteTemplate[3].speedY=4;
    spriteTemplate[3].timerA=90;
    spriteTemplate[3].timerB=0;
    spriteTemplate[3].isRendered=TRUE;
    spriteTemplate[3].idTextura=idTexturaBalaInimigo;
    spriteTemplate[3].altura=4;
    spriteTemplate[3].largura=2;
}

void setupEnemyMine()
{
    spriteTemplate[4].type=4;
    spriteTemplate[4].hp=200;
    spriteTemplate[4].attack=100;
    spriteTemplate[4].x=0;
    spriteTemplate[4].y=0;
    spriteTemplate[4].speedX=1;
    spriteTemplate[4].speedY=1;
    spriteTemplate[4].timerA=999;
    spriteTemplate[4].timerB=0;
    spriteTemplate[4].isRendered=TRUE;
    spriteTemplate[4].idTextura=idTexturaEnemyMine;
    spriteTemplate[4].altura=15;
    spriteTemplate[4].largura=15;
}

void setupSpyEnemy()
{
    spriteTemplate[5].type=5;
    spriteTemplate[5].hp=200;
    spriteTemplate[5].attack=100;
    spriteTemplate[5].x=0;
    spriteTemplate[5].y=0;
    spriteTemplate[5].speedX=0.2;
    spriteTemplate[5].speedY=0.2;
    spriteTemplate[5].timerA=500;
    spriteTemplate[5].timerB=120;
    spriteTemplate[5].isRendered=TRUE;
    spriteTemplate[5].idTextura=idTexturaEnemySpy;
    spriteTemplate[5].altura=10;
    spriteTemplate[5].largura=15;
}

void setupEdgeShooter()
{
    spriteTemplate[6].type=6;
    spriteTemplate[6].hp=200;
    spriteTemplate[6].attack=100;
    spriteTemplate[6].x=0;
    spriteTemplate[6].y=0;
    spriteTemplate[6].speedX=1;
    spriteTemplate[6].speedY=1;
    spriteTemplate[6].timerA=900;
    spriteTemplate[6].timerB=60;
    spriteTemplate[6].isRendered=TRUE;
    spriteTemplate[6].idTextura=idTexturaEdgeShooter;
    spriteTemplate[6].altura=15;
    spriteTemplate[6].largura=15;
}

void setupDvdEnemy()
{
    spriteTemplate[7].type=7;
    spriteTemplate[7].hp=200;
    spriteTemplate[7].attack=100;
    spriteTemplate[7].x=0;
    spriteTemplate[7].y=0;
    spriteTemplate[7].speedX=1;
    spriteTemplate[7].speedY=1;
    spriteTemplate[7].timerA=900;
    spriteTemplate[7].timerB=30;
    spriteTemplate[7].isRendered=TRUE;
    spriteTemplate[7].idTextura=idTexturaInimigoAsteroide;
    spriteTemplate[7].altura=15;
    spriteTemplate[7].largura=15;
}

void setupBoss()
{
    spriteTemplate[8].type=8;
    spriteTemplate[8].hp=5000;
    spriteTemplate[8].attack=5;
    spriteTemplate[8].x=0;
    spriteTemplate[8].y=0;
    spriteTemplate[8].speedX=0;
    spriteTemplate[8].speedY=0;
    spriteTemplate[8].timerA=60;
    spriteTemplate[8].timerB=150;
    spriteTemplate[8].isRendered=TRUE;
    spriteTemplate[8].idTextura=idTexturaBoss;
    spriteTemplate[8].altura=60;
    spriteTemplate[8].largura=170;
}

void setupBossLaser()
{
    spriteTemplate[9].type=9;
    spriteTemplate[9].hp=5000;
    spriteTemplate[9].attack=10;
    spriteTemplate[9].x=0;
    spriteTemplate[9].y=0;
    spriteTemplate[9].speedX=0;
    spriteTemplate[9].speedY=-5;
    spriteTemplate[9].timerA=200;
    spriteTemplate[9].timerB=0;
    spriteTemplate[9].isRendered=TRUE;
    spriteTemplate[9].idTextura=idTexturaLazerBoss;
    spriteTemplate[9].altura=100;
    spriteTemplate[9].largura=30;
}

int main(int argc, char** argv) 
{
//inicializa a freeglut
    glutInit(&argc, argv);
    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(resolutionX, resolutionY);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Spacewar");
    glOrtho(-100, 100, -100, 100, -1, 1);

//define as callbacks
    glutDisplayFunc(displayDraw);
    glutReshapeFunc(windowResized);
    //glutKeyboardFunc(keyboardRead);
    //glutSpecialFunc(specialKeyFunc);
    glutTimerFunc(33, timerFunction, 33);
    glutMouseFunc(mouseEvent);
    glutMotionFunc(pointerMovedActive);
    glutPassiveMotionFunc(pointerMovedPassive);

//inicializa texturas
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    idTexturaBackground = carregaTextura("bg.png");
    idTexturaPlayer = carregaTextura("nave0.png");
    idTexturaTitulo = carregaTextura("titulo.png");
    idTexturaMenuJogar = carregaTextura("botaoJogar.png");
    idtexturaMenuComoJogar = carregaTextura("botaoComoJogar.png");
    idTexturaMenuInstrucoes = carregaTextura("botaoInstrucoes.png");
    idTexturaBalaPlayer = carregaTextura("playerBullet.png");
    idTexturaBalaInimigo = carregaTextura("enemyBullet.png");
    idTexturaInimigoBasico = carregaTextura("basicEnemy.png");
    idTexturaEnemyMine = carregaTextura("enemyMine.png");
    idtexturaHudPontuacao = carregaTextura("hudPontuacao.png");
    idTexturaEstrelaAcesa = carregaTextura("star1.png");
    idTexturaEstrelaApagada = carregaTextura("star2.png");
    idTexturaEnemySpy = carregaTextura("nave3.png");
    idTexturaEdgeShooter = carregaTextura("ufo.png");
    idTexturaInimigoAsteroide = carregaTextura("asteroid.png");
    idTexturaBoss = carregaTextura("boss1.png");
    idTexturaLazerBoss = carregaTextura("leizo.png");
    idTexturaNivel1Completo = carregaTextura("nivel1Completo.png");
    


//inicializa inimgos
    int temp=0;
    while(temp<100)
    {
        enemies[temp].altura =0;
        enemies[temp].angle=0;
        enemies[temp].attack=0;
        enemies[temp].hp=0;
        enemies[temp].idTextura=0;
        enemies[temp].isRendered=0;
        enemies[temp].largura=0;
        enemies[temp].speedX=0;
        enemies[temp].speedY=0;
        enemies[temp].timerA=0;
        enemies[temp].timerB=0;
        enemies[temp].type=-1;
        enemies[temp].x=-500;
        enemies[temp].y=-500;
        temp++;
    }
    setupPlayer();          //id 0
    setupPlayerBullet();    //id 1
    setupBasicEnemy();      //id 2
    setupEnemyBullet();     //id 3
    setupEnemyMine();       //id 4
    setupSpyEnemy();        //id 5
    setupEdgeShooter();     //id 6
    setupDvdEnemy();        //id 7
    setupBoss();            //id 8
    setupBossLaser();       //id 9

    nivel1Completo=0;
    bossSpawned=0;
    glutMainLoop();
    return 0;
}