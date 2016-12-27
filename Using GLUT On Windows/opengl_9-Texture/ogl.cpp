#include<GL/freeglut.h>
#include "Header.h"
bool bFullScreen=false;
GLfloat tri=0.0f;
GLfloat rect=0.0f;

GLuint Texture_kundali;
GLuint Texture_Stone;

int LoadGLTextures(GLuint *,TCHAR[]);

int main(int argc,char **argv)
{
	void initialize();
	void uninitialize();
	void keyboard(unsigned char,int,int);
	void mouse(int,int,int,int);
	void display();
	void resize(int,int);
	void spin();


	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowSize(800,800);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Texture App");

	initialize();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutReshapeFunc(resize);
	glutCloseFunc(uninitialize);
	glutIdleFunc(spin);

	glutMainLoop();

	// return 0;
}

void initialize()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

	LoadGLTextures(&Texture_kundali,MAKEINTRESOURCE(IDBITMAP_KUNDALI));
	LoadGLTextures(&Texture_Stone,MAKEINTRESOURCE(IDBITMAP_STONE));
	//enable textures here
	glEnable(GL_TEXTURE_2D);
}

int LoadGLTextures(GLuint *texture,TCHAR ImgResourceID[])
{
	HBITMAP hBitmap;
	BITMAP bmp;
	int iStatus;

	glGenTextures(1,texture);
	hBitmap=(HBITMAP)LoadImage(GetModuleHandle(NULL),ImgResourceID,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION);
	if(hBitmap)
	{
		iStatus=TRUE;
		GetObject(hBitmap,sizeof(bmp),&bmp);
		glPixelStorei(GL_UNPACK_ALIGNMENT,4);
		glBindTexture(GL_TEXTURE_2D,*texture);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);

		gluBuild2DMipmaps(GL_TEXTURE_2D,3,bmp.bmWidth,bmp.bmHeight,GL_BGR_EXT,GL_UNSIGNED_BYTE,bmp.bmBits);

		DeleteObject(hBitmap);
	}
	return (iStatus);
}

void uninitialize()
{

}

void keyboard(unsigned char key,int x,int y)
{
	switch(key)
	{
	case 27:
		glutLeaveMainLoop();
		break;
	case 'f':
		if(bFullScreen==false)
		{
			bFullScreen=true;
			glutFullScreen();
		}
		else
		{
			bFullScreen=false;
			glutLeaveFullScreen();
		}
		break;
	default:
		break;
	}
}

void mouse(int btn,int state,int x,int y)
{
	switch(btn)
	{
	case GLUT_LEFT_BUTTON:
		//MessageBox(NULL,TEXT("HEY"),TEXT("HI"),MB_OK);
		break;
	default:
		break;
	}
}

void resize(int width,int height)
{
	if(height==0)
		height=1;
	glViewport(0,0,(GLsizei)width,(GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void spin()
{
	
	tri=tri+0.1f;

	if(tri>=360.0f)
	{
		tri=0.0f;
	}

	rect=rect+0.1f;

	if(rect>=360.0f)
	{
		rect=0.0f;
	}
	glutPostRedisplay();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glTranslatef(-2.0f,0.0f,-7.0f);
	glRotatef(tri,0.0f,1.0f,0.0f);
	glBindTexture(GL_TEXTURE_2D,Texture_Stone);
	glBegin(GL_TRIANGLES);
	//triangle/pyramid code

	glTexCoord2f(0.5f,1.0f);
	glVertex3f(0.0f,1.0f,0.0f);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(-1.0f,-1.0f,1.0f);
	glTexCoord2f(1.0f,0.0f);
	glVertex3f(1.0f,-1.0f,1.0f);

	//right face
	glTexCoord2f(0.5f,1.0f);
	glVertex3f(0.0f,1.0f,0.0f);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(1.0f,-1.0f,1.0f);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(1.0f,-1.0f,-1.0f);

	//back face
	glTexCoord2f(0.5f,1.0f);
	glVertex3f(0.0f,1.0f,0.0f);
	glTexCoord2f(1.0f,0.0f);
	glVertex3f(1.0f,-1.0f,-1.0f);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(-1.0f,-1.0f,-1.0f);

	//left side
	glTexCoord2f(0.5f,1.0f);
	glVertex3f(0.0f,1.0f,0.0f);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(-1.0f,-1.0f,-1.0f);
	glTexCoord2f(1.0f,0.0f);
	glVertex3f(-1.0f,-1.0f,1.0f);
	

	glEnd();
	//square
	glLoadIdentity();
	glTranslatef(2.0f,0.0f,-7.0f);
	glScalef(0.75f,0.75f,0.75f);
	glRotatef(rect,1.0f,1.0f,1.0f);
	glBindTexture(GL_TEXTURE_2D,Texture_kundali);

	glBegin(GL_QUADS);
	
	//top
	glTexCoord2f(0.0f,1.0f);
	glVertex3f(1.0f,1.0f,-1.0f);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(-1.0f,1.0f,-1.0f);
	glTexCoord2f(1.0f,0.0f);
	glVertex3f(-1.0f,1.0f,1.0f);
	glTexCoord2f(1.0f,1.0f);
	glVertex3f(1.0f,1.0f,1.0f);

	//bottom
	glTexCoord2f(1.0f,1.0f);
	glVertex3f(1.0f,-1.0f,-1.0f);
	glTexCoord2f(0.0f,1.0f);
	glVertex3f(-1.0f,-1.0f,-1.0f);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(-1.0f,-1.0f,1.0f);
	glTexCoord2f(1.0f,0.0f);
	glVertex3f(1.0f,-1.0f,1.0f);

	//front
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(1.0f,1.0f,1.0f);
	glTexCoord2f(1.0f,0.0f);
	glVertex3f(-1.0f,1.0f,1.0f);
	glTexCoord2f(1.0f,1.0f);
	glVertex3f(-1.0f,-1.0f,1.0f);
	glTexCoord2f(0.0f,1.0f);
	glVertex3f(1.0f,-1.0f,1.0f);

	//back
	glTexCoord2f(1.0f,0.0f);
	glVertex3f(1.0f,1.0f,-1.0f);
	glTexCoord2f(1.0f,1.0f);
	glVertex3f(-1.0f,1.0f,-1.0f);
	glTexCoord2f(0.0f,1.0f);
	glVertex3f(-1.0f,-1.0f,-1.0f);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(1.0f,-1.0f,-1.0f);

	//left
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(-1.0f,1.0f,1.0f);
	glTexCoord2f(1.0f,0.0f);
	glVertex3f(-1.0f,1.0f,-1.0f);
	glTexCoord2f(1.0f,1.0f);
	glVertex3f(-1.0f,-1.0f,-1.0f);
	glTexCoord2f(0.0f,1.0f);
	glVertex3f(-1.0f,-1.0f,1.0f);

	//right 
	glTexCoord2f(1.0f,0.0f);
	glVertex3f(1.0f,1.0f,-1.0f);
	glTexCoord2f(1.0f,1.0f);
	glVertex3f(1.0f,1.0f,1.0f);
	glTexCoord2f(0.0f,1.0f);
	glVertex3f(1.0f,-1.0f,1.0f);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(1.0f,-1.0f,-1.0f);

	glEnd();

	glutSwapBuffers();
}