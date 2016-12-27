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
	glutCreateWindow("2D Ortho Triangle");

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

	if(width<=height)
	{
		glOrtho(-100.0f,100.0f,(-100.0f*(height/width)),(100.0f*(height/width)),-100.0f,100.0f);
	}
	else
	{
		glOrtho(-100.0f,100.0f,(-100.0f*(width/height)),(100.0f*(width/height)),-100.0f,100.0f);
	}

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

	glBegin(GL_TRIANGLES);

	glColor3f(1.0f,0.0f,0.0f);
	
	glVertex3f(0.0f,50.0f,0.0f);
	glVertex3f(-50.0f,-50.0f,0.0f);
	glVertex3f(50.0f,-50.0f,0.0f);
	
	glEnd();
	glFlush();
}

//problem on full screen