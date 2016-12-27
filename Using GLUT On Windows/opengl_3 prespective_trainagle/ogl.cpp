#include<GL\freeglut.h>

bool blFullScreen=false;



int main(int argc,char **argv)
{
	void initialize();
	void uninitialize();
	void keyboard(unsigned char,int,int);
	void mouse(int,int,int,int);
	void resize(int,int);
	void display();


	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	//glutInitWindowSize(GetSystemMetrics(SM_CXFULLSCREEN),GetSystemMetrics(SM_CYFULLSCREEN));
	glutInitWindowSize(800,800);
	glutInitWindowPosition(100,100);
	glutCreateWindow("2D pers Triangle");

	initialize();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutReshapeFunc(resize);
	glutCloseFunc(uninitialize);

	glutMainLoop();


	//return 0;
}

void initialize()
{
	glClearColor(0.0f,0.0f,1.0f,0.0f);
}

void uninitialize()
{

}

void resize(int height,int width)
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

void keyboard(unsigned char key,int x,int y)
{
	switch(key)
	{
	case 27:
		glutLeaveMainLoop();
		break;
	case 'f':
		if(blFullScreen==false)
		{
			blFullScreen=true;
			glutFullScreen();
		}
		else
		{
			glutLeaveFullScreen();
			blFullScreen=false;
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

	glLoadIdentity();
	glTranslatef(0.0f,0.0f,-7.0f);

	glBegin(GL_TRIANGLES);

	glColor3f(1.0f,0.0f,0.0f);
	
	glVertex3f(0.0f,1.0f,0.0f); //apex
	glVertex3f(-1.0f,-1.0f,0.0f); //left-botton 
	glVertex3f(1.0f,-1.0f,0.0f); //right-bottom 
	
	glEnd();
	glFlush();
}

//problem on full screen