// 发送按键
class SendKeys
{
public:
    template<typename ... T>
    SendKeys(T ... keys)
    {
        std::vector <int> keys_ = { keys ... };
        for (auto & key : keys_ )
        {
            INPUT input = { 0 };
            input.type = INPUT_KEYBOARD;
            input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY;
            input.ki.wVk = key;

            // 修正鼠标消息
            switch (key)
            {
            case VK_MBUTTON:
                input.type = INPUT_MOUSE;
                input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
                break;
            }

            inputs_.push_back(input);
        }

        SendInput((UINT)inputs_.size(), &inputs_[0], sizeof(INPUT));
    }
    ~SendKeys()
    {
        for (auto & input : inputs_)
        {
            input.ki.dwFlags |= KEYEVENTF_KEYUP;

            // 修正鼠标消息
            switch (input.ki.wVk)
            {
            case VK_MBUTTON:
                input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
                break;
            }
        }

        SendInput((UINT)inputs_.size(), &inputs_[0], sizeof(INPUT));
    }
private:
    std::vector <INPUT> inputs_;
};

/*
chrome ui tree


browser/ui/views/frame/BrowserRootView
ui/views/window/NonClientView
BrowserView
    TopContainerView
        TabStrip
            Tab
                TabCloseButton
            ImageButton
        ToolbarView
            LocationBarView
                OmniboxViewViews
    BookmarkBarView
        BookmarkButton
*/

template<typename Function>
void GetAccessibleName(IAccessible *node, Function f)
{
    VARIANT self;
    self.vt = VT_I4;
    self.lVal = CHILDID_SELF;

    BSTR bstr = NULL;
    if( S_OK == node->get_accName(self, &bstr) )
    {
        f(bstr);
        SysFreeString(bstr);
    }
}

template<typename Function>
void GetAccessibleValue(IAccessible *node, Function f)
{
    VARIANT self;
    self.vt = VT_I4;
    self.lVal = CHILDID_SELF;

    BSTR bstr = NULL;
    if( S_OK == node->get_accValue(self, &bstr) )
    {
        f(bstr);
        SysFreeString(bstr);
    }
}

template<typename Function>
void GetAccessibleSize(IAccessible *node, Function f)
{
    VARIANT self;
    self.vt = VT_I4;
    self.lVal = CHILDID_SELF;

    RECT rect;
    if( S_OK == node->accLocation(&rect.left, &rect.top, &rect.right, &rect.bottom, self) )
    {
        rect.right += rect.left;
        rect.bottom += rect.top;
        f(rect);
    }
}

long GetAccessibleState(IAccessible *node)
{
    VARIANT self;
    self.vt = VT_I4;
    self.lVal = CHILDID_SELF;

    VARIANT state;
    if( S_OK == node->get_accState(self, &state) )
    {
        if (state.vt == VT_I4)
        {
            return state.lVal;
        }
    }
    return 0;
}

long GetAccessibleRole(IAccessible *node)
{
    VARIANT self;
    self.vt = VT_I4;
    self.lVal = CHILDID_SELF;

    VARIANT role;
    if( S_OK == node->get_accRole(self, &role) )
    {
        if (role.vt == VT_I4)
        {
            return role.lVal;
        }
    }
    return 0;
}

template<typename Function>
void TraversalAccessible(IAccessible *node, Function f)
{
    long childCount = 0;
    if( node && S_OK == node->get_accChildCount(&childCount) && childCount )
    {
        VARIANT* varChildren = (VARIANT*)malloc(sizeof(VARIANT) * childCount);
        if( S_OK == AccessibleChildren(node, 0, childCount, varChildren, &childCount) )
        {
            for(int i=0;i<childCount;i++)
            {
                if( varChildren[i].vt==VT_DISPATCH )
                {
                    IDispatch* dispatch = varChildren[i].pdispVal;
                    IAccessible* child = NULL;
                    if( S_OK == dispatch->QueryInterface(IID_IAccessible, (void**)&child))
                    {
                        if( (GetAccessibleState(child) & STATE_SYSTEM_INVISIBLE) == 0) // 只遍历可见节点
                        {
                            if(f(child))
                            {
                                dispatch->Release();
                                break;
                            }
                            child->Release();
                        }
                    }
                    dispatch->Release();
                }
            }
        }
        free(varChildren);
    }
}

