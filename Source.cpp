// Windows Header Files:
#include <Windows.h>
#include <CommCtrl.h>

// C RunTime Header Files
#include <math.h>
#include <objbase.h>
#include <stdio.h>
#include <tchar.h>

/******************************************************************
*                                                                 *
*  Macros and definitions                                         *
*                                                                 *
******************************************************************/
template<class Interface>
inline void
SafeRelease(Interface **ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = NULL;
	}
}

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

#define pi 3.14
#define ScreenX GetSystemMetrics(SM_CXSCREEN)
#define ScreenY GetSystemMetrics(SM_CYSCREEN)
#define MAPWIDTH 1024
#define MAPHEIGHT 768

/******************************************************************
*                                                                 *
*  DemoApp                                                        *
*                                                                 *
******************************************************************/

class DemoApp
{
public:
	DemoApp();
	~DemoApp();

	HRESULT Initialize();

	void RunMessageLoop();

private:
	HRESULT CreateResources();
	void DiscardResources();

	static LRESULT CALLBACK WndProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
		);

private:
	HWND m_hwnd;

};

int i, xView, yView;
double y;
TCHAR Buf[5];
int ItemIndex = 0;

// Процедура рисовании линии
BOOL Line(HDC hdc, int x1, int y1, int x2, int y2);
void PAR(HDC hdc);
void GIP(HDC hdc);
void SIN(HDC hdc);
void TAN(HDC hdc);



/******************************************************************
*                                                                 *
*  The application entry point.                                   *
*                                                                 *
******************************************************************/

int WINAPI WinMain(
	HINSTANCE     /* hInstance */,
	HINSTANCE     /* hPrevInstance */,
	LPSTR     /* lpCmdLine */,
	int     /* nCmdShow */
	)
{
	// Ignore the return value because we want to run the program even in the
	// unlikely event that HeapSetInformation fails.
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
	if (SUCCEEDED(CoInitialize(NULL)))
	{
		{
			DemoApp app;

			if (SUCCEEDED(app.Initialize()))
			{
				app.RunMessageLoop();
			}
		}
		CoUninitialize();
	}

	return 0;
}

/******************************************************************
*                                                                 *
*  DemoApp::DemoApp constructor                                   *
*                                                                 *
*  Initialize member data.                                         *
*                                                                 *
******************************************************************/

DemoApp::DemoApp() :
	m_hwnd(NULL)
{
}

/******************************************************************
*                                                                 *
*  Release resources.                                             *
*                                                                 *
******************************************************************/

DemoApp::~DemoApp()
{
	// TODO: Release app resource here.
}

/*******************************************************************
*                                                                  *
*  Create the application window and the combobox.                 *
*                                                                  *
*******************************************************************/

HRESULT DemoApp::Initialize()
{
	HRESULT hr;

	// Register the window class.
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = DemoApp::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG_PTR);
	wcex.hInstance = HINST_THISCOMPONENT;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);;
	wcex.lpszMenuName = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszClassName = TEXT("DemoApp");

	RegisterClassEx(&wcex);

	// Create the application window.
	//
	// Because the CreateWindow function takes its size in pixels, we
	// obtain the system DPI and use it to scale the window size.
	int dpiX = 0;
	int dpiY = 0;
	HDC hdc = GetDC(NULL);
	if (hdc)
	{
		dpiX = GetDeviceCaps(hdc, LOGPIXELSX);
		dpiY = GetDeviceCaps(hdc, LOGPIXELSY);
		ReleaseDC(NULL, hdc);
	}

	/*m_hwnd = CreateWindow(
	TEXT("DemoApp"),
	TEXT("Simple Combo Box Example"),
	WS_OVERLAPPEDWINDOW,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	static_cast<UINT>(ceil(640.f * dpiX / 96.f)),
	static_cast<UINT>(ceil(480.f * dpiY / 96.f)),
	NULL,
	NULL,
	HINST_THISCOMPONENT,
	this
	);*/
	int sx = ScreenX;
	int sy = ScreenY;
	m_hwnd = CreateWindow(
		TEXT("DemoApp"),
		TEXT("Simple Combo Box Example"),
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
		(ScreenX - MAPWIDTH) / 2,
		(ScreenY - MAPHEIGHT) / 2,
		MAPWIDTH,
		MAPHEIGHT,
		(HWND)NULL,
		(HMENU)NULL,
		(HINSTANCE)HINST_THISCOMPONENT,
		this);

	hr = m_hwnd ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
	{
		ShowWindow(m_hwnd, SW_SHOWNORMAL);
		UpdateWindow(m_hwnd);
	}


	// Create the Combobox
	//
	// Uses the CreateWindow function to create a child window of 
	// the application window. The WC_COMBOBOX window style specifies  
	// that it is a combobox.

	int xposCombo = 5 * MAPWIDTH / 8;            // Horizontal position of the window.
	int yposCombo = 1 * MAPHEIGHT / 8;            // Vertical position of the window.
	int nwidthCombo = 200;          // Width of the window
	int nheightCombo = 200;         // Height of the window
	HWND hwndParent = m_hwnd; // Handle to the parent window

	HWND hWndComboBox = CreateWindow(WC_COMBOBOX, TEXT(""),
		CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
		xposCombo, yposCombo, nwidthCombo, nheightCombo, hwndParent, NULL, HINST_THISCOMPONENT,
		NULL);



	// load the combobox with item list.  
	// Send a CB_ADDSTRING message to load each item

	TCHAR Planets[9][10] =
	{
		TEXT("Парабола"), TEXT("Гипербола"), TEXT("Синус"), TEXT("Тангенс")
	};

	TCHAR A[16];
	int  k = 0;

	memset(&A, 0, sizeof(A));
	for (k = 0; k <= 8; k += 1)
	{
		wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)Planets[k]);

		// Add string to combobox.
		SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)A);
	}

	// Send the CB_SETCURSEL message to display an initial item 
	//  in the selection field  
	SendMessage(hWndComboBox, CB_SETCURSEL, (WPARAM)ItemIndex, (LPARAM)0);

	// Create Button
	int xposButton = xposCombo + 200;            // Horizontal position of the window.
	int yposButton = 1 * MAPHEIGHT / 8;            // Vertical position of the window.
	int nwidthButton = 200;          // Width of the window
	int nheightButton = 22;         // Height of the window
	HWND hWndButton = CreateWindow(WC_BUTTON, TEXT("Plot"),
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		xposButton, yposButton, nwidthButton, nheightButton, hwndParent, NULL, HINST_THISCOMPONENT,
		NULL);

	return hr;
}


