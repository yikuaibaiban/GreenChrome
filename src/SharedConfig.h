#pragma data_seg(".SHARED")

bool DoubleClickCloseTab = false;
bool RightClickCloseTab = false;
bool HoverActivateTab = false;
int HoverTime = HOVER_DEFAULT;
bool KeepLastTab = false;
bool HoverTabSwitch = false;
bool RightTabSwitch = false;
bool BookMarkNewTab = false;
bool OpenUrlNewTab = false;
bool NotBlankTab = false;
bool FrontNewTab = false;
bool MouseGesture = false;
bool StopWeb = false;

#pragma data_seg()
#pragma comment(linker, "/section:.SHARED,RWS")

void ReadConfig(const wchar_t *iniPath)
{
    DoubleClickCloseTab = GetPrivateProfileInt(L"界面增强", L"双击关闭标签页", 1, iniPath) == 1;
    RightClickCloseTab = GetPrivateProfileInt(L"界面增强", L"右键关闭标签页", 0, iniPath) == 1;
    HoverActivateTab = GetPrivateProfileInt(L"界面增强", L"悬停激活标签页", 0, iniPath) == 1;
    HoverTime = GetPrivateProfileInt(L"界面增强", L"悬停时间", HOVER_DEFAULT, iniPath);
    KeepLastTab = GetPrivateProfileInt(L"界面增强", L"保留最后标签", 1, iniPath) == 1;
    HoverTabSwitch = GetPrivateProfileInt(L"界面增强", L"悬停快速标签切换", 1, iniPath) == 1;
    RightTabSwitch = GetPrivateProfileInt(L"界面增强", L"右键快速标签切换", 1, iniPath) == 1;
    BookMarkNewTab = GetPrivateProfileInt(L"界面增强", L"新标签打开书签", 1, iniPath) == 1;
    OpenUrlNewTab = GetPrivateProfileInt(L"界面增强", L"新标签打开网址", 0, iniPath) == 1;
    NotBlankTab = GetPrivateProfileInt(L"界面增强", L"新标签页不生效", 1, iniPath) == 1;
    FrontNewTab = GetPrivateProfileInt(L"界面增强", L"前台打开新标签", 1, iniPath) == 1;

    MouseGesture = GetPrivateProfileInt(L"鼠标手势", L"启用", 1, iniPath) == 1;

    StopWeb = GetPrivateProfileInt(L"基本设置", L"停用WEB设置", 0, iniPath) == 1;
}