long GetButtonCount(IAccessible *node, int &count)
{
    count = 0;
    bool flag = false;//标记是否有子节点
    TraversalAccessible(node, [&flag, &count]
    (IAccessible* child) {
        if (GetAccessibleRole(child) == ROLE_SYSTEM_PUSHBUTTON)
        {
            count++;
        }
        flag = true;
        return false;
    });
    return flag;
}

IAccessible* GetChildElement(IAccessible *parent, bool aoto_release, int index)
{
    IAccessible* element = NULL;
    if(parent)
    {
        int i = 1;
        TraversalAccessible(parent, [&element, &i, &index]
            (IAccessible* child){
                if(i==index)
                {
                    element = child;
                }
                i++;
                return element!=NULL;
            });
        if(aoto_release) parent->Release();
    }
    return element;
}

bool CheckAccessibleRole(IAccessible* node, long role)
{
    if(node)
    {
        if(GetAccessibleRole(node)==role) return true;
        node->Release();
    }

    return false;
}

IAccessible* FindChildElement(IAccessible *parent, long role)
{
    IAccessible* element = NULL;
    if (parent)
    {
        TraversalAccessible(parent, [&element, &role]
        (IAccessible* child) {
            if (GetAccessibleRole(child) == role)
            {
                element = child;
            }
            return element != NULL;
        });
    }
    return element;
}

IAccessible* FindTopContainerView(IAccessible *top)
{
    top = GetChildElement(top, true, 3);
    if(!CheckAccessibleRole(top, ROLE_SYSTEM_WINDOW)) return NULL;

    top = GetChildElement(top, true, 1);
    if(!CheckAccessibleRole(top, ROLE_SYSTEM_CLIENT)) return NULL;

    top = GetChildElement(top, true, 2);
    if(!CheckAccessibleRole(top, ROLE_SYSTEM_CLIENT)) return NULL;

    IAccessible *BookMark = GetChildElement(top, false, 2);
    if(BookMark)
    {
        if(GetAccessibleRole(BookMark)==ROLE_SYSTEM_TOOLBAR)
        {
            BookMark->Release();
            top = GetChildElement(top, true, 3);
        }
        else
        {
            top->Release();
            top = BookMark;
        }

        if(!CheckAccessibleRole(top, ROLE_SYSTEM_CLIENT)) return NULL;
    }
    else
    {
        top->Release();
        return NULL;
    }

    return top;
}

IAccessible* GetTopContainerView(HWND hwnd)
{
    IAccessible *TopContainerView = NULL;
    wchar_t name[MAX_PATH];
    if(GetClassName(hwnd, name, MAX_PATH) && wcscmp(name, L"Chrome_WidgetWin_1")==0)
    {
        IAccessible *paccMainWindow = NULL;
        if ( S_OK == AccessibleObjectFromWindow(hwnd, OBJID_WINDOW, IID_IAccessible, (void**)&paccMainWindow) )
        {
            TopContainerView = FindTopContainerView(paccMainWindow);
            if (!TopContainerView)
            {
                DebugLog(L"FindTopContainerView failed");
            }
        }
    }
    return TopContainerView;
}

// 鼠标是否在标签栏上
bool IsOnTheTab(IAccessible* top, POINT pt)
{
    bool flag = false;
    IAccessible *TabStrip = FindChildElement(top, ROLE_SYSTEM_PAGETABLIST);
    if(TabStrip)
    {
        GetAccessibleSize(TabStrip, [&flag, &pt]
            (RECT rect){
                if(PtInRect(&rect, pt))
                {
                    flag = true;
                }
        });
        TabStrip->Release();
    }
    else
    {
        if (top) DebugLog(L"IsOnTheTab failed");
    }
    return flag;
}

