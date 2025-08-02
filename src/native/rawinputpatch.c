#include <stdio.h>
#include <stdbool.h>
#include <windows.h>

#include "legacyfixnative.h"

HWND g_hWnd = NULL;

static WNDPROC s_originalWndProc = NULL;

static int s_nDeltaX = 0;
static int s_nDeltaY = 0;

LRESULT CALLBACK WndProc_Hook( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if ( msg == WM_INPUT )
	{
		UINT unSize = 0;
		GetRawInputData( ( HRAWINPUT )lParam, RID_INPUT, NULL, &unSize, sizeof( RAWINPUTHEADER ) );

		BYTE *pData = ( BYTE * )_alloca( unSize );
		if ( GetRawInputData( ( HRAWINPUT )lParam, RID_INPUT, pData, &unSize, sizeof( RAWINPUTHEADER ) ) == unSize )
		{
			RAWINPUT *pRaw = ( RAWINPUT * )pData;
			if ( pRaw->header.dwType == RIM_TYPEMOUSE )
			{
				s_nDeltaX += pRaw->data.mouse.lLastX;
				s_nDeltaY += pRaw->data.mouse.lLastY;
			}
		}
	}

	return CallWindowProc( s_originalWndProc, hWnd, msg, wParam, lParam );
}

DLLEXPORT void InitRawInputPatch( void *hWnd )
{
	g_hWnd = ( HWND )hWnd;

	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01;
	rid.usUsage = 0x02;
	rid.dwFlags = RIDEV_INPUTSINK;
	rid.hwndTarget = g_hWnd;

	if ( RegisterRawInputDevices( &rid, 1, sizeof( rid ) ) )
	{
		s_originalWndProc = ( WNDPROC )( LONG_PTR )GetWindowLongPtr( g_hWnd, GWLP_WNDPROC );
		SetWindowLongPtr( g_hWnd, GWLP_WNDPROC, ( LONG_PTR )WndProc_Hook );
	}
}

DLLEXPORT void GetDelta( int *out_pnDeltaX, int *out_pnDeltaY )
{
	*out_pnDeltaX = s_nDeltaX;
	*out_pnDeltaY = s_nDeltaY;

	s_nDeltaX = s_nDeltaY = 0;
}

DLLEXPORT bool BIsRawInputAvailable( void )
{
	return s_originalWndProc != NULL;
}