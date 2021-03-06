#include<Windows.h>
#include<gl/GL.h>

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

#define WIN_WIDTH  600
#define WIN_HEIGHT 800

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
void initialize(void);

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
	TCHAR AppName[]=TEXT("My first gl native window");
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

	hwnd=CreateWindowEx(WS_EX_APPWINDOW,AppName,TEXT("OpenglNativeWindow"),WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE,0,0,800,600,NULL,NULL,hInstance,NULL);

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
	void display(void);
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
		case 0x46:
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
	pfd.dwFlags=PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL;
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
	glClearColor(0.0f,0.0f,1.0f,0.0f);
	resize(WIN_WIDTH,WIN_HEIGHT);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

void resize(int width,int height)
{
	if(height==0)
		height=1;
	glViewport(0,0,(GLsizei)width,(GLsizei)height);
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