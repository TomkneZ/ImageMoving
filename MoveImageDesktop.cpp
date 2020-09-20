#include <Windows.h>
#include <gdiplus.h>
#include <shellapi.h>
#include <math.h>
#include <tchar.h>

#pragma comment (lib,"Gdiplus.lib")

#define PATH_TO_IMAGE_BMP L"Tamara.bmp"
#define MOVE_LENGTH 20

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawImage(HDC, HWND);
void MoveImageOnMousewheel(WPARAM);
void MoveImageOnKeyboard(WPARAM, HDC, HWND);

static TCHAR szWindowClass[] = _T("Tamara's Lab1 on OSaSP");

INT width = 900;
INT length = 750;

INT xPosition = 0;
INT yPosition = 200;

INT windowHeight = 0;
INT windowWidth = 0;

INT imageHeight = 0;
INT imageWidth = 0;

// hInstance - дескриптор экземпл€ра приложени€. Ётот дескриптор содержит адрес начала кода программы в ее адресном пространстве.
// hPrevInstance - дескриптор предыдущего экземпл€ра приложени€, почти всегда равен NULL
// lpCmdLine - указатель на начало командной строки, введенной при запуске программы
// nCmdShow - это значение содержит желаемый вид окна (например, свернутый или развернутый)

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	HWND hWnd;
	MSG msg;

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = wcex.hIcon;
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);
		return 1;
	}
	// The parameters to CreateWindow explained:
	// szWindowClass: the name of the application
	// szTitle: the text that appears in the title bar
	// WS_OVERLAPPEDWINDOW: the type of window to create
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
	// initial size (width, length)
	// NULL: the parent of this window
	// NULL: this application does not have a menu bar
	// hInstance: the first parameter from WinMain
	// NULL: not used in this application
	hWnd = CreateWindow(
		szWindowClass,
		szWindowClass,
		WS_OVERLAPPEDWINDOW,
		100, 10,
		width, length,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	// Sets the specified window's show state
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	RECT rect;
	if (GetWindowRect(hWnd, &rect))
	{
		windowWidth = rect.right - rect.left;
		windowHeight = rect.bottom - rect.top;
	}

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Gdiplus::GdiplusShutdown(gdiplusToken);
	return (int)msg.wParam;
}

void DrawImage(HDC hdc, HWND hWnd)
{
	RECT rcClient;
	GetClientRect(hWnd, &rcClient);
	int left = rcClient.left;
	int top = rcClient.top;
	int width = rcClient.right - rcClient.left;
	int height = rcClient.bottom - rcClient.top;

	HDC hdcMem = CreateCompatibleDC(hdc);
	const int nMemDC = SaveDC(hdcMem);

	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);
	SelectObject(hdcMem, hBitmap);

	Gdiplus::Graphics graphics(hdcMem);
	Gdiplus::Bitmap bmp(PATH_TO_IMAGE_BMP);

	imageWidth = bmp.GetWidth();
	imageHeight = bmp.GetHeight();

	Gdiplus::REAL imageCenterWidth = xPosition + imageWidth / 2;
	Gdiplus::REAL imageCenterHight = yPosition + imageHeight / 2;

	RECT rect;
	if (GetWindowRect(hWnd, &rect))
	{
		windowWidth = rect.right - rect.left;
		windowHeight = rect.bottom - rect.top;
	}
	FillRect(hdcMem, &rcClient, (HBRUSH)(COLOR_WINDOW + 1));

	graphics.DrawImage(&bmp, xPosition, yPosition);
	BitBlt(hdc, left, top, width, height, hdcMem, left, top, SRCCOPY);
	RestoreDC(hdcMem, nMemDC);
	DeleteObject(hBitmap);
}

void ProcessDraggedFiles(HWND hWnd, WPARAM wParam) {
	LPWSTR fileName = NULL;
	DragQueryFileW((HDROP)wParam, 0, fileName, 200);
}
void Move_1_2() {
	while (xPosition >= length/4)
	{
		xPosition -= 1;
	}
	while (yPosition >= 0)
	{
		yPosition -= 1;
	}
}

void Move_2_3()
{
	while (xPosition >= 0)
	{
		xPosition -= 1;
	}
	while (yPosition <= width/4)
	{
		yPosition += 1;
	}
}
void Move_3_4()
{
	while (xPosition <= length/2)
	{
		xPosition += 1;
	}
	while (yPosition <= width/2)
	{
		yPosition += 1;
	}
}

void Move_4_1()
{
	while (xPosition <= length-100)
	{
		xPosition += 1;
	}
	while (yPosition >= width/5)
	{
		yPosition -= 1;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_TIMER:		
		if (xPosition >= width/4)
		{
			if (yPosition >= length/4)
			{
				// 4 четверть
				Move_4_1();				
			}
			else
			{
				//1 четверть
				Move_1_2();				
			}
		}
		else
		{
			if (yPosition <= length/4)
			{
				// 2 четверть
				Move_2_3();		

			}
			else
			{
				// 3 четверть
				Move_3_4();							
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_CREATE:		
		DragAcceptFiles(hWnd, true);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		DrawImage(hdc, hWnd);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		DragAcceptFiles(hWnd, false);
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		hdc = BeginPaint(hWnd, &ps);
		MoveImageOnKeyboard(wParam, hdc, hWnd);
		EndPaint(hWnd, &ps);
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_MOUSEWHEEL:
		MoveImageOnMousewheel(wParam);
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DROPFILES:
		ProcessDraggedFiles(hWnd, wParam);
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_ERASEBKGND:
		return FALSE;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void MoveLeft() {
	if (xPosition >= MOVE_LENGTH)
		xPosition -= MOVE_LENGTH;
}

void MoveRight() {
	if (xPosition + imageWidth < windowWidth + 2 * MOVE_LENGTH)
		xPosition += MOVE_LENGTH;
}

void MoveUp() {
	if (yPosition >= MOVE_LENGTH)
		yPosition -= MOVE_LENGTH;
}

void MoveDown() {
	if (yPosition + imageHeight < windowHeight + 2 * MOVE_LENGTH)
		yPosition += MOVE_LENGTH;
}

void MoveImageOnMousewheel(WPARAM wParam) {
	if (GET_KEYSTATE_WPARAM(wParam) == MK_SHIFT) {
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) {
			MoveLeft();
		}
		else {
			MoveRight();
		}
	}
	else {
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) {
			MoveUp();
		}
		else {
			MoveDown();
		}
	}
}

void MoveImageOnKeyboard(WPARAM wParam, HDC hdc, HWND hWnd) {
	switch (wParam) {
	case VK_CONTROL:
		SetTimer(hWnd, 1, 500, NULL);		
		break;
	case VK_ESCAPE:
		KillTimer(hWnd, 1);
		break;
	case VK_LEFT:
		MoveLeft();
		break;
	case VK_RIGHT:
		MoveRight();
		break;
	case VK_UP:
		MoveUp();
		break;
	case VK_DOWN:
		MoveDown();
		break;
	}
}
