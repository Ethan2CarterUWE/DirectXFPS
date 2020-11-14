#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <DirectXPackedVector.h>
#include<string>
#include<sstream>
#include <d3dcompiler.h>


// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "D3DCompiler.lib")

// define the screen resolution
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

// global declarations
IDXGISwapChain* swapchain;             // the pointer to the swap chain interface
ID3D11Device* dev;                     // the pointer to our Direct3D device interface
ID3D11DeviceContext* devcon;           // the pointer to our Direct3D device context
ID3D11RenderTargetView* backbuffer;    // the pointer to our back buffer

ID3D11InputLayout* pLayout;            // the pointer to the input layout
ID3D11VertexShader* pVS;               // the pointer to the vertex shader
ID3D11PixelShader* pPS;                // the pointer to the pixel shader
ID3D11Buffer* pVBuffer;                // the pointer to the vertex buffer

// a struct to define a single vertex

//struct VERTEX { FLOAT X, Y, Z; DirectX::PackedVector::XMCOLOR Color; };
struct VERTEX
{
    struct
    {
        float x;
        float y;
    } pos;
    struct
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    } color;
};
void InitGraphics(void);    // creates the shape to render
void InitPipeline(void);    // loads and prepares the shaders

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
    InitPipeline();
    InitGraphics();
}

// this is the function used to render a single frame
void RenderFrame(void)
{
    // clear the back buffer to a deep blue
    float color[4] = { 0.0f, 0.3f, 0.4f, 1.0f };
    devcon->ClearRenderTargetView(backbuffer, color);
    // do 3D rendering on the back buffer here
        // select which vertex buffer to display
    UINT stride = sizeof(VERTEX);
    UINT offset = 0;
    devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

    // select which primtive type we are using
    devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // draw the vertex buffer to the back buffer
    devcon->Draw(9, 0);
    // switch the back buffer and the front buffer
    swapchain->Present(0, 0);
}

// this is the function that cleans up Direct3D and COM
void CleanD3D(void)
{
    swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode
    // close and release all existing COM objects
    pLayout->Release();
    pVS->Release();
    pPS->Release();
    pVBuffer->Release();
    swapchain->Release();
    backbuffer->Release();
    dev->Release();
    devcon->Release();
}
// this is the function that creates the shape to render
void InitGraphics()
{
    // create a triangle using the VERTEX struct
    VERTEX OurVertices[] =
    {
         /*{0.0f, 0.5f, 0.0f, DirectX::PackedVector::XMCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
       {0.45f, -0.5, 0.0f,  DirectX::PackedVector::XMCOLOR(0.0f, 1.0f, 0.0f, 1.0f)},
       {-0.45f, -0.5f, 0.0f, DirectX::PackedVector::XMCOLOR(0.0f, 0.0f, 1.0f, 1.0f)}*/


       /*{0.0f, 0.5f, 0.0f},
       {0.45f, -0.5, 0.0f},
       {-0.45f, -0.5f, 0.0f}*/

        { 0.0f,0.5f,255,0,0,0 },
		{ 0.5f,-0.5f,0,255,0,0 },
		{ -0.5f,-0.5f,0,0,255,0 },
      

          { 0.5f,1.0f,255,0,0,0 },
        { 1.0f,0.5f,0,255,0,0 },
        { 0.5f,0.5f,0,0,255,0 },

         { -1.0f,0.5f,255,0,0,0 },
        { 0.5f,-0.5f,0,255,0,0 },
        { -0.5f,0.5f,0,0,255,0 }

        /*        {0.0f, 0.5f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
        {0.45f, -0.5, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)},
        {-0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)}*/



    };
   // OurVertices[0].color.b = 255;
    //OurVertices[1].color.b = 255;



    // create the vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(VERTEX) * 9;             // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

    dev->CreateBuffer(&bd, NULL, &pVBuffer);       // create the buffer


    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, OurVertices, sizeof(OurVertices));                 // copy the data
    devcon->Unmap(pVBuffer, NULL);                                      // unmap the buffer
}


// this function loads and prepares the shaders
void InitPipeline()
{
    // load and compile the two shaders
   
        ID3D10Blob* VS, * PS;

        HRESULT result = D3DCompileFromFile(L"VertexShader.hlsl", NULL, NULL, "main", "vs_4_0", 0, 0, &VS, NULL);

        D3DCompileFromFile(L"PixelShader.hlsl", NULL, NULL, "main", "ps_4_0", 0, 0, &PS, NULL);

    // encapsulate both shaders into shader objects
    dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
    dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

    // set the shader objects
    devcon->VSSetShader(pVS, 0, 0);
    devcon->PSSetShader(pPS, 0, 0);

    // create the input layout object
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    dev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
    devcon->IASetInputLayout(pLayout);
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