/******************************************************************
*                                                                 *
*  The main window's message loop.                                *
*                                                                 *
******************************************************************/

void DemoApp::RunMessageLoop()
{
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}


/******************************************************************
*                                                                 *
*  The window's message handler.                                  *
*                                                                 *
******************************************************************/

LRESULT CALLBACK DemoApp::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static RECT rect;
	HDC hdc; // создаем контекст устройства
	PAINTSTRUCT ps; // создаём экземпляр структуры графического вывода
	HBRUSH hBrush = NULL;
	LRESULT result = 0;

	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		DemoApp *pDemoApp = (DemoApp *)pcs->lpCreateParams;

		::SetWindowLongPtrW(
			hwnd,
			GWLP_USERDATA,
			PtrToUlong(pDemoApp)
			);

		result = 1;
	}
	else
	{
		DemoApp *pDemoApp = reinterpret_cast<DemoApp *>(static_cast<LONG_PTR>(
			::GetWindowLongPtrW(
				hwnd,
				GWLP_USERDATA
				)));

		bool wasHandled = false;

		if (pDemoApp)
		{
			switch (message)
			{
			case WM_SIZE:
				xView = LOWORD(lParam);
				yView = HIWORD(lParam);
				break;
			case WM_COMMAND:

				if (HIWORD(wParam) == CBN_SELCHANGE)
					// If the user makes a selection from the list:
					//   Send CB_GETCURSEL message to get the index of the selected list item.
					//   Send CB_GETLBTEXT message to get the item.
					//   Display the item in a messagebox.
				{
					ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL,
						(WPARAM)0, (LPARAM)0);
					TCHAR  ListItem[256];
					(TCHAR)SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT,
						(WPARAM)ItemIndex, (LPARAM)ListItem);
					//MessageBox(hwnd, (LPCWSTR)ListItem, TEXT("Item Selected"), MB_OK);
				}

				if (HIWORD(wParam) == BN_CLICKED)
				{
					RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
				}

				wasHandled = true;
				result = 0;
				break;

			case WM_PAINT:

				hdc = BeginPaint(hwnd, &ps);
				SetBkMode(hdc, TRANSPARENT);
				SetMapMode(hdc, MM_ISOTROPIC); // логические единицы отображаем, как физические
				SetWindowExtEx(hdc, 500, 500, NULL); // длина осей
				SetViewportExtEx(hdc, 500, -500, NULL); // определяем область вывода
				SetViewportOrgEx(hdc, 0.5 * xView, 0.6 * yView, NULL); // начало координат

				// Создание желтого прямоугольника
				hBrush = CreateSolidBrush(RGB(204, 255, 0));
				RECT localRect;
				SetRect(&localRect, -(xView/2-20), yView/2-100, xView/2-20, -(yView/2-100));
				FillRect(hdc, &localRect, hBrush);

				switch (ItemIndex)
				{
				case 0:
					PAR(hdc);
					break;
				case 1:
					GIP(hdc);
					break;
				case 2:
					SIN(hdc);
					break;
				case 3:
					TAN(hdc);
					break;
				default:
					break;
				}

				DeleteObject(hBrush);

				EndPaint(hwnd, &ps);

				/*InvalidateRect(hwnd, &rect, TRUE);
				hdc = BeginPaint(hwnd, &ps);
				SetBkMode(hdc, TRANSPARENT);
				SetMapMode(hdc, MM_ISOTROPIC); // логические единицы отображаем, как физические
				SetWindowExtEx(hdc, 500, 500, NULL); // длина осей
				SetViewportExtEx(hdc, 500, -500, NULL); // определяем область вывода
				SetViewportOrgEx(hdc, 0.5 * xView, 0.6 * yView, NULL); // начало координат

				// Создание желтого прямоугольника
				hBrush = CreateSolidBrush(RGB(204, 255, 0));
				RECT rect;
				SetRect(&rect, -120, 240, 800, -240);
				FillRect(hdc, &rect, hBrush);

				// Рисуем оси координат
				Line(hdc, 0, 220, 0, -220); // ось Y
				Line(hdc, -100, 0, 780, 0); // ось X
				MoveToEx(hdc, 0, 0, NULL); // перемещаемся в начало координат

				// Создание красного пера
				hPen = CreatePen(1, 4, RGB(255, 25, 0));
				SelectObject(hdc, hPen);

				// Синусоида
				for (i = 0; i < 450; i++)
				{
					y = 180.0 * (exp(-i * 0.01)) * sin(pi * i * (200.0 / 400.0) / 180.0);
					LineTo(hdc, i, (int)y);
				}

				// Делаем перо снова черным
				hPen = CreatePen(1, 1, RGB(0, 0, 0));
				SelectObject(hdc, hPen);

				// Наносим деления
				for (i = -100; i < 500; i += 100)
				{
					Line(hdc, i, 3, i, -3);
					Line(hdc, -3, i, 3, i);
					_stprintf(Buf, L"%d", i);
					TextOut(hdc, i - 5, -5, Buf, _ftcslen(Buf));
					TextOut(hdc, -5, i, Buf, _ftcslen(Buf));
				}



				EndPaint(hwnd, &ps);*/
				wasHandled = true;
				result = 0;

				break;
			case WM_DISPLAYCHANGE:
				InvalidateRect(hwnd, NULL, FALSE);
				wasHandled = true;
				result = 0;
				break;

			case WM_DESTROY:
				PostQuitMessage(0);
				wasHandled = true;
				result = 1;
				break;
			}
		}

		if (!wasHandled)
		{
			result = DefWindowProc(hwnd, message, wParam, lParam);
		}
	}

	return result;
}

