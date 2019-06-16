wchar_t html_file[MAX_PATH];

bool RemoveUpdateError = false;

DWORD resources_pak_size = 0;

#pragma pack(push)
#pragma pack(1)

#define PACK_FILE_VERSION  (4)

struct PAK_HEADER
{
    uint32_t version;
    uint32_t num_entries;
    uint8_t encodeing;
};

struct PAK_ENTRY
{
    uint16_t id;
    uint32_t offset;
};

#pragma pack(pop)

template<typename Function>
void TraversalPakFile(uint8_t *buffer, Function f)
{
    PAK_HEADER *pak_header = (PAK_HEADER*)buffer;

    // 检查文件头
    if (pak_header->version != PACK_FILE_VERSION) return;
    if (pak_header->encodeing != 1) return;

    PAK_ENTRY *pak_entry = (PAK_ENTRY*)(buffer + sizeof(PAK_HEADER));

    // 为了保存最后一条的"下一条"，这条特殊的条目的id一定为0
    PAK_ENTRY *end_entry = pak_entry + pak_header->num_entries;
    if (end_entry->id != 0) return;

    //
    for (uint32_t i = 0; i < pak_header->num_entries; i++)
    {
        PAK_ENTRY *next_entry = pak_entry + 1;
        if (f(buffer + pak_entry->offset, buffer + next_entry->offset, next_entry->offset - pak_entry->offset))
        {
            // 如果不再需要遍历就退出
            break;
        }

        pak_entry = next_entry;
    }
}

void CustomNewTab(uint8_t *buffer)
{
    BYTE search[] = R"(<div id="ntp-contents">)";

    uint8_t* pos = memmem(buffer, resources_pak_size, search, sizeof(search) - 1);
    if(pos)
    {
        TraversalPakFile(buffer, [=](uint8_t *begin, uint8_t* end, uint32_t size){
            if (pos >= begin && pos <= end)
            {
                // 填充空格
                memset(begin, ' ', size);

                if(wcscmp(html_file, L"%demo%")==0)
                {
                    wcscpy(html_file, L"http://settings.shuax.com/gc/demo.html");
                }
                if (isStartWith(html_file, L"http"))
                {
                    char s1[] = R"(<iframe src=")";
                    size_t s1_len = strlen(s1);
                    memcpy(begin, s1, s1_len);

                    std::string url = utf16to8(html_file);
                    memcpy(begin + s1_len, url.c_str(), url.length());

                    char s2[] = R"(" style="position:fixed; top:0px; left:0px; bottom:0px; right:0px; width:100%; height:100%; border:none; margin:0; padding:0; overflow:hidden;" />)";
                    size_t s2_len = strlen(s2);
                    memcpy(begin + s1_len + url.length(), s2, s2_len);
                    return true;
                }

                // 读取文件覆盖内存
                FILE * fp = _wfopen(html_file, L"rb");
                if (fp)
                {
                    fread(begin, size, 1, fp);
                    fclose(fp);
                }
                return true;
            }
            return false;
        });
    }
    else
    {
        DebugLog(L"CustomNewTab failed");
    }
}

void RemovePakUpdateError(uint8_t *buffer)
{
    BYTE search_start[] = R"(container.hidden = status == 'disabled';)";

    BYTE patch[]  = R"(container.hidden = status  = 'disabled';)";
    size_t patch_size = sizeof(patch) - 1;

    uint8_t* start = memmem(buffer, resources_pak_size, search_start, sizeof(search_start) - 1);
    if(start)
    {
        // 打补丁
        memcpy(start, patch, patch_size);
    }
    else
    {
        DebugLog(L"RemovePakUpdateError failed");
    }
}

