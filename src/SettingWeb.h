#include "cJSON\cJSON.h"

const char extra_header[] = "Content-Type: text/html; charset=utf-8\r\nAccess-Control-Allow-Origin: http://settings.shuax.com";

void ReadList(cJSON *root, const wchar_t *iniPath, const wchar_t *name)
{
    cJSON *list = cJSON_CreateArray();

    auto contents = GetSection(name, iniPath);
    for (auto &content : contents)
    {
        cJSON_AddItemToArray(list, cJSON_CreateString(utf16to8(content.c_str()).c_str()));
    }

    cJSON_AddItemToObject(root, utf16to8(name).c_str(), list);
}

void ReadValue(cJSON *node, const wchar_t *iniPath, const wchar_t *section, const wchar_t *name)
{
    wchar_t value[256];
    GetPrivateProfileStringW(section, name, L"", value, MAX_PATH, iniPath);
    cJSON_AddItemToObject(node, utf16to8(name).c_str(), cJSON_CreateString(utf16to8(value).c_str()));
}

static bool http_post(struct mg_connection *nc, struct http_message *hm)
{
    const wchar_t *iniPath = (const wchar_t *)nc->mgr->user_data;

    if (mg_vcmp(&hm->uri, "/get_setting") == 0)
    {
        cJSON *root = cJSON_CreateObject();
        ReadList(root, iniPath, L"追加参数");
        ReadList(root, iniPath, L"启动时运行");
        ReadList(root, iniPath, L"关闭时运行");
        ReadList(root, iniPath, L"鼠标手势");

        cJSON *node = cJSON_CreateObject();
        ReadValue(node, iniPath, L"基本设置", L"数据目录");
        ReadValue(node, iniPath, L"基本设置", L"老板键");
        ReadValue(node, iniPath, L"基本设置", L"新标签页面");
        ReadValue(node, iniPath, L"基本设置", L"移除更新错误");
        ReadValue(node, iniPath, L"基本设置", L"自动结束运行程序");
        ReadValue(node, iniPath, L"基本设置", L"便携化");
        cJSON_AddItemToObject(root, utf16to8(L"基本设置").c_str(), node);

        node = cJSON_CreateObject();
        ReadValue(node, iniPath, L"界面增强", L"双击关闭标签页");
        ReadValue(node, iniPath, L"界面增强", L"右键关闭标签页");
        ReadValue(node, iniPath, L"界面增强", L"悬停激活标签页");
        ReadValue(node, iniPath, L"界面增强", L"悬停时间");
        ReadValue(node, iniPath, L"界面增强", L"保留最后标签");
        ReadValue(node, iniPath, L"界面增强", L"悬停快速标签切换");
        ReadValue(node, iniPath, L"界面增强", L"右键快速标签切换");
        ReadValue(node, iniPath, L"界面增强", L"新标签打开书签");
        ReadValue(node, iniPath, L"界面增强", L"新标签打开网址");
        ReadValue(node, iniPath, L"界面增强", L"新标签页不生效");
        ReadValue(node, iniPath, L"界面增强", L"前台打开新标签");
        cJSON_AddItemToObject(root, utf16to8(L"界面增强").c_str(), node);

        node = cJSON_CreateObject();
        ReadValue(node, iniPath, L"检查更新", L"更新器地址");
        ReadValue(node, iniPath, L"检查更新", L"检查版本");
        cJSON_AddItemToObject(root, utf16to8(L"检查更新").c_str(), node);

        node = cJSON_CreateObject();
        ReadValue(node, iniPath, L"鼠标手势", L"启用");
        ReadValue(node, iniPath, L"鼠标手势", L"轨迹");
        ReadValue(node, iniPath, L"鼠标手势", L"动作");
        cJSON_AddItemToObject(root, utf16to8(L"鼠标手势开关").c_str(), node);

        cJSON_AddItemToObject(root, utf16to8(L"version").c_str(), cJSON_CreateString(RELEASE_VER_STR));

        char *str = cJSON_PrintUnformatted(root);
        int len = (int)strlen(str);
        cJSON_Delete(root);

        mg_send_head(nc, 200, len, extra_header);
        mg_send(nc, str, len);
        free(str);
    }
    else if (mg_vcmp(&hm->uri, "/set_setting") == 0)
    {
        char section[200];
        char name[200];
        char value[200];
        mg_get_http_var(&hm->body, "section", section, sizeof(section));
        mg_get_http_var(&hm->body, "name", name, sizeof(name));
        mg_get_http_var(&hm->body, "value", value, sizeof(value));
        //DebugLog(L"set_setting %s %s=%s", utf8to16(section).c_str(), utf8to16(name).c_str(), utf8to16(value).c_str());

        ::WritePrivateProfileString(utf8to16(section).c_str(), utf8to16(name).c_str(), utf8to16(value).c_str(), iniPath);
        ReadConfig(iniPath);

        mg_send_head(nc, 200, 2, extra_header);
        mg_send(nc, "{}", 2);
    }
    else if (mg_vcmp(&hm->uri, "/del_setting") == 0)
    {
        char section[200];
        char name[200];
        mg_get_http_var(&hm->body, "section", section, sizeof(section));
        mg_get_http_var(&hm->body, "name", name, sizeof(name));
        //DebugLog(L"del_setting %s %s=%s", utf8to16(section).c_str(), utf8to16(name).c_str());

        ::WritePrivateProfileString(utf8to16(section).c_str(), utf8to16(name).c_str(), NULL, iniPath);

        mg_send_head(nc, 200, 2, extra_header);
        mg_send(nc, "{}", 2);
    }
    else if (mg_vcmp(&hm->uri, "/add_section") == 0)
    {
        char section[200];
        char value[200];
        mg_get_http_var(&hm->body, "section", section, sizeof(section));
        mg_get_http_var(&hm->body, "value", value, sizeof(value));

        auto contents = GetSection(utf8to16(section).c_str(), iniPath);
        contents.push_back(utf8to16(value));
        SetSection(utf8to16(section).c_str(), contents, iniPath);

        mg_send_head(nc, 200, 2, extra_header);
        mg_send(nc, "{}", 2);
    }
    else if (mg_vcmp(&hm->uri, "/del_section") == 0)
    {
        char section[200];
        char value[200];
        mg_get_http_var(&hm->body, "section", section, sizeof(section));
        mg_get_http_var(&hm->body, "value", value, sizeof(value));

        auto contents = GetSection(utf8to16(section).c_str(), iniPath);

        int index = 0;
        for (auto &content : contents)
        {
            if (_wcsicmp(content.c_str(), utf8to16(value).c_str()) == 0)
            {
                contents.erase(contents.begin() + index);
                SetSection(utf8to16(section).c_str(), contents, iniPath);
                break;
            }
            index++;
        }

        mg_send_head(nc, 200, 2, extra_header);
        mg_send(nc, "{}", 2);
    }
    else
    {
        return false;
    }

    return true;
}

