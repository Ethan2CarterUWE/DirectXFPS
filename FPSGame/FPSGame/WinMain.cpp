#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

#include<string>
#include<sstream>


// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")

// define the screen resolution
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

// global declarations
IDXGISwapChain* swapchain;             // the pointer to the swap chain interface
ID3D11Device* dev;                     // the pointer to our Direct3D device interface
ID3D11DeviceContext* devcon;           // the pointer to our Direct3D device context
ID3D11RenderTargetView* backbuffer;    // the pointer to our back buffer


// function prototypes
void InitD3D(HWND hwnd);    // sets up and initializes Direct3D
void RenderFrame(void);     // renders a single frame
void CleanD3D(void);        // closes Direct3D and releases memory

// function prototypes
//void initD3D(HWND hwnd);    // sets up and initializes Direct3D
//void render_frame(void);    // renders a single frame
//void cleanD3D(void);    // closes Direct3D and releases memory


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
        0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,

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

    InitD3D(hwnd);
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
        RenderFrame();
    }
       
    
      

    
    CleanD3D();
    return 0;

  
}

// this function initializes and prepares Direct3D for use
void InitD3D(HWND hWnd)
{
    // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd;

    // clear out the struct for use
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    // fill the swap chain description struct
    scd.BufferCount = 1;                                    // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
    scd.BufferDesc.Width = SCREEN_WIDTH;                    // set the back buffer width
    scd.BufferDesc.Height = SCREEN_HEIGHT;                  // set the back buffer height
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
    scd.OutputWindow = hWnd;                                // the window to be used
    scd.SampleDesc.Count = 4;                               // how many multisamples
    scd.Windowed = TRUE;                                    // windowed/full-screen mode
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     // allow full-screen switching

    // create a device, device context and swap chain using the information in the scd struct
    D3D11CreateDeviceAndSwapChain(NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        NULL,
        NULL,
        NULL,
        D3D11_SDK_VERSION,
        &scd,
        &swapchain,
        &dev,
        NULL,
        &devcon);

    // get the address of the back buffer
    ID3D11Texture2D* pBackBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    // use the back buffer address to create the render target
    dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
    pBackBuffer->Release();

    // set the render target as the back buffer
    devcon->OMSetRenderTargets(1, &backbuffer, NULL);


    // Set the viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = SCREEN_WIDTH;
    viewport.Height = SCREEN_HEIGHT;

    devcon->RSSetViewports(1, &viewport);
}

// this is the function used to render a single frame
void RenderFrame(void)
{
    // clear the back buffer to a deep blue
    float color[4] = { 0.0f, 0.3f, 0.4f, 1.0f };
    devcon->ClearRenderTargetView(backbuffer, color);
    // do 3D rendering on the back buffer here

    // switch the back buffer and the front buffer
    swapchain->Present(0, 0);
}

// this is the function that cleans up Direct3D and COM
void CleanD3D(void)
{
    swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode
    // close and release all existing COM objects
    swapchain->Release();
    backbuffer->Release();
    dev->Release();
    devcon->Release();
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