void ModifyHelpPage(uint8_t *buffer)
{
    BYTE search_start[] = R"(
<body class="uber-frame">
  <header>
    <h1 i18n-content="aboutTitle"></h1>
)";

    uint8_t* pos = memmem(buffer, resources_pak_size, search_start, sizeof(search_start) - 1);
    if (pos)
    {
        TraversalPakFile(buffer, [=](uint8_t *begin, uint8_t* end, uint32_t size) {
            if (pos >= begin && pos <= end)
            {
                // 压缩HTML以备写入补丁信息
                std::string html((char*)begin, size);
                compression_html(html);

                const char prouct_title[] = u8R"(<br><div>GreenChrome )" RELEASE_VER_STR u8R"(</div><div id="product-container">)";
                ReplaceStringInPlace(html, R"(</div><div id="product-container">)", prouct_title);

                if (html.length() <= size)
                {
                    // 写入修改
                    memcpy(begin, html.c_str(), html.length());

                    // 填充空格
                    memset(begin + html.length(), ' ', size - html.length());
                }
                else
                {
                    DebugLog(L"ModifyHelpPage size failed");
                }

                return true;
            }
            return false;
        });
    }
    else
    {
        DebugLog(L"ModifyHelpPage failed");
    }
}


void ModifySettingsPage(uint8_t *buffer)
{
    if (StopWeb)
    {
        return;
    }
    BYTE search_start[] = R"(<h3 i18n-content="sectionTitleSync"></h3>)";

    uint8_t* pos = memmem(buffer, resources_pak_size, search_start, sizeof(search_start) - 1);
    if (pos)
    {
        TraversalPakFile(buffer, [=](uint8_t *begin, uint8_t* end, uint32_t size) {
            if (pos >= begin && pos <= end)
            {
                // 压缩HTML以备写入补丁信息
                std::string html((char*)begin, size);
                compression_html(html);

                const char prouct_title[] = u8R"(<section><h3>GreenChrome</h3><div class="settings-row">如果喜欢它，可以 <a class="alert-link" href="https://www.shuax.com/donate.html" target="_blank">鼓励作者</a> 继续完善。</div><button><a href="http://settings.shuax.com/gc/" target="_blank" style="text-decoration:none;color:#444;">点击设置</a></button></section><section id="sync-section">)";
                ReplaceStringInPlace(html, R"(<section id="sync-section">)", prouct_title);

                if (html.length() <= size)
                {
                    // 写入修改
                    memcpy(begin, html.c_str(), html.length());

                    // 填充空格
                    memset(begin + html.length(), ' ', size - html.length());
                }
                else
                {
                    DebugLog(L"ModifySettingsPage size failed");
                }

                return true;
            }
            return false;
        });
    }
    else
    {
        DebugLog(L"ModifySettingsPage failed");
    }
}

HANDLE resources_pak_map = NULL;

typedef HANDLE (WINAPI *pMapViewOfFile)(
    _In_ HANDLE hFileMappingObject,
    _In_ DWORD  dwDesiredAccess,
    _In_ DWORD  dwFileOffsetHigh,
    _In_ DWORD  dwFileOffsetLow,
    _In_ SIZE_T dwNumberOfBytesToMap
);

pMapViewOfFile RawMapViewOfFile = NULL;

HANDLE WINAPI MyMapViewOfFile(
    _In_ HANDLE hFileMappingObject,
    _In_ DWORD  dwDesiredAccess,
    _In_ DWORD  dwFileOffsetHigh,
    _In_ DWORD  dwFileOffsetLow,
    _In_ SIZE_T dwNumberOfBytesToMap
)
{
    if(hFileMappingObject == resources_pak_map)
    {
        // 修改属性为可修改
        LPVOID buffer = RawMapViewOfFile(hFileMappingObject, FILE_MAP_COPY, dwFileOffsetHigh,
            dwFileOffsetLow, dwNumberOfBytesToMap);

        // 不再需要hook
        resources_pak_map = NULL;
        MH_DisableHook(MapViewOfFile);

        if(buffer)
        {
            if(html_file[0])
            {
                // 自定义新标签
                CustomNewTab((BYTE*)buffer);
            }

            if(RemoveUpdateError)
            {
                //移除更新错误
                RemovePakUpdateError((BYTE*)buffer);
            }

            // 修改关于页面
            ModifyHelpPage((BYTE*)buffer);
            // 修改设置页面
            ModifySettingsPage((BYTE*)buffer);
        }

        return buffer;
    }

    return RawMapViewOfFile(hFileMappingObject, dwDesiredAccess, dwFileOffsetHigh,
        dwFileOffsetLow, dwNumberOfBytesToMap);
}

