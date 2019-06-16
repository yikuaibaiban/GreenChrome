wchar_t user_data_path[MAX_PATH];

typedef BOOL (WINAPI *pSHGetFolderPath)(
  _In_  HWND   hwndOwner,
  _In_  int    nFolder,
  _In_  HANDLE hToken,
  _In_  DWORD  dwFlags,
  _Out_ LPTSTR pszPath
);

pSHGetFolderPath RawSHGetFolderPath = NULL;

BOOL WINAPI MySHGetFolderPath(
  _In_  HWND   hwndOwner,
  _In_  int    nFolder,
  _In_  HANDLE hToken,
  _In_  DWORD  dwFlags,
  _Out_ LPTSTR pszPath
)
{
    BOOL result = RawSHGetFolderPath(hwndOwner, nFolder, hToken, dwFlags, pszPath);
    if (nFolder == CSIDL_LOCAL_APPDATA)
    {
        // 用户数据路径
        wcscpy(pszPath, user_data_path);
    }

    return result;
}

typedef struct _CRYPTOAPI_BLOB {
  DWORD cbData;
  BYTE  *pbData;
} CRYPT_INTEGER_BLOB, *PCRYPT_INTEGER_BLOB, CRYPT_UINT_BLOB, *PCRYPT_UINT_BLOB, CRYPT_OBJID_BLOB, *PCRYPT_OBJID_BLOB, CERT_NAME_BLOB, CERT_RDN_VALUE_BLOB, *PCERT_NAME_BLOB, *PCERT_RDN_VALUE_BLOB, CERT_BLOB, *PCERT_BLOB, CRL_BLOB, *PCRL_BLOB, DATA_BLOB, *PDATA_BLOB, CRYPT_DATA_BLOB, *PCRYPT_DATA_BLOB, CRYPT_HASH_BLOB, *PCRYPT_HASH_BLOB, CRYPT_DIGEST_BLOB, *PCRYPT_DIGEST_BLOB, CRYPT_DER_BLOB, PCRYPT_DER_BLOB, CRYPT_ATTR_BLOB, *PCRYPT_ATTR_BLOB;

typedef struct _CRYPTPROTECT_PROMPTSTRUCT {
  DWORD   cbSize;
  DWORD   dwPromptFlags;
  HWND    hwndApp;
  LPCWSTR szPrompt;
} CRYPTPROTECT_PROMPTSTRUCT, *PCRYPTPROTECT_PROMPTSTRUCT;

BOOL WINAPI MyCryptProtectData(
  _In_       DATA_BLOB                 *pDataIn,
  _In_opt_   LPCWSTR                   szDataDescr,
  _In_opt_   DATA_BLOB                 *pOptionalEntropy,
  _Reserved_ PVOID                     pvReserved,
  _In_opt_   CRYPTPROTECT_PROMPTSTRUCT *pPromptStruct,
  _In_       DWORD                     dwFlags,
  _Out_      DATA_BLOB                 *pDataOut
)
{
    pDataOut->cbData = pDataIn->cbData;
    pDataOut->pbData = (BYTE*)LocalAlloc(LMEM_FIXED, pDataOut->cbData);
    memcpy(pDataOut->pbData, pDataIn->pbData, pDataOut->cbData);
    return true;
}

typedef BOOL(WINAPI *pCryptUnprotectData)(
    _In_       DATA_BLOB                 *pDataIn,
    _Out_opt_  LPWSTR                    *ppszDataDescr,
    _In_opt_   DATA_BLOB                 *pOptionalEntropy,
    _Reserved_ PVOID                     pvReserved,
    _In_opt_   CRYPTPROTECT_PROMPTSTRUCT *pPromptStruct,
    _In_       DWORD                     dwFlags,
    _Out_      DATA_BLOB                 *pDataOut
    );

pCryptUnprotectData RawCryptUnprotectData = NULL;

BOOL WINAPI MyCryptUnprotectData(
  _In_       DATA_BLOB                 *pDataIn,
  _Out_opt_  LPWSTR                    *ppszDataDescr,
  _In_opt_   DATA_BLOB                 *pOptionalEntropy,
  _Reserved_ PVOID                     pvReserved,
  _In_opt_   CRYPTPROTECT_PROMPTSTRUCT *pPromptStruct,
  _In_       DWORD                     dwFlags,
  _Out_      DATA_BLOB                 *pDataOut
)
{
    if (RawCryptUnprotectData(pDataIn, ppszDataDescr, pOptionalEntropy, pvReserved, pPromptStruct, dwFlags, pDataOut))
    {
        return true;
    }

    pDataOut->cbData = pDataIn->cbData;
    pDataOut->pbData = (BYTE*)LocalAlloc(LMEM_FIXED, pDataOut->cbData);
    memcpy(pDataOut->pbData, pDataIn->pbData, pDataOut->cbData);
    return true;
}

