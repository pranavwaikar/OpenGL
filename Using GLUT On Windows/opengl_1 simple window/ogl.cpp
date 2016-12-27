#include <GL/freeglut.h>

bool blFullScreen=false;

int main(int argc,char **argv)
{
	void display();
	void resize(int,int);
	void keyboard(unsigned char,int,int);
	void mouse(int,int,int,int);
	void initialize();
	void uninitialize();

	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(800,800);
	glutInitWindowPosition(100,100);
	glutCreateWindow("OpenGL: Hello world!!");

	initialize();
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutCloseFunc(uninitialize);

	glutMainLoop();

	//return 0;

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

void initialize()
{
	glClearColor(0.0f,0.0f,1.0f,0.0f);
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
			glutFullScreen();
			blFullScreen=true;
		}
		else
		{
			glutLeaveFullScreen();
			blFullScreen=true;
		}
		break;
	default:
		break;
	}
}

void uninitialize()
{


}

void mouse(int btn,int state,int x,int y)
{
	switch (btn)
	{
	case GLUT_LEFT_BUTTON:
		break;
	default:
		break;
	}
}

void resize(int width,int height)
{

}