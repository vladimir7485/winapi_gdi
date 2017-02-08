#include "globals.h"
#include "FACTORY.h"

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
	static FACTORY factory;
};