// 鼠标是否在某个标签上
bool IsOnOneTab(IAccessible* top, POINT pt)
{
    bool flag = false;
    IAccessible *TabStrip = FindChildElement(top, ROLE_SYSTEM_PAGETABLIST);
    if(TabStrip)
    {
        TraversalAccessible(TabStrip, [&flag, &pt]
            (IAccessible* child){
                if(GetAccessibleRole(child)==ROLE_SYSTEM_PAGETAB)
                {
                    GetAccessibleSize(child, [&flag, &pt]
                        (RECT rect){
                            if(PtInRect(&rect, pt))
                            {
                                flag = true;
                            }
                        });
                }
                if(flag) child->Release();
                return flag;
            });
        TabStrip->Release();
    }
    else
    {
        if(top) DebugLog(L"IsOnOneTab failed");
    }
    return flag;
}

// 是否悬停在关闭按钮上
bool IsOnOneInactiveTabCloseButton(IAccessible *node, POINT pt)
{
    bool flag = false;
    TraversalAccessible(node, [&pt, &flag]
    (IAccessible* child) {
        if (GetAccessibleRole(child) == ROLE_SYSTEM_PUSHBUTTON)
        {
            GetAccessibleSize(child, [&flag, &pt]
            (RECT rect) {
                //调整大小，更精确匹配
                rect.left += 4;
                rect.top += 7;
                rect.right -= 16;
                rect.bottom -= 6;
                if (PtInRect(&rect, pt))
                {
                    flag = true;
                }
            });
        }
        return false;
    });
    return flag;
}

// 获取到在第几个tab上
int GetTabIndex(IAccessible *node, POINT pt)
{
    std::vector <RECT> tab_rects;
    TraversalAccessible(node, [&]
    (IAccessible* child) {
        if (GetAccessibleRole(child) == ROLE_SYSTEM_PAGETAB)
        {
            GetAccessibleSize(child, [&]
            (RECT rect) {
                tab_rects.push_back(rect);
            });
        }
        return false;
    });
    std::sort(tab_rects.begin(), tab_rects.end(), [](auto &a, auto &b) {
        return a.left < b.left;
    });

    int index = 0;
    for (auto rect : tab_rects)
    {
        index++;
        if (PtInRect(&rect, pt))
        {
            break;
        }
    }

    if (index >= 9)
    {
        if (index == tab_rects.size())
        {
            index = 9;
        }
        else
        {
            index = 0;
        }

    }
    return index;
}

// 鼠标是否在某个未激活标签上
bool IsOnOneInactiveTab(IAccessible* top, POINT pt, int &index, bool &onclose)
{
    bool flag = false;
    index = 0;
    onclose = false;
    IAccessible *TabStrip = FindChildElement(top, ROLE_SYSTEM_PAGETABLIST);
    if (TabStrip)
    {
        TraversalAccessible(TabStrip, [&]
        (IAccessible* child) {
            if (GetAccessibleRole(child) == ROLE_SYSTEM_PAGETAB)
            {
                if (GetAccessibleState(child) & STATE_SYSTEM_SELECTED)
                {
                    // 跳过已经选中标签
                    return false;
                }
                GetAccessibleSize(child, [&]
                (RECT rect) {
                    if (PtInRect(&rect, pt))
                    {
                        flag = true;
                        onclose = IsOnOneInactiveTabCloseButton(child, pt);
                        index = GetTabIndex(TabStrip, pt);
                    }
                });
            }
            if (flag) child->Release();
            return flag;
        });
        TabStrip->Release();
    }
    else
    {
        if (top) DebugLog(L"IsOnOneTab failed");
    }
    return flag;
}

