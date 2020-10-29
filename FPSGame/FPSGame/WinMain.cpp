#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>


#include<string>
#include<sstream>




// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")

// global declarations
LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class

// function prototypes
void initD3D(HWND hwnd);    // sets up and initializes Direct3D
void render_frame(void);    // renders a single frame
void cleanD3D(void);    // closes Direct3D and releases memory


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

    initD3D(hwnd);

    // Run the message loop.

    MSG msg;
    //infinite loop
    while (TRUE)
    {

        //while (GetMessage(&msg, NULL, 0, 0))
        //no longer waiting for message instead peak into it
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))

        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

        }

        if (msg.message == WM_QUIT)
            break;

        render_frame();
    }
       
    
      

    
    cleanD3D();
    return 0;

  
}

void initD3D(HWND hwnd)
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION);    // create the Direct3D interface

    D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information

    ZeroMemory(&d3dpp, sizeof(d3dpp));    // clear out the struct for use
    d3dpp.Windowed = TRUE;    // program windowed, not fullscreen
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
    d3dpp.hDeviceWindow = hwnd;    // set the window to be used by Direct3D

    // create a device class using this information and information from the d3dpp stuct
    d3d->CreateDevice(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hwnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp,
        &d3ddev);
}

// this is the function used to render a single frame
void render_frame(void)
{
    // clear the window to a deep blue
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);

    d3ddev->BeginScene();    // begins the 3D scene

    // do 3D rendering on the back buffer here

    d3ddev->EndScene();    // ends the 3D scene

    d3ddev->Present(NULL, NULL, NULL, NULL);    // displays the created frame
}

// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
    d3ddev->Release();    // close and release the 3D device
    d3d->Release();    // close and release Direct3D
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