BOOL Line(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, NULL); // сделать текущими координаты x1, y1
	return LineTo(hdc, x2, y2); // нарисовать линию
}

void PAR(HDC hdc)
{
	HPEN hPen = NULL;

	double scale = 10.0;
	POINT pt;
	GetViewportOrgEx(hdc, &pt);
	SetViewportOrgEx(hdc, 0.5 * xView, 0.9 * yView, NULL);
	MoveToEx(hdc, 0, 0, NULL);

	// Рисуем оси координат
	Line(hdc, 0, (yView / 2 - 120) * 2 - 20, 0, -20); // ось Y
	Line(hdc, -(xView / 2 - 40), 0, xView / 2 - 40, 0); // ось X
	MoveToEx(hdc, 0, 0, NULL); // перемещаемся в начало координат

							   // Создание красного пера
	hPen = CreatePen(1, 4, RGB(255, 25, 0));
	SelectObject(hdc, hPen);

	// Парабола
	for (i = 0; i < xView / 2 - 40; i++)
	{
		y = pow((double)i / scale, 2.0);
		if (y < (yView / 2 - 120) * 2 - 20)
			LineTo(hdc, i, (int)y);
	}
	MoveToEx(hdc, 0, 0, NULL);
	for (i = 0; i > -(xView / 2 - 40); i--)
	{
		y = pow((double)i / scale, 2.0);
		if (y < (yView / 2 - 120) * 2 - 20)
			LineTo(hdc, i, (int)y);
	}

	// Делаем перо снова черным
	hPen = CreatePen(1, 1, RGB(0, 0, 0));
	SelectObject(hdc, hPen);

	// Наносим деления
	MoveToEx(hdc, 0, 0, NULL);
	// по Y
	for (i = (yView / 2 - 120)*2 - 30; i > -10; i -= 50)
	{
		Line(hdc, -3, i, 3, i);
		_stprintf(Buf, L"%4.2f", (float)i);
		TextOut(hdc, -5, i, Buf, _ftcslen(Buf));
	}
	// по X
	for (i = -(xView / 2 - 40) / 90 * 90; i < (xView / 2 - 40) / 90 * 90; i += 90)
	{
		Line(hdc, i, 3, i, -3);
		_stprintf(Buf, L"%4.2f", (float)i / scale);
		TextOut(hdc, i - 5, -5, Buf, _ftcslen(Buf));
	}

	DeleteObject(hPen);

	SetViewportOrgEx(hdc, pt.x, pt.y, NULL);
}

