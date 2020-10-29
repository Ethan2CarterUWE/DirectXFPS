#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include<string>
#include<sstream>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// entry point for the program
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    //const wchar_t CLASS_NAME[]  = L"Sample Window Class";
    
    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = L"WindowClass1";

    // register the window class
    RegisterClass(&wc);

    // Create the window and the handle.
    HWND hwnd = CreateWindowEx(
        0,                       //window styles.                       
        L"WindowClass1",         // window class
        L"THis is my WINDOW",    // window text
        WS_OVERLAPPEDWINDOW,     // window style

        //  position(2) and size(2) 
        CW_USEDEFAULT, CW_USEDEFAULT, 1440, 900,

        NULL,       // no parent window    
        NULL,       // no menus
        hInstance,  // instance handle
        NULL        // no  additional windows
        );

    if (hwnd == NULL)
    {
        return 0;
    }

    //display window on screen
    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    /*case WM_KEYDOWN:
        if (wParam == 'C')
        {
            // have to include an L prefix
            SetWindowText(hwnd, L"test");
        }
        break;*/


        //Quits the game on Escape
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
        {
            
            PostQuitMessage(0);
            return 0;
        }
        break;

    /*case WM_LBUTTONDOWN:
        {
        const POINTS pt = MAKEPOINTS(lParam);
        std::ostringstream oss;
        oss << "(" << pt.x << "," << pt.y << ")";
        //SetWindowText(hwnd, L,oss.str().c_str());
        }
        break;*/
        
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);



            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

            EndPaint(hwnd, &ps);
        }
        return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}