// 是否只有一个标签
bool IsOnlyOneTab(IAccessible* top)
{
    IAccessible *TabStrip = FindChildElement(top, ROLE_SYSTEM_PAGETABLIST);
    if(TabStrip)
    {
        long tab_count = 0;
        bool closing = false;
        TraversalAccessible(TabStrip, [&tab_count, &closing]
        (IAccessible* child) {
            if (GetAccessibleRole(child) == ROLE_SYSTEM_PAGETAB)
            {
                if (closing)
                {
                    //已经找到一个正在关闭的标签了，其它的都算正常标签
                    tab_count++;
                }
                else
                {
                    //如果有关闭按钮才算标签
                    int count = 0;
                    if (GetButtonCount(child, count))
                    {
                        if (count == 0)
                            closing = true;
                        else
                            tab_count++;
                    }
                    else
                    {
                        //没有子节点，比如固定标签页
                        tab_count++;
                    }
                }
                }
            return false;
        });
        TabStrip->Release();
        return tab_count<=1;
    }
    else
    {
        if (top) DebugLog(L"IsOnlyOneTab failed");
    }
    return false;
}


bool IsOnOneBookmarkInner(IAccessible* parent, POINT pt)
{
    bool flag = false;

    // 寻找书签栏
    IAccessible *BookmarkBarView = NULL;
    if (parent)
    {
        TraversalAccessible(parent, [&BookmarkBarView]
        (IAccessible* child) {
            if (GetAccessibleRole(child) == ROLE_SYSTEM_TOOLBAR)
            {
                IAccessible *group = FindChildElement(child, ROLE_SYSTEM_GROUPING);
                if (group==NULL)
                {
                    BookmarkBarView = child;
                    return true;
                }
                group->Release();
            }
            return false;
        });
    }

    if(BookmarkBarView)
    {
        TraversalAccessible(BookmarkBarView, [&flag, &pt]
            (IAccessible* child){
                if(GetAccessibleRole(child)==ROLE_SYSTEM_PUSHBUTTON)
                {
                    GetAccessibleSize(child, [&flag, &pt]
                        (RECT rect){
                            if(PtInRect(&rect, pt))
                            {
                                flag = true;
                            }
                        });
                }
                if(flag) child->Release();
                return flag;
            });
        BookmarkBarView->Release();
    }
    return flag;
}

// 是否点击书签栏
bool IsOnOneBookmark(IAccessible* top, POINT pt)
{
    bool flag = false;
    if(top)
    {
        // 开启了书签栏长显
        if(IsOnOneBookmarkInner(top, pt)) return true;

        // 未开启书签栏长显
        IDispatch* dispatch = NULL;
        if( S_OK == top->get_accParent(&dispatch) && dispatch)
        {
            IAccessible* parent = NULL;
            if( S_OK == dispatch->QueryInterface(IID_IAccessible, (void**)&parent))
            {
                flag = IsOnOneBookmarkInner(parent, pt);
                parent->Release();
            }
            dispatch->Release();
        }
    }
    return flag;
}

// 当前激活标签是否是新标签
bool IsBlankTab(IAccessible* top)
{
    bool flag = false;
    IAccessible *TabStrip = FindChildElement(top, ROLE_SYSTEM_PAGETABLIST);
    if(TabStrip)
    {
        wchar_t* new_tab_title = NULL;
        TraversalAccessible(TabStrip, [&new_tab_title]
            (IAccessible* child){
                if(GetAccessibleRole(child)==ROLE_SYSTEM_PUSHBUTTON)
                {
                    GetAccessibleName(child, [&new_tab_title]
                        (BSTR bstr){
                            new_tab_title = _wcsdup(bstr);
                    });
                }
                return false;
            });
        if(new_tab_title)
        {
            TraversalAccessible(TabStrip, [&flag, &new_tab_title]
                (IAccessible* child){
                    if(GetAccessibleState(child) & STATE_SYSTEM_SELECTED)
                    {
                        GetAccessibleName(child, [&flag, &new_tab_title]
                            (BSTR bstr){
                                if(wcscmp(bstr, new_tab_title)==0)
                                {
                                    flag = true;
                                }
                        });
                    }
                    return false;
                });
            free(new_tab_title);
        }
        TabStrip->Release();
    }
    return flag;
}

