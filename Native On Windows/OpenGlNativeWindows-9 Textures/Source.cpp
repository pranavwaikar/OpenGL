#include<Windows.h>
#include<gl/GL.h>
#include<gl/GLU.h>
#include "Header.h"

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

#define WIN_WIDTH  600
#define WIN_HEIGHT 800

GLfloat tri=0.0f;
GLfloat rect=0.0f;

GLuint Texture_kundali;
GLuint Texture_Stone;

bool bLight=false;
GLfloat LightAmbient[]={0.5f,0.5f,0.5f,1.0f};
GLfloat LightDiffuse[]={1.0f,1.0f,1.0f,1.0f};
GLfloat LightPosition[]={0.0f,0.0f,2.0f,1.0f};


LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
void initialize(void);
void spin(void);
void display(void);
int LoadGLTextures(GLuint *,TCHAR[]);

HWND ghwnd;
bool bDone=false;
bool gbActiveWindow=false;
bool gbEscKeyPressed=false;
bool gbFullScreen=false;
DWORD dwStyle;
WINDOWPLACEMENT wpPrev={sizeof(WINDOWPLACEMENT)};
HDC ghdc;
HGLRC ghrc=NULL;

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdLine,int iCmdShow)
{
	MSG msg;
	TCHAR AppName[]=TEXT("2D Rotation");
	WNDCLASSEX wndclass;
	HWND hwnd;

	wndclass.cbClsExtra=0;
	wndclass.cbSize=sizeof(wndclass);
	wndclass.cbWndExtra=0;
	wndclass.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hCursor=LoadCursor(NULL,IDC_ARROW);
	wndclass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	wndclass.hIconSm=LoadIcon(NULL,IDI_APPLICATION);
	wndclass.hInstance=hInstance;
	wndclass.lpfnWndProc=WndProc;
	wndclass.lpszClassName=AppName;
	wndclass.lpszMenuName=0;
	wndclass.style=CS_HREDRAW|CS_VREDRAW;

	RegisterClassEx(&wndclass);

	hwnd=CreateWindowEx(WS_EX_APPWINDOW,AppName,TEXT("Textures"),WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE,0,0,800,600,NULL,NULL,hInstance,NULL);

	ghwnd=hwnd;

	initialize();

	ShowWindow(hwnd,SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	while(bDone==false)
	{
		if(PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			if(msg.message==WM_QUIT)
				bDone=true;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			display();
			spin();
			if(gbActiveWindow==true)
			{
				if(gbEscKeyPressed==true)
					bDone=true;
			}
		}
	}

}

LRESULT CALLBACK WndProc(HWND hwnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
	void resize(int,int);
	void ToggleScreen(void);
	void uninitialize(void);

	switch(iMsg)
	{
	case WM_ACTIVATE:
		if(HIWORD(wParam)==0)
			gbActiveWindow=true;
		else
			gbActiveWindow=false;
		break;
	case WM_PAINT:
		display();
		break;
	case WM_ERASEBKGND:
		return (0);
		break;
	case WM_SIZE:
		resize(LOWORD(lParam),HIWORD(lParam));
		break;
	case WM_KEYDOWN:

		switch(wParam)
		{
		case VK_ESCAPE:
			if(gbEscKeyPressed==false)
				gbEscKeyPressed=true;
			break;
		case 0x46://f key
			if(gbFullScreen==false)
			{
				ToggleScreen();
				gbFullScreen=true;
			}
			else
			{
				ToggleScreen();
				gbFullScreen=false;
			}
			break;
		case 0x4C:// L key
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
		default:
			break;
		}
	case WM_LBUTTONDOWN:
		break;
	case WM_CLOSE:
		uninitialize();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return(DefWindowProc(hwnd,iMsg,wParam,lParam));
}

void ToggleScreen(void)
{
	MONITORINFO mi;

	if(gbFullScreen==false)
	{
		dwStyle=GetWindowLong(ghwnd,GWL_STYLE);
		if(dwStyle & WS_OVERLAPPEDWINDOW)
		{
			mi.cbSize=sizeof(MONITORINFO);

			if(GetWindowPlacement(ghwnd,&wpPrev)&&GetMonitorInfo(MonitorFromWindow(ghwnd,MONITORINFOF_PRIMARY),&mi))
			{
				SetWindowLong(ghwnd,GWL_STYLE,dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd,HWND_TOP,mi.rcMonitor.left,mi.rcMonitor.top,mi.rcMonitor.right-mi.rcMonitor.left,mi.rcMonitor.bottom-mi.rcMonitor.top,SWP_NOZORDER|SWP_FRAMECHANGED);
			}
		}
		ShowCursor(FALSE);
	}
	else
	{
		SetWindowLong(ghwnd,GWL_STYLE,dwStyle|WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd,&wpPrev);
		SetWindowPos(ghwnd,HWND_TOP,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOOWNERZORDER|SWP_NOZORDER|SWP_FRAMECHANGED);
		ShowCursor(TRUE);
	}
}

void initialize(void)
{
	void resize(int,int);

	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex;

	ZeroMemory(&pfd,sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize=sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion=1;
	pfd.dwFlags=PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
	pfd.iPixelType=PFD_TYPE_RGBA;
	pfd.cColorBits=32;
	pfd.cRedBits=8;
	pfd.cGreenBits=8;
	pfd.cBlueBits=8;
	pfd.cAlphaBits=8;
	pfd.cDepthBits=32;

	ghdc=GetDC(ghwnd);
	iPixelFormatIndex=ChoosePixelFormat(ghdc,&pfd);

	if(iPixelFormatIndex==0)
	{
		ReleaseDC(ghwnd,ghdc);
		ghdc=NULL;
	}
	if(SetPixelFormat(ghdc,iPixelFormatIndex,&pfd)==false)
	{
		ReleaseDC(ghwnd,ghdc);
		ghdc=NULL;
	}

	ghrc=wglCreateContext(ghdc);
	if(ghrc==NULL)
	{
		ReleaseDC(ghwnd,ghdc);
		ghdc=NULL;
	}

	if(wglMakeCurrent(ghdc,ghrc)==false)
	{
		wglDeleteContext(ghrc);
		ghrc=NULL;
		ReleaseDC(ghwnd,ghdc);
		ghdc=NULL;
	}


	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

		//activate light
	glLightfv(GL_LIGHT1,GL_AMBIENT,LightAmbient);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,LightDiffuse);
	glLightfv(GL_LIGHT1,GL_POSITION,LightPosition);
	glEnable(GL_LIGHT1);

	//texture loadings
	LoadGLTextures(&Texture_kundali,MAKEINTRESOURCE(IDBITMAP_KUNDALI));
	LoadGLTextures(&Texture_Stone,MAKEINTRESOURCE(IDBITMAP_STONE));
	//enable textures here
	glEnable(GL_TEXTURE_2D);

	resize(WIN_WIDTH,WIN_HEIGHT);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glTranslatef(-2.0f,0.0f,-7.0f);
	glBindTexture(GL_TEXTURE_2D,Texture_Stone);
	glRotatef(tri,0.0f,1.0f,0.0f);

	glBegin(GL_TRIANGLES);

	//now add normal to every surface
	glNormal3f(-1.0f,0.0f,0.0f);
	glTexCoord2f(0.5f,1.0f);
	glVertex3f(0.0f,1.0f,0.0f);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(-1.0f,-1.0f,1.0f);
	glTexCoord2f(1.0f,0.0f);
	glVertex3f(1.0f,-1.0f,1.0f);

	glNormal3f(0.0f,1.0f,0.0f);
	glTexCoord2f(0.5f,1.0f);
	glVertex3f(0.0f,1.0f,0.0f);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(1.0f,-1.0f,-1.0f);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(-1.0f,-1.0f,-1.0f);

	glNormal3f(0.0f,0.0f,1.0f);
	glTexCoord2f(0.5f,1.0f);
	glVertex3f(0.0f,1.0f,0.0f);
	glTexCoord2f(1.0f,0.0f);
	glVertex3f(1.0f,-1.0f,1.0f);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(1.0f,-1.0f,-1.0f);

	glNormal3f(0.0f, 0.0f, -1.0f);
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
	
	glNormal3f(0.0f,1.0f,0.0f);
	glTexCoord2f(0.0f,1.0f);
	glVertex3f(1.0f,1.0f,-1.0f);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(-1.0f,1.0f,-1.0f);
	glTexCoord2f(1.0f,0.0f);
	glVertex3f(-1.0f,1.0f,1.0f);
	glTexCoord2f(1.0f,1.0f);
	glVertex3f(1.0f,1.0f,1.0f);

	glNormal3f(0.0f,-1.0f,0.0f);
	glTexCoord2f(1.0f,1.0f);
	glVertex3f(1.0f,-1.0f,-1.0f);
	glTexCoord2f(0.0f,1.0f);
	glVertex3f(-1.0f,-1.0f,-1.0f);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(-1.0f,-1.0f,1.0f);
	glTexCoord2f(1.0f,0.0f);
	glVertex3f(1.0f,-1.0f,1.0f);


	glNormal3f(0.0f,0.0f,1.0f);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(1.0f,1.0f,1.0f);
	glTexCoord2f(1.0f,0.0f);
	glVertex3f(-1.0f,1.0f,1.0f);
	glTexCoord2f(1.0f,1.0f);
	glVertex3f(-1.0f,-1.0f,1.0f);
	glTexCoord2f(0.0f,1.0f);
	glVertex3f(1.0f,-1.0f,1.0f);

	glNormal3f(0.0f,0.0f,-1.0f);
	glTexCoord2f(1.0f,0.0f);
	glVertex3f(1.0f,1.0f,-1.0f);
	glTexCoord2f(1.0f,1.0f);
	glVertex3f(-1.0f,1.0f,-1.0f);
	glTexCoord2f(0.0f,1.0f);
	glVertex3f(-1.0f,-1.0f,-1.0f);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(1.0f,-1.0f,-1.0f);

	glNormal3f(-1.0f,0.0f,0.0f);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(-1.0f,1.0f,1.0f);
	glTexCoord2f(1.0f,0.0f);
	glVertex3f(-1.0f,1.0f,-1.0f);
	glTexCoord2f(1.0f,1.0f);
	glVertex3f(-1.0f,-1.0f,-1.0f);
	glTexCoord2f(0.0f,1.0f);
	glVertex3f(-1.0f,-1.0f,1.0f);

	glNormal3f(1.0f,0.0f,0.0f);
	glTexCoord2f(1.0f,0.0f);
	glVertex3f(1.0f,1.0f,-1.0f);
	glTexCoord2f(1.0f,1.0f);
	glVertex3f(1.0f,1.0f,1.0f);
	glTexCoord2f(0.0f,1.0f);
	glVertex3f(1.0f,-1.0f,1.0f);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(1.0f,-1.0f,-1.0f);

	glEnd();

	SwapBuffers(ghdc);
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
}


void uninitialize()
{
	if(gbFullScreen==true)
	{
		dwStyle=GetWindowLong(ghwnd,GWL_STYLE);
		SetWindowLong(ghwnd,GWL_STYLE,dwStyle|WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd,&wpPrev);
		SetWindowPos(ghwnd,HWND_TOP,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOOWNERZORDER|SWP_NOZORDER|SWP_FRAMECHANGED);
		ShowCursor(TRUE);
	}
	wglMakeCurrent(NULL,NULL);
	wglDeleteContext(ghrc);
	ghrc=NULL;
	ReleaseDC(ghwnd,ghdc);
	ghdc=NULL;
	DestroyWindow(ghwnd);
}