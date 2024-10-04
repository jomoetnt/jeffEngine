

#include "framework.h"
#include "jeffEngine.h"
#include "testGame.h"

constexpr int MAX_LOADSTRING = 100;

constexpr int WINDOW_WIDTH = 1920;
constexpr int WINDOW_HEIGHT = 1080;

constexpr int FRAMETIME = 1000 / 144;
constexpr int PHYSICS_FRAMETIME = 1000 / 60;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

using namespace jeffNamespace;

std::thread jThread;
std::thread jThread2;
int quit = 0;

float refreshTimer = 0.0f;

// temporary test
testGame* game;

static void timer_start()
{
    jThread = std::thread([]()
    {
        float delta = 0.0f;
        while (quit == 0)
        {
            auto y = std::chrono::steady_clock::now();
            quit = jeffManager::getInstance()->doFrame();
            //std::this_thread::sleep_for(std::chrono::milliseconds(FRAMETIME));
            auto z = std::chrono::steady_clock::now();
            auto w = z - y;
            delta = std::chrono::duration_cast<std::chrono::microseconds>(w).count() / 1000000.0f;
            refreshTimer += delta;
            if (refreshTimer > 0.1f)
            {
                jGraphics::getInstance()->delta = delta;
                refreshTimer = 0.0f;
            }
        }
    });
    jThread.detach();
    jThread2 = std::thread([]()
    {
        float delta2 = 0.0f;
        while (quit == 0)
        {
            auto y2 = std::chrono::steady_clock::now();
            jeffManager::getInstance()->doPhysicsTick(delta2);
            std::this_thread::sleep_for(std::chrono::milliseconds(PHYSICS_FRAMETIME));
            auto z2 = std::chrono::steady_clock::now();
            auto w2 = z2 - y2;
            delta2 = std::chrono::duration_cast<std::chrono::microseconds>(w2).count() / 1000000.0f;
        }
    });
    jThread2.detach();
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_JEFFENGINE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_JEFFENGINE));

    MSG msg{};
    while (quit == 0)
    {
        // Main message loop:
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

    return (int) msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex{};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_JEFFENGINE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, 0, WINDOW_WIDTH, WINDOW_HEIGHT, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    jGraphics::makeInstance(hWnd);
    jeffAudio::makeInstance();
    jeffManager::makeInstance();
    game = new testGame();

    timer_start();

    return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return jeffManager::getInstance()->handleInputEvent(hWnd, message, wParam, lParam);
}