HANDLE resources_pak_file = NULL;

typedef HANDLE (WINAPI *pCreateFileMapping)(
    _In_     HANDLE                hFile,
    _In_opt_ LPSECURITY_ATTRIBUTES lpAttributes,
    _In_     DWORD                 flProtect,
    _In_     DWORD                 dwMaximumSizeHigh,
    _In_     DWORD                 dwMaximumSizeLow,
    _In_opt_ LPCTSTR               lpName
);

pCreateFileMapping RawCreateFileMapping = NULL;

HANDLE WINAPI MyCreateFileMapping(
    _In_     HANDLE                hFile,
    _In_opt_ LPSECURITY_ATTRIBUTES lpAttributes,
    _In_     DWORD                 flProtect,
    _In_     DWORD                 dwMaximumSizeHigh,
    _In_     DWORD                 dwMaximumSizeLow,
    _In_opt_ LPCTSTR               lpName
)
{
    if(hFile == resources_pak_file)
    {
        // 修改属性为可修改
        resources_pak_map = RawCreateFileMapping(hFile, lpAttributes, PAGE_WRITECOPY,
            dwMaximumSizeHigh, dwMaximumSizeLow, lpName);

        // 不再需要hook
        resources_pak_file = NULL;
        MH_DisableHook(CreateFileMappingW);

        if (MH_CreateHook(MapViewOfFile, MyMapViewOfFile, (LPVOID*)&RawMapViewOfFile) == MH_OK)
        {
            MH_EnableHook(MapViewOfFile);
        }

        return resources_pak_map;
    }
    return RawCreateFileMapping(hFile, lpAttributes, flProtect, dwMaximumSizeHigh,
        dwMaximumSizeLow, lpName);
}

typedef HANDLE (WINAPI *pCreateFile)(
    _In_     LPCTSTR               lpFileName,
    _In_     DWORD                 dwDesiredAccess,
    _In_     DWORD                 dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_     DWORD                 dwCreationDisposition,
    _In_     DWORD                 dwFlagsAndAttributes,
    _In_opt_ HANDLE                hTemplateFile
);

pCreateFile RawCreateFile = NULL;

HANDLE WINAPI MyCreateFile(
    _In_     LPCTSTR               lpFileName,
    _In_     DWORD                 dwDesiredAccess,
    _In_     DWORD                 dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_     DWORD                 dwCreationDisposition,
    _In_     DWORD                 dwFlagsAndAttributes,
    _In_opt_ HANDLE                hTemplateFile
)
{
    // 禁止生成debug.log
    if(isEndWith(lpFileName, L"debug.log"))
    {
        return INVALID_HANDLE_VALUE;
    }

    HANDLE file = RawCreateFile(lpFileName, dwDesiredAccess, dwShareMode,
        lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes,
        hTemplateFile);

    if(isEndWith(lpFileName, L"resources.pak"))
    {
        resources_pak_file = file;
        resources_pak_size = GetFileSize(resources_pak_file, NULL);

        if (MH_CreateHook(CreateFileMappingW, MyCreateFileMapping, (LPVOID*)&RawCreateFileMapping) == MH_OK)
        {
            MH_EnableHook(CreateFileMappingW);
        }

        // 不再需要hook
        //MH_DisableHook(CreateFileW);
    }

    return file;
}

