#include "Parsekeys.h"

static bool is_hide = false;

static std::vector <HWND> hwnd_list;

BOOL CALLBACK SearchChromeWindow(HWND hWnd, LPARAM lParam)
{
    //隐藏
    if(IsWindowVisible(hWnd))
    {
        wchar_t buff[256];
        GetClassNameW(hWnd, buff, 255);
        if ( wcscmp(buff, L"Chrome_WidgetWin_1")==0 )// || wcscmp(buff, L"Chrome_WidgetWin_2")==0 || wcscmp(buff, L"SysShadow")==0 )
        {
            ShowWindow(hWnd, SW_HIDE);
            hwnd_list.push_back(hWnd);
        }
    }
    return true;
}

void OnBosskey()
{
    if(!is_hide)
    {
        EnumWindows(SearchChromeWindow, 0);
    }
    else
    {
        for (auto r_iter = hwnd_list.rbegin(); r_iter != hwnd_list.rend() ; r_iter++)
        {
            ShowWindow(*r_iter, SW_SHOW);
        }
        hwnd_list.clear();
    }
    is_hide = !is_hide;
}

void HotKeyRegister(LPARAM lParam)
{
    RegisterHotKey(NULL, 0, LOWORD(lParam), HIWORD(lParam));

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (msg.message == WM_HOTKEY)
        {
            OnBosskey();
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

bool Bosskey(const wchar_t *iniPath)
{
    wchar_t keys[256];
    GetPrivateProfileStringW(L"基本设置", L"老板键", L"", keys, 256, iniPath);
    if(keys[0])
    {
        UINT flag = ParseHotkeys(keys);

        std::thread th(HotKeyRegister, flag);
        th.detach();
    }
    return keys[0]!='\0';
}