// 是否焦点在地址栏
bool IsOmniboxViewFocus(IAccessible* top)
{
    bool flag = false;

    // 寻找地址栏
    IAccessible *LocationBarView = NULL;
    if (top)
    {
        TraversalAccessible(top, [&LocationBarView]
        (IAccessible* child) {
            if (GetAccessibleRole(child) == ROLE_SYSTEM_TOOLBAR)
            {
                IAccessible *group = FindChildElement(child, ROLE_SYSTEM_GROUPING);
                if (group)
                {
                    LocationBarView = group;
                    child->Release();
                    return true;
                }
            }
            return false;
        });
    }

    if (LocationBarView)
    {
        IAccessible *OmniboxViewViews = FindChildElement(LocationBarView, ROLE_SYSTEM_TEXT);
        if(OmniboxViewViews)
        {
            GetAccessibleValue(OmniboxViewViews, [&OmniboxViewViews, &flag]
                (BSTR bstr){
                    if(bstr[0]!=0) // 地址栏为空直接跳过
                    {
                        if( (GetAccessibleState(OmniboxViewViews) & STATE_SYSTEM_FOCUSED) == STATE_SYSTEM_FOCUSED)
                        {
                            flag = true;
                        }
                    }
            });
            OmniboxViewViews->Release();
        }
        LocationBarView->Release();
    }
    else
    {
        if (top) DebugLog(L"IsOmniboxViewFocus failed");
    }
    return flag;
}

std::map <HWND, bool> tracking_hwnd;
bool ignore_mouse_event = false;