// 破解自定义页面安全策略
void CrackCSP()
{
    /*const char kChromeURLContentSecurityPolicyHeaderBase[] =
    "Content-Security-Policy: ";

    if (add_content_security_policy_) {
    std::string base = kChromeURLContentSecurityPolicyHeaderBase;
    base.append(RequiresUnsafeEval() ? " 'unsafe-eval'; " : "; ");
    base.append(content_security_policy_object_source_);
    base.append(content_security_policy_frame_source_);
    info->headers->AddHeader(base);
    }

    if (add_content_security_policy_) {
    std::string base = kChromeURLContentSecurityPolicyHeaderBase;
    base.append(content_security_policy_script_source_);
    base.append(content_security_policy_object_source_);
    base.append(content_security_policy_child_source_);
    base.append(content_security_policy_style_source_);
    base.append(content_security_policy_image_source_);
    info->headers->AddHeader(base);
    }
    */
#ifdef _WIN64
    BYTE search[][10] = {
        { 0x02, 0x00, 0x00, 0x0F, 0x84, 0x88, 0x00, 0x00, 0x00, 0x48 },
        { 0x02, 0x00, 0x00, 0x0F, 0x84, 0x8D, 0x00, 0x00, 0x00, 0x45 },
        { 0x02, 0x00, 0x00, 0x0F, 0x84, 0xC9, 0x00, 0x00, 0x00, 0x41 },
        { 0x02, 0x00, 0x00, 0x0F, 0x84, 0xBD, 0x00, 0x00, 0x00, 0x41 },
    };

    bool crack_ok = false;
    for (size_t i = 0; i < _countof(search); i++)
    {
        uint8_t *csp = SearchModule(L"chrome.dll", search[i], sizeof(search[i]));
        if (csp)
        {
            BYTE patch[] = { 0x90, 0xE9 };
            WriteMemory(csp + 3, patch, sizeof(patch));
            crack_ok = true;
            break;
        }
    }
    if (!crack_ok)
    {
        DebugLog(L"patch csp failed");
    }
#else
    BYTE search1[] = { 0x01, 0x00, 0x00, 0x00, 0x74, 0x5F, 0x68 };
    BYTE search2[] = { 0x99, 0x01, 0x00, 0x00, 0x00, 0x0F, 0x84 };
    BYTE search3[] = { 0xA1, 0x01, 0x00, 0x00, 0x00, 0x0F, 0x84 };
    uint8_t *csp = SearchModule(L"chrome.dll", search1, sizeof(search1));
    if (csp)
    {
        BYTE patch[] = { 0xEB };
        WriteMemory(csp + 4, patch, sizeof(patch));
    }
    else if (csp = SearchModule(L"chrome.dll", search2, sizeof(search2)))
    {
        BYTE patch[] = { 0x90, 0xE9 };
        WriteMemory(csp + 5, patch, sizeof(patch));
    }
    else if (csp = SearchModule(L"chrome.dll", search3, sizeof(search3)))
    {
        BYTE patch[] = { 0x90, 0xE9 };
        WriteMemory(csp + 5, patch, sizeof(patch));
    }
    else
    {
        DebugLog(L"patch csp failed");
    }
#endif
}

void PatchResourcesPak(const wchar_t *iniPath)
{
    GetPrivateProfileStringW(L"基本设置", L"新标签页面", L"", html_file, MAX_PATH, iniPath);
    RemoveUpdateError = GetPrivateProfileIntW(L"基本设置", L"移除更新错误", 0, iniPath)==1;

    // 扩展环境变量
    std::wstring path = ExpandEnvironmentPath(html_file);

    // exe路径
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);

    // exe所在文件夹
    wchar_t exeFolder[MAX_PATH];
    wcscpy(exeFolder, exePath);
    PathRemoveFileSpecW(exeFolder);

    // 扩展%app%
    ReplaceStringInPlace(path, L"%app%", exeFolder);

    wcscpy(html_file, path.c_str());

    if(html_file[0])
    {
        // 破解自定义页面安全策略
        CrackCSP();
    }

    MH_STATUS status = MH_CreateHook(CreateFileW, MyCreateFile, (LPVOID*)&RawCreateFile);
    if (status == MH_OK)
    {
        MH_EnableHook(CreateFileW);
    }
    else
    {
        DebugLog(L"MH_CreateHook PatchResourcesPak failed:%d", status);
    }
}
