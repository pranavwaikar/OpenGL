#include<GL/freeglut.h>
#include "Header.h"
#include "OGL.h"

bool bFullScreen=false;
GLfloat angle=0.0f;


bool bLight=false;
GLfloat LightAmbient[]={0.5f,0.5f,0.5f,1.0f};
GLfloat LightDiffuse[]={1.0f,1.0f,1.0f,1.0f};
GLfloat LightPosition[]={0.0f,0.0f,2.0f,1.0f};
bool IsTextured=false;
GLuint Texture_glass;

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
	glutCreateWindow("Model-Teapot");

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

	LoadGLTextures(&Texture_glass,MAKEINTRESOURCE(IDBITMAP_GLASS));
	glBindTexture(GL_TEXTURE_2D,Texture_glass);
	//Lights here
	
	glLightfv(GL_LIGHT1,GL_AMBIENT,LightAmbient);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,LightDiffuse);
	glLightfv(GL_LIGHT1,GL_POSITION,LightPosition);
	glEnable(GL_LIGHT1);
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
	case 'l':
		if(bLight==false)
		{
			bLight=true;
			glEnable(GL_LIGHTING);
		}
		else
		{
			bLight=false;
			glDisable(GL_LIGHTING);
		}
		break;
	case 't':
		if(IsTextured==false)
		{
			IsTextured=true;
			glEnable(GL_TEXTURE_2D);
		}
		else
		{
			IsTextured=false;
			glDisable(GL_TEXTURE_2D);
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
	
	angle=angle+0.1f;

	if(angle>=360.0f)
	{
		angle=angle-360.0f;
	}
	glutPostRedisplay();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glTranslatef(0.0f,0.0f,-3.0f);
	glRotatef(angle,0.0f,1.0f,0.0f);
	glScalef(2.0f,2.0f,2.0f);
	
	glBegin(GL_TRIANGLES);
	//triangle/pyramid code

	for(int i=0;i<sizeof(face_indicies)/sizeof(face_indicies[0]);i++)
	{
		for(int j=0;j<3;j++)
		{
			int vi=face_indicies[i][j];
			int ni=face_indicies[i][j+3];
			int ti=face_indicies[i][j+6];
			glNormal3f(normals[ni][0],normals[ni][1],normals[ni][2]);
			glTexCoord2f(textures[ti][0],textures[ti][1]);
			glVertex3f(vertices[vi][0],vertices[vi][1],vertices[vi][2]);
		}
	}
	glEnd();

	glutSwapBuffers();
}