// 识别是否启用手势
class GestureMgr
{
public:
    void Init(GestureWindow *wnd)
    {
        gesture_window = wnd;
        running_ = false;
        recognition_ = false;
        ignore_mouse_event = false;
    }

    //鼠标右键按下
    bool OnRButtonDown(PMOUSEHOOKSTRUCT pmouse)
    {
        if(!ignore_mouse_event)
        {
            running_ = true;
            init = pmouse->pt;
            gesture_recognition.init(pmouse->pt.x, pmouse->pt.y);
            gesture_window->SendMessageW(WM_USER_HWND, (WPARAM)pmouse->hwnd);
            return true;
        }

        return false;
    }

    //鼠标右键弹起
    bool OnRButtonUp(PMOUSEHOOKSTRUCT pmouse, bool ignore = false)
    {
        if(!ignore_mouse_event && running_)
        {
            if(recognition_)
            {
                std::wstring command = gesture_recognition.result();
                gesture_window->SendMessageW(WM_USER_END, (WPARAM)command.c_str());
            }
            else
            {
                if (!ignore)
                {
                    ignore_mouse_event = true;
                    
                    SendOneMouse(MOUSEEVENTF_RIGHTDOWN);
                    SendOneMouse(MOUSEEVENTF_RIGHTUP);
                }
            }
            running_ = false;
            recognition_ = false;
            return true;
        }

        ignore_mouse_event = false;
        return false;
    }

    // 鼠标移动
    bool OnMouseMove(PMOUSEHOOKSTRUCT pmouse)
    {
        if(running_)
        {
            if(recognition_)
            {
                if(IsPressed(VK_RBUTTON))
                {
                    gesture_recognition.move(pmouse->pt.x, pmouse->pt.y);
                    gesture_window->SendMessageW(WM_USER_UPDATE);
                }
                else
                {
                    gesture_window->SendMessageW(WM_USER_END, (WPARAM)L"");
                    running_ = false;
                    recognition_ = false;
                }
            }
            else
            {
                // 尚未开始识别
                if(GestureRecognition::GetDistance(init, pmouse->pt)>10)
                {
                    gesture_window->SendMessageW(WM_USER_SHOW);
                    recognition_ = true;
                }
            }
        }
        return false;
    }
private:
    bool running_;
    bool recognition_;
    bool ignore_mouse_event;
    POINT init;
    GestureWindow *gesture_window;
} gesture_mgr;

void StartGestureThread()
{
    // 初始化gdiplus
    GdiplusStartupInput in;
    ULONG_PTR gdiplus_token;
    GdiplusStartup(&gdiplus_token, &in, NULL);

    // 初始化手势
    GestureWindow wnd;
    wnd.Create(NULL, CRect(0, 0, 0, 0), L"GreenChromeGesture", WS_POPUP, WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE);
    gesture_mgr.Init(&wnd);

    // 消息循环
    WTL::CMessageLoop msgLoop;
    int ret = msgLoop.Run();

    GdiplusShutdown(gdiplus_token);
}
