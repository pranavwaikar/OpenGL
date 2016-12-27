#include<GL/freeglut.h>

bool bFullScreen=false;
GLfloat tri=0.0f;
GLfloat rect=0.0f;

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
	glutCreateWindow("3D rotation");

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
	glViewport(0,0,(GLsizei)width,(GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,1.0f,100.0f);

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

	glBegin(GL_TRIANGLES);


	glColor3f(1.0f,0.0f,0.0f);
	glVertex3f(0.0f,1.0f,0.0f);
	glColor3f(1.0f,1.0f,1.0f);
	glVertex3f(-1.0f,-1.0f,1.0f);
	glColor3f(0.0f,0.0f,1.0f);
	glVertex3f(1.0f,-1.0f,1.0f);

	glColor3f(1.0f,0.0f,0.0f);
	glVertex3f(0.0f,1.0f,0.0f);
	glColor3f(1.0f,1.0f,1.0f);
	glVertex3f(1.0f,-1.0f,1.0f);
	glColor3f(0.0f,0.0f,1.0f);
	glVertex3f(1.0f,-1.0f,-1.0f);

	glColor3f(1.0f,0.0f,0.0f);
	glVertex3f(0.0f,1.0f,0.0f);
	glColor3f(1.0f,1.0f,1.0f);
	glVertex3f(1.0f,-1.0f,-1.0f);
	glColor3f(0.0f,0.0f,1.0f);
	glVertex3f(-1.0f,-1.0f,-1.0f);

	glColor3f(1.0f,0.0f,0.0f);
	glVertex3f(0.0f,1.0f,0.0f);
	glColor3f(1.0f,1.0f,1.0f);
	glVertex3f(-1.0f,-1.0f,-1.0f);
	glColor3f(0.0f,0.0f,1.0f);
	glVertex3f(-1.0f,-1.0f,1.0f);
	

	glEnd();
	//square
	glLoadIdentity();
	glTranslatef(2.0f,0.0f,-7.0f);
	glScalef(0.75f,0.75f,0.75f);
	glRotatef(rect,1.0f,1.0f,1.0f);

	glBegin(GL_QUADS);
	
	glColor3f(1.0f,0.0f,0.0f);
	glVertex3f(1.0f,1.0f,-1.0f);
	glVertex3f(-1.0f,1.0f,-1.0f);
	glVertex3f(-1.0f,1.0f,1.0f);
	glVertex3f(1.0f,1.0f,1.0f);

	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(1.0f,-1.0f,-1.0f);
	glVertex3f(-1.0f,-1.0f,-1.0f);
	glVertex3f(-1.0f,-1.0f,1.0f);
	glVertex3f(1.0f,-1.0f,1.0f);

	glColor3f(0.0f,0.0f,1.0f);
	glVertex3f(1.0f,1.0f,1.0f);
	glVertex3f(-1.0f,1.0f,1.0f);
	glVertex3f(-1.0f,-1.0f,1.0f);
	glVertex3f(1.0f,-1.0f,1.0f);

	glColor3f(1.0f,1.0f,0.0f);
	glVertex3f(1.0f,1.0f,-1.0f);
	glVertex3f(-1.0f,1.0f,-1.0f);
	glVertex3f(-1.0f,-1.0f,-1.0f);
	glVertex3f(1.0f,-1.0f,-1.0f);

	glColor3f(0.0f,1.0f,1.0f);
	glVertex3f(-1.0f,1.0f,1.0f);
	glVertex3f(-1.0f,1.0f,-1.0f);
	glVertex3f(-1.0f,-1.0f,-1.0f);
	glVertex3f(-1.0f,-1.0f,1.0f);

	glColor3f(1.0f,0.0f,1.0f);
	glVertex3f(1.0f,1.0f,-1.0f);
	glVertex3f(1.0f,1.0f,1.0f);
	glVertex3f(1.0f,-1.0f,1.0f);
	glVertex3f(1.0f,-1.0f,-1.0f);

	glEnd();

	glutSwapBuffers();
}