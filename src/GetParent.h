typedef LONG (WINAPI *QueryInformationProcess)(HANDLE,UINT,PVOID,ULONG,PULONG);

typedef struct
{
    MWORD ExitStatus;
    MWORD PebBaseAddress;
    MWORD AffinityMask;
    MWORD BasePriority;
    MWORD UniqueProcessId;
    MWORD InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION;

DWORD GetParentProcessID()
{
    QueryInformationProcess NtQueryInformationProcess = (QueryInformationProcess)GetProcAddress(GetModuleHandleA("ntdll"), "NtQueryInformationProcess");
    if (NtQueryInformationProcess)
    {
        PROCESS_BASIC_INFORMATION pbi;
        LONG status = NtQueryInformationProcess(GetCurrentProcess(), 0, (PVOID)&pbi, sizeof(pbi), NULL);

        if(!status)
        {
            return (DWORD)pbi.InheritedFromUniqueProcessId;
        }
    }
    return 0;
}

bool GetParentPath(wchar_t* path)
{
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, GetParentProcessID());
    if(hProcess)
    {
        DWORD dwSize = MAX_PATH;
        bool ret = QueryFullProcessImageNameW(hProcess, 0, path, &dwSize)!=0;

        CloseHandle(hProcess);
        return ret;
    }

    return false;
}
