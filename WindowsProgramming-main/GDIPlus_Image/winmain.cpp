#include <windows.h>
#include <stdio.h>

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

LPCTSTR g_szClassName = TEXT("윈도우 클래스 이름");
int g_width = 1024;
int g_height = 768;

HWND g_hWnd;
HDC g_FrontBufferDC;    // 앞면 DC
HDC g_BackBufferDC;    // 뒷면 DC
HBITMAP g_BackBufferBitmap;

// 콘솔 초기화
void InitConsole()
{
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	SetConsoleTitle(L"윈도우 메시지 콘솔 로그");
	printf("콘솔 로그 시작...\n\n");
}

void UninitConsole()
{
	// 표준 출력 스트림 닫기
	fclose(stdout);
	// 콘솔 해제
	FreeConsole();
}

// WIN32 API 에러 값에 대한 실제 메세지를 출력하는 함수
void PrintLastErrorMessage()
{
	DWORD errorCode = GetLastError();
	LPVOID lpMsgBuf;

	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // 기본 언어
		(LPSTR)&lpMsgBuf,
		0,
		NULL);

	if (lpMsgBuf)
	{
		printf("오류 코드: %lu\n오류 메시지: %s\n", errorCode, (char*)lpMsgBuf);
		LocalFree(lpMsgBuf); // 할당된 버퍼 해제
	}
	else
	{
		printf("오류 코드: %lu (메시지를 찾을 수 없음)\n", errorCode);
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	InitConsole();  // 콘솔 출력 초기화

	char szPath[MAX_PATH] = { 0, };
	::GetCurrentDirectoryA(MAX_PATH, szPath);
	printf("Current Directory: %s\n", szPath);

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = g_szClassName;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	// 기본 커서 모양
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// 기본 아이콘 모양
	RegisterClass(&wc);

	// 원하는 크기가 조정되어 리턴
	RECT rcClient = { 0, 0, (LONG)g_width, (LONG)g_height };
	AdjustWindowRect(&rcClient, WS_OVERLAPPEDWINDOW, FALSE);

	//생성
	HWND hwnd = CreateWindow(
		g_szClassName,
		TEXT("윈도우 타이틀바에 표시할 문자열"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	

	// GDI+ 초기화
	ULONG_PTR g_GdiPlusToken;
	Gdiplus::GdiplusStartupInput gsi;
	Gdiplus::GdiplusStartup(&g_GdiPlusToken, &gsi, nullptr);
	Gdiplus::Graphics* g_pBackBufferGraphics = Gdiplus::Graphics::FromHDC(g_BackBufferDC);

	RendererInitialize(hwnd);

	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	
		Render(hwnd, g_pBackBufferGraphics);
		

		// Renderer::EndDraw()
		BitBlt(g_FrontBufferDC, 0, 0, g_width, g_height, g_BackBufferDC, 0, 0, SRCCOPY);
	}

	// Renderer::Uninitialize
	
	// GDI+ 해제
	delete g_pImageBitmap;
	delete g_pBackBufferGraphics;
	Gdiplus::GdiplusShutdown(g_GdiPlusToken);

	DeleteObject(g_BackBufferBitmap);
	DeleteDC(g_BackBufferDC);
	ReleaseDC(hwnd, g_FrontBufferDC);
	//////////////////////////////////////////////////////////////////////////

	UninitConsole();  // 콘솔 출력 해제
	return (int)msg.wParam;
}

void RendererInitialize(HWND hwnd)
{

	////////Renderer::Initialize
	g_FrontBufferDC = GetDC(hwnd); //윈도우 클라이언트 영역의 DeviceContext얻기
	g_BackBufferDC = CreateCompatibleDC(g_FrontBufferDC); // 호환되는 DeviceContext 생성
	g_BackBufferBitmap = CreateCompatibleBitmap(g_FrontBufferDC, g_width, g_height); // 메모리 영역생성
	SelectObject(g_BackBufferDC, g_BackBufferBitmap); // MemDC의 메모리영역 지정
}

struct PNGmaps
{
	Gdiplus::Bitmap* g_pImageBitmap1 = new Gdiplus::Bitmap(L"../Resource/work1.png");
	Gdiplus::Bitmap* g_pImageBitmap2 = new Gdiplus::Bitmap(L"../Resource/work2.png");
	Gdiplus::Bitmap* g_pImageBitmap3 = new Gdiplus::Bitmap(L"../Resource/work3.png");
	Gdiplus::Bitmap* g_pImageBitmap4 = new Gdiplus::Bitmap(L"../Resource/work4.png");
	Gdiplus::Bitmap* g_pImageBitmap5 = new Gdiplus::Bitmap(L"../Resource/work5.png");
	Gdiplus::Bitmap* g_pImageBitmap6 = new Gdiplus::Bitmap(L"../Resource/work6.png");
	Gdiplus::Bitmap* g_pImageBitmap7 = new Gdiplus::Bitmap(L"../Resource/work7.png");
	Gdiplus::Bitmap* g_pImageBitmapStop = new Gdiplus::Bitmap(L"../Resource/workStop.png");
};

void Render(HWND hwnd, Gdiplus::Graphics* g_pBackBufferGraphics)
{
	PNGmaps* pngmaps; 

	//Render()
	// 이미지를 불러와서, 배열로 저장해서 , 출력. 
	// 애니메이션으로 만들 이미지 전부를 따로 관리해 모으고, 
	// 모은 이미지를 불러서 배열에 저장하여서, 
	// 배열 내의 모든 요소를 순회시켜서 반복 출력 시킨다. 
	// 프레임 내의 Update() 에서 요소를 순서대로 불러서 출력한다. 

	//ULONG_PTR g_GdiPlusToken;
	//Gdiplus::GdiplusStartupInput gsi;
	//Gdiplus::GdiplusStartup(&g_GdiPlusToken, &gsi, nullptr);
	//Gdiplus::Graphics* g_pBackBufferGraphics = Gdiplus::Graphics::FromHDC(g_BackBufferDC);

	// 이미지 로드
	Gdiplus::Bitmap* g_pImageBitmap = new Gdiplus::Bitmap(L"../Resource/elf32.png");
	UINT witdh = g_pImageBitmap->GetWidth();
	UINT height = g_pImageBitmap->GetHeight();


	for (int BitmapIndex = 0; BitmapIndex < sizeof(PNGmaps); ++BitmapIndex)
	{
		if (g_pImageBitmap->GetLastStatus() != Gdiplus::Ok)
		{
			MessageBox(hwnd, L"PNG 파일 로드 실패", L"오류", MB_ICONERROR);
			PostQuitMessage(0);
		}
		//0번 요소 부터 순회
		PNGmaps anim = pngmaps[BitmapIndex]; 
		
	}


	
	// Renderer::BeginDraw()
	PatBlt(g_BackBufferDC, 0, 0, g_width, g_height, BLACKNESS);

	g_pBackBufferGraphics->DrawImage(g_pImageBitmap, 0, 0);
	//Render();
	return g_pImageBitmap;
}