void GIP(HDC hdc)
{
	HPEN hPen = NULL;

	double k = 10000.0;

	// Рисуем оси координат
	Line(hdc, 0, yView / 2 - 120, 0, -(yView / 2 - 120)); // ось Y
	Line(hdc, -(xView / 2 - 40), 0, xView / 2 - 40, 0); // ось X
	MoveToEx(hdc, 0, 0, NULL); // перемещаемся в начало координат

							   // Создание красного пера
	hPen = CreatePen(1, 4, RGB(255, 25, 0));
	SelectObject(hdc, hPen);

	bool flag = false;

	// Гипербола
	for (i = 1; i < xView / 2 - 40; i++)
	{
		y = k / i;
		if (y < (yView / 2 - 120))
		{
			if (!flag)
			{
				flag = true;
				MoveToEx(hdc, i, (int)y, NULL);
			}
			LineTo(hdc, i, (int)y);
		}
	}

	flag = false;
	for (i = -1; i > -(xView / 2 - 40); i--)
	{
		y = k / i;
		if (y > -(yView / 2 - 120))
		{
			if (!flag)
			{
				flag = true;
				MoveToEx(hdc, i, (int)y, NULL);
			}
			LineTo(hdc, i, (int)y);
		}
	}

	// Делаем перо снова черным
	hPen = CreatePen(1, 1, RGB(0, 0, 0));
	SelectObject(hdc, hPen);

	// Наносим деления
	MoveToEx(hdc, 0, 0, NULL);
	for (i = yView / 2 - 120; i > -(yView / 2 - 120); i -= 50)
	{
		Line(hdc, -3, i, 3, i);
		_stprintf(Buf, L"%4.2f", (float)i);
		TextOut(hdc, -5, i, Buf, _ftcslen(Buf));
	}
	for (i = -(xView / 2 - 40) / 90 * 90; i < (xView / 2 - 40) / 90 * 90; i += 90)
	{
		Line(hdc, i, 3, i, -3);
		_stprintf(Buf, L"%4.2f", (float)i);
		TextOut(hdc, i - 5, -5, Buf, _ftcslen(Buf));
	}

	DeleteObject(hPen);
}

void SIN(HDC hdc)
{
	HPEN hPen = NULL;

	double scale = 40.0;

	// Рисуем оси координат
	Line(hdc, 0, yView/2-120, 0, -(yView/2-120)); // ось Y
	Line(hdc, -(xView/2-40), 0, xView/2-40, 0); // ось X
	MoveToEx(hdc, 0, 0, NULL); // перемещаемся в начало координат

	// Создание красного пера
	hPen = CreatePen(1, 4, RGB(255, 25, 0));
	SelectObject(hdc, hPen);

	// Синусоида
	for (i = 0; i < xView/2-40; i++)
	{
		y = sin((double)i / scale) * (yView / 2 - 120);
		LineTo(hdc, i, (int)y);
	}
	MoveToEx(hdc, 0, 0, NULL);
	for (i = 0; i > -(xView/2-40); i--)
	{
		//y = 180.0 * (exp(-i * 0.01)) * sin(pi * i * (200.0 / 400.0) / 180.0);
		y = sin((double)i / scale) * (yView / 2 - 120);
		LineTo(hdc, i, (int)y);
	}

	// Делаем перо снова черным
	hPen = CreatePen(1, 1, RGB(0, 0, 0));
	SelectObject(hdc, hPen);

	// Наносим деления
	MoveToEx(hdc, 0, 0, NULL);
	for (i = yView / 2 - 120; i > -(yView / 2 - 120); i -= 50)
	{
		Line(hdc, -3, i, 3, i);
		_stprintf(Buf, L"%4.2f", (float)i/(yView / 2 - 120));
		TextOut(hdc, -5, i, Buf, _ftcslen(Buf));
	}
	for (i = -(xView / 2 - 40)/90*90; i < (xView / 2 - 40)/90*90; i += 90)
	{
		Line(hdc, i, 3, i, -3);
		_stprintf(Buf, L"%4.2f", (float)i/scale/pi*180);
		TextOut(hdc, i - 5, -5, Buf, _ftcslen(Buf));
	}

	DeleteObject(hPen);
}

void TAN(HDC hdc)
{

}
