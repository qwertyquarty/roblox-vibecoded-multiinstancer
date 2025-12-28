#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>

#define WM_T (WM_USER+1)

NOTIFYICONDATAA n;

LRESULT CALLBACK W(HWND h, UINT m, WPARAM w, LPARAM l) {
    if (m == WM_T && (l == WM_LBUTTONUP || l == WM_RBUTTONUP)) PostQuitMessage(0);
    if (m == WM_DESTROY) { Shell_NotifyIconA(NIM_DELETE, &n); PostQuitMessage(0); }
    return DefWindowProcA(h,m,w,l);
}

int WINAPI WinMain(HINSTANCE i, HINSTANCE, LPSTR, int) {
    HANDLE mx = CreateMutexA(0,1,"ROBLOX_singletonEvent");
    if (GetLastError() == ERROR_ALREADY_EXISTS) return 0;

    WNDCLASSA c = {0};
    c.lpfnWndProc = W;
    c.hInstance = i;
    c.lpszClassName = "t";
    RegisterClassA(&c);

    HWND h = CreateWindowA("t","",0,0,0,0,0,HWND_MESSAGE,0,i,0);

    n.cbSize = sizeof(n);
    n.hWnd = h;
    n.uID = 1;
    n.uFlags = NIF_MESSAGE|NIF_ICON;
    n.uCallbackMessage = WM_T;
    n.hIcon = LoadIconA(0,IDI_APPLICATION);
    Shell_NotifyIconA(NIM_ADD,&n);

    MSG msg;
    while (GetMessageA(&msg,0,0,0)) DispatchMessageA(&msg);

    CloseHandle(mx);
    return 0;
}