HHOOK mouse_hook = NULL;
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    static bool close_tab_ing = false;
    static bool wheel_tab_ing = false;
    
    bool close_tab = false;
    bool keep_tab = false;
    bool bookmark_new_tab = false;

    if (nCode==HC_ACTION)
    {
        PMOUSEHOOKSTRUCT pmouse = (PMOUSEHOOKSTRUCT) lParam;

        if (wParam == WM_RBUTTONUP && wheel_tab_ing)
        {
            wheel_tab_ing = false;
            if (MouseGesture)
            {
                gesture_mgr.OnRButtonUp(pmouse, true);
            }
            return 1;
        }

        // 处理鼠标手势
        if(MouseGesture)
        {
            bool handled = false;

            if (wParam == WM_RBUTTONDOWN || wParam == WM_NCRBUTTONDOWN)
            {
                handled = gesture_mgr.OnRButtonDown(pmouse);
            }
            if (wParam == WM_RBUTTONUP || wParam == WM_NCRBUTTONUP)
            {
                handled = gesture_mgr.OnRButtonUp(pmouse);
            }
            if (wParam == WM_MOUSEMOVE || wParam == WM_NCMOUSEMOVE)
            {
                handled = gesture_mgr.OnMouseMove(pmouse);
            }

            if(handled)
            {
                return 1;
            }
        }

        if (RightTabSwitch)
        {
            if (wParam == WM_RBUTTONDOWN)
            {
                if (!ignore_mouse_event)
                {
                    return 1;
                }
            }
            if (wParam == WM_RBUTTONUP)
            {
                if (!ignore_mouse_event)
                {
                    ignore_mouse_event = true;
                    SendOneMouse(MOUSEEVENTF_RIGHTDOWN);
                    SendOneMouse(MOUSEEVENTF_RIGHTUP);
                    return 1;
                }
                ignore_mouse_event = false;
            }
        }

        if (HoverActivateTab && ignore_mouse_event)
        {
            if (wParam == WM_LBUTTONDBLCLK)
            {
                ignore_mouse_event = false;
                return 1;
            }
        }

        if (HoverActivateTab && wParam == WM_MOUSEMOVE)
        {
            HWND hwnd = WindowFromPoint(pmouse->pt);
            if (tracking_hwnd.find(hwnd) == tracking_hwnd.end())
            {
                TRACKMOUSEEVENT MouseEvent;
                MouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
                MouseEvent.dwFlags = TME_HOVER | TME_LEAVE;
                MouseEvent.hwndTrack = hwnd;
                MouseEvent.dwHoverTime = HoverTime;
                if (::TrackMouseEvent(&MouseEvent))
                {
                    tracking_hwnd[hwnd] = true;
                }
            }
        }

        // 忽略鼠标移动消息
        if(wParam==WM_MOUSEMOVE || wParam==WM_NCMOUSEMOVE)
        {
            return CallNextHookEx(mouse_hook, nCode, wParam, lParam );;
        }

        IAccessible* TopContainerView = GetTopContainerView(WindowFromPoint(pmouse->pt));

        // 双击关闭
        if(wParam==WM_LBUTTONDBLCLK && DoubleClickCloseTab && IsOnOneTab(TopContainerView, pmouse->pt))
        {
            close_tab = true;
        }

        // 右键关闭（没有按住SHIFT）
        if(wParam==WM_RBUTTONUP && RightClickCloseTab && !IsPressed(VK_SHIFT) && IsOnOneTab(TopContainerView, pmouse->pt))
        {
            close_tab = true;
        }

        // 只有一个标签
        if(close_tab && KeepLastTab && IsOnlyOneTab(TopContainerView))
        {
            keep_tab = true;
        }

        if(wParam==WM_MBUTTONUP && IsOnOneTab(TopContainerView, pmouse->pt))
        {
            if(close_tab_ing)
            {
                close_tab_ing = false;
            }
            else
            {
                if(KeepLastTab && IsOnlyOneTab(TopContainerView))
                {
                    keep_tab = true;
                    close_tab = true;
                }
            }
        }

        if(wParam==WM_MOUSEWHEEL)
        {
            PMOUSEHOOKSTRUCTEX pwheel = (PMOUSEHOOKSTRUCTEX) lParam;
            int zDelta = GET_WHEEL_DELTA_WPARAM(pwheel->mouseData);
            if( HoverTabSwitch && IsOnTheTab(TopContainerView, pmouse->pt) )
            {
                // 切换标签页，发送ctrl+pagedown/pageup
                SendKeys(VK_CONTROL, zDelta>0 ? VK_PRIOR : VK_NEXT);
            }
            else if( RightTabSwitch && IsPressed(VK_RBUTTON) )
            {
                // 切换标签页，发送ctrl+pagedown/pageup
                SendKeys(VK_CONTROL, zDelta>0 ? VK_PRIOR : VK_NEXT);

                wheel_tab_ing = true;
                if(TopContainerView)
                {
                    TopContainerView->Release();
                }
                return 1;
            }
        }

        if(wParam==WM_LBUTTONUP && BookMarkNewTab && !IsPressed(VK_CONTROL) && IsOnOneBookmark(TopContainerView, pmouse->pt) )
        {
            if(!NotBlankTab || !IsBlankTab(TopContainerView))
            {
                bookmark_new_tab = true;
            }
        }

        if(TopContainerView)
        {
            TopContainerView->Release();
        }
    }

    if(keep_tab)
    {
        // 最后一个标签页要关闭，新建一个标签
        // 打开新标签页，发送ctrl+t
        SendKeys(VK_CONTROL, 'T');
    }

    if(close_tab)
    {
        // 发送中键消息，关闭标签
        SendKeys(VK_MBUTTON);
        close_tab_ing = true;
        return 1;
    }

    if(bookmark_new_tab)
    {
        if(FrontNewTab)
        {
            // 发送中键消息，新建标签，前台
            SendKeys(VK_MBUTTON, VK_SHIFT);
        }
        else
        {
            // 发送中键消息，关闭标签
            SendKeys(VK_MBUTTON);
        }
        return 1;
    }

    return CallNextHookEx(mouse_hook, nCode, wParam, lParam );
}