void CustomUserData(const wchar_t *iniPath)
{
    GetPrivateProfileStringW(L"基本设置", L"数据目录", L"", user_data_path, MAX_PATH, iniPath);

    // 扩展环境变量
    std::wstring path = ExpandEnvironmentPath(user_data_path);

    // exe路径
    wchar_t exeFolder[MAX_PATH];
    GetModuleFileNameW(NULL, exeFolder, MAX_PATH);
    PathRemoveFileSpecW(exeFolder);

    // 扩展%app%
    ReplaceStringInPlace(path, L"%app%", exeFolder);

    wcscpy(user_data_path, path.c_str());

    if(user_data_path[0])
    {
        //GetDefaultUserDataDirectory
        //*result = result->Append(chrome::kUserDataDirname);
        //const wchar_t kUserDataDirname[] = L"User Data";
        #ifdef _WIN64
        BYTE search[] = {0x48, 0x8B, 0xD1, 0xB9, 0x6E, 0x00, 0x00, 0x00, 0xE8};
        uint8_t *get_user_data = SearchModule(L"chrome.dll", search, sizeof(search));
        if (get_user_data && *(get_user_data + 17) == 0x0F && *(get_user_data + 18) == 0x84)
        {
            BYTE patch[] = { 0x90, 0xE9 };
            WriteMemory(get_user_data + 17, patch, sizeof(patch));
        }
        else if (get_user_data && *(get_user_data + 15) == 0x0F && *(get_user_data + 16) == 0x84)
        {
            BYTE patch[] = { 0x90, 0xE9 };
            WriteMemory(get_user_data + 15, patch, sizeof(patch));
        }
        else
        {
            DebugLog(L"patch user_data_path failed");
        }
        #else
        BYTE search[] = {0x57, 0x6A, 0x6E, 0xE8};
        uint8_t *get_user_data = SearchModule(L"chrome.dll", search, sizeof(search));
        if(get_user_data && *(get_user_data + 12) == 0x74)
        {
            BYTE patch[] = {0xEB};
            WriteMemory(get_user_data + 12, patch, sizeof(patch));
        }
        else
        {
            BYTE search[] = {0x6A, 0x6E, 0x8B, 0xD7 };
            uint8_t *get_user_data = SearchModule(L"chrome.dll", search, sizeof(search));
            if (get_user_data && *(get_user_data + 12) == 0x74)
            {
                BYTE patch[] = { 0xEB };
                WriteMemory(get_user_data + 12, patch, sizeof(patch));
            }
            else
            {
                DebugLog(L"patch user_data_path failed");
            }
        }
        #endif

        MH_STATUS status = MH_CreateHook(SHGetFolderPathW, MySHGetFolderPath, (LPVOID*)&RawSHGetFolderPath);
        if (status == MH_OK)
        {
            MH_EnableHook(SHGetFolderPathW);
        }
        else
        {
            DebugLog(L"MH_CreateHook CustomUserData failed:%d", status);
        }
    }
}



BOOL WINAPI FakeGetComputerName(
    _Out_   LPTSTR  lpBuffer,
    _Inout_ LPDWORD lpnSize
)
{
    return 0;
}

BOOL WINAPI FakeGetVolumeInformation(
    _In_opt_  LPCTSTR lpRootPathName,
    _Out_opt_ LPTSTR  lpVolumeNameBuffer,
    _In_      DWORD   nVolumeNameSize,
    _Out_opt_ LPDWORD lpVolumeSerialNumber,
    _Out_opt_ LPDWORD lpMaximumComponentLength,
    _Out_opt_ LPDWORD lpFileSystemFlags,
    _Out_opt_ LPTSTR  lpFileSystemNameBuffer,
    _In_      DWORD   nFileSystemNameSize
)
{
    return 0;
}

// 不让chrome使用GetComputerNameW，GetVolumeInformationW
// chromium/rlz/win/lib/machine_id_win.cc
void MakePortable(const wchar_t *iniPath)
{
    if(GetPrivateProfileIntW(L"基本设置", L"便携化", 0, iniPath)==1)
    {
        HMODULE kernel32 = LoadLibraryW(L"kernel32.dll");
        if(kernel32)
        {
            PBYTE GetComputerNameW = (PBYTE)GetProcAddress(kernel32, "GetComputerNameW");
            PBYTE GetVolumeInformationW = (PBYTE)GetProcAddress(kernel32, "GetVolumeInformationW");

            MH_STATUS status = MH_CreateHook(GetComputerNameW, FakeGetComputerName, NULL);
            if (status == MH_OK)
            {
                MH_EnableHook(GetComputerNameW);
            }
            else
            {
                DebugLog(L"MH_CreateHook GetComputerNameW failed:%d", status);
            }
            status = MH_CreateHook(GetVolumeInformationW, FakeGetVolumeInformation, NULL);
            if (status == MH_OK)
            {
                MH_EnableHook(GetVolumeInformationW);
            }
            else
            {
                DebugLog(L"MH_CreateHook GetVolumeInformationW failed:%d", status);
            }
        }

        //components/os_crypt/os_crypt_win.cc
        HMODULE Crypt32 = LoadLibraryW(L"Crypt32.dll");
        if(Crypt32)
        {
            PBYTE CryptProtectData = (PBYTE)GetProcAddress(Crypt32, "CryptProtectData");
            PBYTE CryptUnprotectData = (PBYTE)GetProcAddress(Crypt32, "CryptUnprotectData");

            MH_STATUS status = MH_CreateHook(CryptProtectData, MyCryptProtectData, NULL);
            if (status == MH_OK)
            {
                MH_EnableHook(CryptProtectData);
            }
            else
            {
                DebugLog(L"MH_CreateHook CryptProtectData failed:%d", status);
            }
            status = MH_CreateHook(CryptUnprotectData, MyCryptUnprotectData, (LPVOID*)&RawCryptUnprotectData);
            if (status == MH_OK)
            {
                MH_EnableHook(CryptUnprotectData);
            }
            else
            {
                DebugLog(L"MH_CreateHook CryptUnprotectData failed:%d", status);
            }
        }
    }
}