void not_found(struct mg_connection *nc)
{
    const char reason[] = "Not Found";
    mg_send_head(nc, 404, sizeof(reason) - 1, extra_header);
    mg_send(nc, reason, sizeof(reason) - 1);
}

static void http_handler(struct mg_connection *nc, int ev, void *ev_data)
{
    switch (ev)
    {
    case MG_EV_HTTP_REQUEST:
    {
        struct http_message *hm = (struct http_message *) ev_data;
        //DebugLog(L"%.*S %.*S %.*S", (int)hm->method.len, hm->method.p, (int)hm->uri.len, hm->uri.p, (int)hm->body.len, hm->body.p);

        if (mg_vcmp(&hm->method, "POST") == 0)
        {
            if (!http_post(nc, hm))
            {
                not_found(nc);
            }
        }
        else
        {
            //404
            not_found(nc);
        }
        nc->flags |= MG_F_SEND_AND_CLOSE;
    }
    break;
    default:
        break;
    }
}

void WebThread(const std::wstring iniPath)
{
    struct mg_mgr mgr;
    struct mg_connection *nc;

    Sleep(200);
    mg_mgr_init(&mgr, (void *)iniPath.c_str());

    for (int http_port = 10000; http_port < 10020; http_port++)
    {
        char http_address[1024];
        wsprintfA(http_address, "127.0.0.1:%d", http_port);
        if (nc = mg_bind(&mgr, http_address, http_handler))
        {
            break;
        }
    }

    if (nc)
    {
        mg_set_protocol_http_websocket(nc);
        for (;;) {
            mg_mgr_poll(&mgr, 100);
        }
    }
    else
    {
        DebugLog(L"WebThread failed");
    }
    mg_mgr_free(&mgr);
}

void SettingWeb(const wchar_t *iniPath)
{
    if (StopWeb)
    {
        return;
    }

    std::thread th(WebThread, iniPath);
    th.detach();
}