HHOOK keyboard_hook = NULL;
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    static bool open_url_ing = false;
    static bool close_tab_ing = false;
    if (nCode==HC_ACTION && !(lParam & 0x80000000)) // pressed
    {
        if(wParam==VK_RETURN && OpenUrlNewTab)
        {
            if(open_url_ing)
            {
                open_url_ing = false;
                return CallNextHookEx(keyboard_hook, nCode, wParam, lParam );
            }

            IAccessible* TopContainerView = GetTopContainerView(GetFocus());
            if( !IsPressed(VK_MENU) && IsOmniboxViewFocus(TopContainerView) )
            {
                if(!NotBlankTab || !IsBlankTab(TopContainerView))
                {
                    open_url_ing = true;
                }
            }

            if(TopContainerView)
            {
                TopContainerView->Release();
            }

            if(open_url_ing)
            {
                // 在新标签打开url，发送alt+enter
                SendKeys(VK_MENU, VK_RETURN);
                return 1;
            }
        }

        if (wParam == 'W' && close_tab_ing)
        {
            close_tab_ing = false;
            return CallNextHookEx(keyboard_hook, nCode, wParam, lParam);
        }

        if(wParam=='W' && IsPressed(VK_CONTROL) && !IsPressed(VK_SHIFT) && KeepLastTab)
        {
            bool keep_tab = false;

            IAccessible* TopContainerView = GetTopContainerView(GetForegroundWindow());
            if (IsOnlyOneTab(TopContainerView))
            {
                keep_tab = true;
            }

            if (TopContainerView)
            {
                TopContainerView->Release();
            }

            if (keep_tab)
            {
                // 打开新标签页，发送ctrl+t
                SendKeys(VK_CONTROL, 'T');

                // 切换到上一个标签页
                SendKeys(VK_CONTROL, VK_PRIOR);

                // 关闭标签页
                close_tab_ing = true;
                SendKeys(VK_CONTROL, 'W');
                return 1;
            }
        }
    }
    return CallNextHookEx(keyboard_hook, nCode, wParam, lParam );
}

void SendClick()
{
    ignore_mouse_event = true;
    SendOneMouse(MOUSEEVENTF_LEFTDOWN);
    SendOneMouse(MOUSEEVENTF_LEFTUP);
    std::thread th([]() {
        Sleep(500);
        ignore_mouse_event = false;
    });
    th.detach();
}

HHOOK message_hook = NULL;
LRESULT CALLBACK MessageProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        MSG* msg = (MSG*)lParam;
        if (msg->message == WM_MOUSEHOVER)
        {
            HWND hwnd = WindowFromPoint(msg->pt);
            IAccessible* TopContainerView = GetTopContainerView(hwnd);
            int index = 0;
            bool onclose;
            if (IsOnOneInactiveTab(TopContainerView, msg->pt, index, onclose))
            {
                if (onclose)
                {
                    if (GetForegroundWindow() != hwnd)
                    {
                        SetForegroundWindow(hwnd);
                    }
                    if (index >= 1 && index <= 9)
                    {
                        // 1到8标签用ctrl+数字直接跳，9为最后一个标签
                        SendKeys(VK_CONTROL, '0' + index);
                    }
                    else
                    {
                        // 移动鼠标位置点击后再移动回来
                        POINT pt;
                        GetCursorPos(&pt);
                        SetCursorPos(pt.x - 30, pt.y);
                        SendClick();
                        SetCursorPos(pt.x, pt.y);
                    }
                }
                else
                {
                    SendClick();
                }
            }
            if (TopContainerView)
            {
                TopContainerView->Release();
            }

            tracking_hwnd.erase(msg->hwnd);
        }
        else if (msg->message == WM_MOUSELEAVE)
        {
            tracking_hwnd.erase(msg->hwnd);
        }
    }
    return CallNextHookEx(message_hook, nCode, wParam, lParam);
}

void TabBookmark()
{
    if(!wcsstr(GetCommandLineW(), L"--channel"))
    {
        mouse_hook = SetWindowsHookEx(WH_MOUSE, MouseProc, hInstance, GetCurrentThreadId());
        keyboard_hook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, hInstance, GetCurrentThreadId());
        message_hook = SetWindowsHookEx(WH_GETMESSAGE, MessageProc, hInstance, GetCurrentThreadId());

        if(MouseGesture)
        {
            std::thread th(StartGestureThread);
            th.detach();
        }
    }
}
