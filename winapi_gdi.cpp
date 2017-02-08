/******************************************************************
*                                                                 *
*  Headers                                                        *
*                                                                 *
******************************************************************/
#include "winapi_gdi.h"
#include <CommCtrl.h>

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

/******************************************************************
*                                                                 *
*  DemoApp                                                        *
*                                                                 *
******************************************************************/

int xView, yView;

// Пусть ItemIndex соответствует 
// идентификаторам кривых CURVE_ID
int ItemIndex = 0;

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

FACTORY DemoApp::factory;

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
*  Create the application window.                                  *
*                                                                  *
*******************************************************************/

HRESULT DemoApp::Initialize()
{
	HRESULT hr;
	//CURVE_FACTORY curve_factory;

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
	wcex.lpszClassName = TEXT("Graphs");

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

	int sx = ScreenX;
	int sy = ScreenY;
	m_hwnd = CreateWindow(
		TEXT("Graphs"),
		TEXT("Графики"),
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

	int xposCombo = (int)(18.0 * MAPWIDTH / 32);            // Horizontal position of the window.
	int yposCombo = (int)(1.0 * MAPHEIGHT / 32);            // Vertical position of the window.
	int nwidthCombo = 200;          // Width of the window
	int nheightCombo = 200;         // Height of the window
	HWND hwndParent = m_hwnd; // Handle to the parent window

	HWND hWndComboBox = CreateWindow(WC_COMBOBOX, TEXT(""),
		CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
		xposCombo, yposCombo, nwidthCombo, nheightCombo, hwndParent, NULL, HINST_THISCOMPONENT,
		NULL);

	// load the combobox with item list.  
	// Send a CB_ADDSTRING message to load each item

	TCHAR charts[4][20] =
	{
		TEXT("Парабола"), TEXT("Гипербола (y=k/x)"), TEXT("Синус"), TEXT("Тангенс")
	};

	TCHAR A[20];
	int  k = 0;

	memset(&A, 0, sizeof(A));
	for (k = 0; k <= 3; k += 1)
	{
		wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)charts[k]);

		// Add string to combobox.
		SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)A);
	}

	// Send the CB_SETCURSEL message to display an initial item 
	//  in the selection field  
	SendMessage(hWndComboBox, CB_SETCURSEL, (WPARAM)ItemIndex, (LPARAM)0);



	// Create Button

	int xposButton = xposCombo + 210;            // Horizontal position of the window.
	int yposButton = yposCombo;            // Vertical position of the window.
	int nwidthButton = 200;          // Width of the window
	int nheightButton = 22;         // Height of the window
	HWND hWndButton = CreateWindow(WC_BUTTON, TEXT("Построить"),
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
	
	CURVE* curve;
	//FACTORY* factory;

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
					ItemIndex = (int)SendMessage((HWND)lParam, (UINT)CB_GETCURSEL,
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
				SetViewportOrgEx(hdc, (int)(0.5 * xView), (int)(0.5 * yView), NULL); // начало координат

				// Создание желтого прямоугольника
				hBrush = CreateSolidBrush(RGB(204, 255, 0));
				RECT localRect;
				SetRect(&localRect, -(xView/2-20), yView/2-100, xView/2-20, -(yView/2-100));
				FillRect(hdc, &localRect, hBrush);

				curve = factory.createCurve(CURVE_ID(ItemIndex));
				curve->plot(hdc, xView, yView);

				DeleteObject(hBrush);

				EndPaint(hwnd, &ps);

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
