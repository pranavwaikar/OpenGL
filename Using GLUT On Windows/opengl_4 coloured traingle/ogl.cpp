#include<GL/freeglut.h>

bool bFullScreen=false;

int main(int argc,char **argv)
{
	void initialize();
	void uninitialize();
	void keyboard(unsigned char,int,int);
	void mouse(int,int,int,int);
	void display();
	void resize(int,int);

	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(800,800);
	glutInitWindowPosition(100,100);
	glutCreateWindow("2D  Traingle Prespective ");

	//initialize();
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutCloseFunc(uninitialize);

	glutMainLoop();

	// return 0;
}


void initailize()
{
	glClearColor(0.0f,0.0f,1.0f,0.0f);
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
		break;
	default:
		break;
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//TRIANGLE CREATION

	glLoadIdentity();

	glTranslatef(0.0f,0.0f,-3.0f);


	glBegin(GL_TRIANGLES);
	
	glColor3f(1.0f,0.0f,0.0f);
	glVertex3f(0.0f,1.0f,0.0f);

	glColor3f(1.0f,1.0f,1.0f);
	glVertex3f(-1.0f,-1.0f,0.0f);
	
	glColor3f(0.0f,0.0f,1.0f);
	glVertex3f(1.0f,-1.0f,0.0f);

	glEnd();

	glFlush();
}

void resize(int width,int height)
{
	if(height==0)
	{
		height=1;
	}
	
	glViewport(0,0,(GLsizei)width,(GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}