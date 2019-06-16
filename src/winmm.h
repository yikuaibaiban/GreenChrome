#define EXTERNC extern "C"
#define EXPORT EXTERNC __declspec(dllexport) void __cdecl

#define NOP_FUNC(seq) { \
    __nop();\
    __nop();\
    __nop();\
    __nop();\
    __nop();\
    __nop();\
    __nop();\
    __nop();\
    __nop();\
    __nop();\
    __nop();\
    __nop();\
    __inbyte(seq);\
}
//用__inbyte来生成一点不一样的代码，避免被VS自动合并相同函数


// 声明导出函数
EXPORT CloseDriver() NOP_FUNC(1)
EXPORT DefDriverProc() NOP_FUNC(2)
EXPORT DriverCallback() NOP_FUNC(3)
EXPORT DrvGetModuleHandle() NOP_FUNC(4)
EXPORT GetDriverModuleHandle() NOP_FUNC(5)
EXPORT NotifyCallbackData() NOP_FUNC(6)
EXPORT OpenDriver() NOP_FUNC(7)
EXPORT PlaySound() NOP_FUNC(8)
EXPORT PlaySoundA() NOP_FUNC(9)
EXPORT PlaySoundW() NOP_FUNC(10)
EXPORT SendDriverMessage() NOP_FUNC(11)
EXPORT WOW32DriverCallback() NOP_FUNC(12)
EXPORT WOW32ResolveMultiMediaHandle() NOP_FUNC(13)
EXPORT WOWAppExit() NOP_FUNC(14)
EXPORT aux32Message() NOP_FUNC(15)
EXPORT auxGetDevCapsA() NOP_FUNC(16)
EXPORT auxGetDevCapsW() NOP_FUNC(17)
EXPORT auxGetNumDevs() NOP_FUNC(18)
EXPORT auxGetVolume() NOP_FUNC(19)
EXPORT auxOutMessage() NOP_FUNC(20)
EXPORT auxSetVolume() NOP_FUNC(21)
EXPORT joy32Message() NOP_FUNC(22)
EXPORT joyConfigChanged() NOP_FUNC(23)
EXPORT joyGetDevCapsA() NOP_FUNC(24)
EXPORT joyGetDevCapsW() NOP_FUNC(25)
EXPORT joyGetNumDevs() NOP_FUNC(26)
EXPORT joyGetPos() NOP_FUNC(27)
EXPORT joyGetPosEx() NOP_FUNC(28)
EXPORT joyGetThreshold() NOP_FUNC(29)
EXPORT joyReleaseCapture() NOP_FUNC(30)
EXPORT joySetCapture() NOP_FUNC(31)
EXPORT joySetThreshold() NOP_FUNC(32)
EXPORT mci32Message() NOP_FUNC(33)
EXPORT mciDriverNotify() NOP_FUNC(34)
EXPORT mciDriverYield() NOP_FUNC(35)
EXPORT mciExecute() NOP_FUNC(36)
EXPORT mciFreeCommandResource() NOP_FUNC(37)
EXPORT mciGetCreatorTask() NOP_FUNC(38)
EXPORT mciGetDeviceIDA() NOP_FUNC(39)
EXPORT mciGetDeviceIDFromElementIDA() NOP_FUNC(40)
EXPORT mciGetDeviceIDFromElementIDW() NOP_FUNC(41)
EXPORT mciGetDeviceIDW() NOP_FUNC(42)
EXPORT mciGetDriverData() NOP_FUNC(43)
EXPORT mciGetErrorStringA() NOP_FUNC(44)
EXPORT mciGetErrorStringW() NOP_FUNC(45)
EXPORT mciGetYieldProc() NOP_FUNC(46)
EXPORT mciLoadCommandResource() NOP_FUNC(47)
EXPORT mciSendCommandA() NOP_FUNC(48)
EXPORT mciSendCommandW() NOP_FUNC(49)
EXPORT mciSendStringA() NOP_FUNC(50)
EXPORT mciSendStringW() NOP_FUNC(51)
EXPORT mciSetDriverData() NOP_FUNC(52)
EXPORT mciSetYieldProc() NOP_FUNC(53)
EXPORT mid32Message() NOP_FUNC(54)
EXPORT midiConnect() NOP_FUNC(55)
EXPORT midiDisconnect() NOP_FUNC(56)
EXPORT midiInAddBuffer() NOP_FUNC(57)
EXPORT midiInClose() NOP_FUNC(58)
EXPORT midiInGetDevCapsA() NOP_FUNC(59)
EXPORT midiInGetDevCapsW() NOP_FUNC(60)
EXPORT midiInGetErrorTextA() NOP_FUNC(61)
EXPORT midiInGetErrorTextW() NOP_FUNC(62)
EXPORT midiInGetID() NOP_FUNC(63)
EXPORT midiInGetNumDevs() NOP_FUNC(64)
EXPORT midiInMessage() NOP_FUNC(65)
EXPORT midiInOpen() NOP_FUNC(66)
EXPORT midiInPrepareHeader() NOP_FUNC(67)
EXPORT midiInReset() NOP_FUNC(68)
EXPORT midiInStart() NOP_FUNC(69)
EXPORT midiInStop() NOP_FUNC(70)
EXPORT midiInUnprepareHeader() NOP_FUNC(71)
EXPORT midiOutCacheDrumPatches() NOP_FUNC(72)
EXPORT midiOutCachePatches() NOP_FUNC(73)
EXPORT midiOutClose() NOP_FUNC(74)
EXPORT midiOutGetDevCapsA() NOP_FUNC(75)
EXPORT midiOutGetDevCapsW() NOP_FUNC(76)
EXPORT midiOutGetErrorTextA() NOP_FUNC(77)
EXPORT midiOutGetErrorTextW() NOP_FUNC(78)
EXPORT midiOutGetID() NOP_FUNC(79)
EXPORT midiOutGetNumDevs() NOP_FUNC(80)
EXPORT midiOutGetVolume() NOP_FUNC(81)
EXPORT midiOutLongMsg() NOP_FUNC(82)
EXPORT midiOutMessage() NOP_FUNC(83)
EXPORT midiOutOpen() NOP_FUNC(84)
EXPORT midiOutPrepareHeader() NOP_FUNC(85)
EXPORT midiOutReset() NOP_FUNC(86)
EXPORT midiOutSetVolume() NOP_FUNC(87)
EXPORT midiOutShortMsg() NOP_FUNC(88)
EXPORT midiOutUnprepareHeader() NOP_FUNC(89)
EXPORT midiStreamClose() NOP_FUNC(90)
EXPORT midiStreamOpen() NOP_FUNC(91)
EXPORT midiStreamOut() NOP_FUNC(92)
EXPORT midiStreamPause() NOP_FUNC(93)
EXPORT midiStreamPosition() NOP_FUNC(94)
EXPORT midiStreamProperty() NOP_FUNC(95)
EXPORT midiStreamRestart() NOP_FUNC(96)
EXPORT midiStreamStop() NOP_FUNC(97)
EXPORT mixerClose() NOP_FUNC(98)
EXPORT mixerGetControlDetailsA() NOP_FUNC(99)
EXPORT mixerGetControlDetailsW() NOP_FUNC(100)
EXPORT mixerGetDevCapsA() NOP_FUNC(101)
EXPORT mixerGetDevCapsW() NOP_FUNC(102)
EXPORT mixerGetID() NOP_FUNC(103)
EXPORT mixerGetLineControlsA() NOP_FUNC(104)
EXPORT mixerGetLineControlsW() NOP_FUNC(105)
EXPORT mixerGetLineInfoA() NOP_FUNC(106)
EXPORT mixerGetLineInfoW() NOP_FUNC(107)
EXPORT mixerGetNumDevs() NOP_FUNC(108)
EXPORT mixerMessage() NOP_FUNC(109)
EXPORT mixerOpen() NOP_FUNC(110)
EXPORT mixerSetControlDetails() NOP_FUNC(111)
EXPORT mmDrvInstall() NOP_FUNC(112)
EXPORT mmGetCurrentTask() NOP_FUNC(113)
EXPORT mmTaskBlock() NOP_FUNC(114)
EXPORT mmTaskCreate() NOP_FUNC(115)
EXPORT mmTaskSignal() NOP_FUNC(116)
EXPORT mmTaskYield() NOP_FUNC(117)
EXPORT mmioAdvance() NOP_FUNC(118)
EXPORT mmioAscend() NOP_FUNC(119)
EXPORT mmioClose() NOP_FUNC(120)
EXPORT mmioCreateChunk() NOP_FUNC(121)
EXPORT mmioDescend() NOP_FUNC(122)
EXPORT mmioFlush() NOP_FUNC(123)
EXPORT mmioGetInfo() NOP_FUNC(124)
EXPORT mmioInstallIOProcA() NOP_FUNC(125)
EXPORT mmioInstallIOProcW() NOP_FUNC(126)
EXPORT mmioOpenA() NOP_FUNC(127)
EXPORT mmioOpenW() NOP_FUNC(128)
EXPORT mmioRead() NOP_FUNC(129)
EXPORT mmioRenameA() NOP_FUNC(130)
EXPORT mmioRenameW() NOP_FUNC(131)
EXPORT mmioSeek() NOP_FUNC(132)
EXPORT mmioSendMessage() NOP_FUNC(133)
EXPORT mmioSetBuffer() NOP_FUNC(134)
EXPORT mmioSetInfo() NOP_FUNC(135)
EXPORT mmioStringToFOURCCA() NOP_FUNC(136)
EXPORT mmioStringToFOURCCW() NOP_FUNC(137)
EXPORT mmioWrite() NOP_FUNC(138)
EXPORT mmsystemGetVersion() NOP_FUNC(139)
EXPORT mod32Message() NOP_FUNC(140)
EXPORT mxd32Message() NOP_FUNC(141)
EXPORT sndPlaySoundA() NOP_FUNC(142)
EXPORT sndPlaySoundW() NOP_FUNC(143)
EXPORT tid32Message() NOP_FUNC(144)
EXPORT timeBeginPeriod() NOP_FUNC(145)
EXPORT timeEndPeriod() NOP_FUNC(146)
EXPORT timeGetDevCaps() NOP_FUNC(147)
EXPORT timeGetSystemTime() NOP_FUNC(148)
EXPORT timeGetTime() NOP_FUNC(149)
EXPORT timeKillEvent() NOP_FUNC(150)
EXPORT timeSetEvent() NOP_FUNC(151)
EXPORT waveInAddBuffer() NOP_FUNC(152)
EXPORT waveInClose() NOP_FUNC(153)
EXPORT waveInGetDevCapsA() NOP_FUNC(154)
EXPORT waveInGetDevCapsW() NOP_FUNC(155)
EXPORT waveInGetErrorTextA() NOP_FUNC(156)
EXPORT waveInGetErrorTextW() NOP_FUNC(157)
EXPORT waveInGetID() NOP_FUNC(158)
EXPORT waveInGetNumDevs() NOP_FUNC(159)
EXPORT waveInGetPosition() NOP_FUNC(160)
EXPORT waveInMessage() NOP_FUNC(161)
EXPORT waveInOpen() NOP_FUNC(162)
EXPORT waveInPrepareHeader() NOP_FUNC(163)
EXPORT waveInReset() NOP_FUNC(164)
EXPORT waveInStart() NOP_FUNC(165)
EXPORT waveInStop() NOP_FUNC(166)
EXPORT waveInUnprepareHeader() NOP_FUNC(167)
EXPORT waveOutBreakLoop() NOP_FUNC(168)
EXPORT waveOutClose() NOP_FUNC(169)
EXPORT waveOutGetDevCapsA() NOP_FUNC(170)
EXPORT waveOutGetDevCapsW() NOP_FUNC(171)
EXPORT waveOutGetErrorTextA() NOP_FUNC(172)
EXPORT waveOutGetErrorTextW() NOP_FUNC(173)
EXPORT waveOutGetID() NOP_FUNC(174)
EXPORT waveOutGetNumDevs() NOP_FUNC(175)
EXPORT waveOutGetPitch() NOP_FUNC(176)
EXPORT waveOutGetPlaybackRate() NOP_FUNC(177)
EXPORT waveOutGetPosition() NOP_FUNC(178)
EXPORT waveOutGetVolume() NOP_FUNC(179)
EXPORT waveOutMessage() NOP_FUNC(180)
EXPORT waveOutOpen() NOP_FUNC(181)
EXPORT waveOutPause() NOP_FUNC(182)
EXPORT waveOutPrepareHeader() NOP_FUNC(183)
EXPORT waveOutReset() NOP_FUNC(184)
EXPORT waveOutRestart() NOP_FUNC(185)
EXPORT waveOutSetPitch() NOP_FUNC(186)
EXPORT waveOutSetPlaybackRate() NOP_FUNC(187)
EXPORT waveOutSetVolume() NOP_FUNC(188)
EXPORT waveOutUnprepareHeader() NOP_FUNC(189)
EXPORT waveOutWrite() NOP_FUNC(190)
EXPORT wid32Message() NOP_FUNC(191)
EXPORT wod32Message() NOP_FUNC(192)
// 
EXPORT IsInteractiveUserSession() NOP_FUNC(193)
EXPORT QueryActiveSession() NOP_FUNC(194)
EXPORT QueryUserToken() NOP_FUNC(195)
EXPORT RegisterUsertokenForNoWinlogon() NOP_FUNC(196)
EXPORT WTSCloseServer() NOP_FUNC(197)
EXPORT WTSConnectSessionA() NOP_FUNC(198)
EXPORT WTSConnectSessionW() NOP_FUNC(199)
EXPORT WTSCreateListenerA() NOP_FUNC(200)
EXPORT WTSCreateListenerW() NOP_FUNC(201)
EXPORT WTSDisconnectSession() NOP_FUNC(202)
EXPORT WTSEnableChildSessions() NOP_FUNC(203)
EXPORT WTSEnumerateListenersA() NOP_FUNC(204)
EXPORT WTSEnumerateListenersW() NOP_FUNC(205)
EXPORT WTSEnumerateProcessesA() NOP_FUNC(206)
EXPORT WTSEnumerateProcessesExA() NOP_FUNC(207)
EXPORT WTSEnumerateProcessesExW() NOP_FUNC(208)
EXPORT WTSEnumerateProcessesW() NOP_FUNC(209)
EXPORT WTSEnumerateServersA() NOP_FUNC(210)
EXPORT WTSEnumerateServersW() NOP_FUNC(211)
EXPORT WTSEnumerateSessionsA() NOP_FUNC(212)
EXPORT WTSEnumerateSessionsExA() NOP_FUNC(213)
EXPORT WTSEnumerateSessionsExW() NOP_FUNC(214)
EXPORT WTSEnumerateSessionsW() NOP_FUNC(215)
EXPORT WTSFreeMemory() NOP_FUNC(216)
EXPORT WTSFreeMemoryExA() NOP_FUNC(217)
EXPORT WTSFreeMemoryExW() NOP_FUNC(218)
EXPORT WTSGetChildSessionId() NOP_FUNC(219)
EXPORT WTSGetListenerSecurityA() NOP_FUNC(220)
EXPORT WTSGetListenerSecurityW() NOP_FUNC(221)
EXPORT WTSIsChildSessionsEnabled() NOP_FUNC(222)
EXPORT WTSLogoffSession() NOP_FUNC(223)
EXPORT WTSOpenServerA() NOP_FUNC(224)
EXPORT WTSOpenServerExA() NOP_FUNC(225)
EXPORT WTSOpenServerExW() NOP_FUNC(226)
EXPORT WTSOpenServerW() NOP_FUNC(227)
EXPORT WTSQueryListenerConfigA() NOP_FUNC(228)
EXPORT WTSQueryListenerConfigW() NOP_FUNC(229)
EXPORT WTSQuerySessionInformationA() NOP_FUNC(230)
EXPORT WTSQuerySessionInformationW() NOP_FUNC(231)
EXPORT WTSQueryUserConfigA() NOP_FUNC(232)
EXPORT WTSQueryUserConfigW() NOP_FUNC(233)
EXPORT WTSQueryUserToken() NOP_FUNC(234)
EXPORT WTSRegisterSessionNotification() NOP_FUNC(235)
EXPORT WTSRegisterSessionNotificationEx() NOP_FUNC(236)
EXPORT WTSSendMessageA() NOP_FUNC(237)
EXPORT WTSSendMessageW() NOP_FUNC(238)
EXPORT WTSSetListenerSecurityA() NOP_FUNC(239)
EXPORT WTSSetListenerSecurityW() NOP_FUNC(240)
EXPORT WTSSetRenderHint() NOP_FUNC(241)
EXPORT WTSSetSessionInformationA() NOP_FUNC(242)
EXPORT WTSSetSessionInformationW() NOP_FUNC(243)
EXPORT WTSSetUserConfigA() NOP_FUNC(244)
EXPORT WTSSetUserConfigW() NOP_FUNC(245)
EXPORT WTSShutdownSystem() NOP_FUNC(246)
EXPORT WTSStartRemoteControlSessionA() NOP_FUNC(247)
EXPORT WTSStartRemoteControlSessionW() NOP_FUNC(248)
EXPORT WTSStopRemoteControlSession() NOP_FUNC(249)
EXPORT WTSTerminateProcess() NOP_FUNC(250)
EXPORT WTSUnRegisterSessionNotification() NOP_FUNC(251)
EXPORT WTSUnRegisterSessionNotificationEx() NOP_FUNC(252)
EXPORT WTSVirtualChannelClose() NOP_FUNC(253)
EXPORT WTSVirtualChannelOpen() NOP_FUNC(254)
EXPORT WTSVirtualChannelOpenEx() NOP_FUNC(255)
EXPORT WTSVirtualChannelPurgeInput() NOP_FUNC(256)
EXPORT WTSVirtualChannelPurgeOutput() NOP_FUNC(257)
EXPORT WTSVirtualChannelQuery() NOP_FUNC(258)
EXPORT WTSVirtualChannelRead() NOP_FUNC(259)
EXPORT WTSVirtualChannelWrite() NOP_FUNC(260)
EXPORT WTSWaitSystemEvent() NOP_FUNC(261)

bool WriteMemory(PBYTE BaseAddress, PBYTE Buffer, DWORD nSize)
{
    DWORD ProtectFlag = 0;
    if (VirtualProtectEx(GetCurrentProcess(), BaseAddress, nSize, PAGE_EXECUTE_READWRITE, &ProtectFlag))
    {
        memcpy(BaseAddress, Buffer, nSize);
        FlushInstructionCache(GetCurrentProcess(), BaseAddress, nSize);
        VirtualProtectEx(GetCurrentProcess(), BaseAddress, nSize, ProtectFlag, &ProtectFlag);
        return true;
    }
    return false;
}

// 还原导出函数
void InstallJMP(PBYTE BaseAddress, MWORD Function)
{
#ifdef _WIN64
    BYTE move[] = {0x48, 0xB8};//move rax,xxL);
    BYTE jump[] = {0xFF, 0xE0};//jmp rax

    WriteMemory(BaseAddress, move, sizeof(move));
    BaseAddress += sizeof(move);

    WriteMemory(BaseAddress, (PBYTE)&Function, sizeof(MWORD));
    BaseAddress += sizeof(MWORD);

    WriteMemory(BaseAddress, jump, sizeof(jump));
#else
    BYTE jump[] = {0xE9};
    WriteMemory(BaseAddress, jump, sizeof(jump));
    BaseAddress += sizeof(jump);

    MWORD offset = Function - (MWORD)BaseAddress - 4;
    WriteMemory(BaseAddress, (PBYTE)&offset, sizeof(offset));
#endif // _WIN64
}

// 加载系统dll
void LoadSysDll(HINSTANCE hModule)
{
    PBYTE pImageBase = (PBYTE)hModule;
    PIMAGE_DOS_HEADER pimDH = (PIMAGE_DOS_HEADER)pImageBase;
    if(pimDH->e_magic==IMAGE_DOS_SIGNATURE)
    {
        PIMAGE_NT_HEADERS pimNH = (PIMAGE_NT_HEADERS)(pImageBase + pimDH->e_lfanew);
        if(pimNH->Signature==IMAGE_NT_SIGNATURE)
        {
            PIMAGE_EXPORT_DIRECTORY pimExD = (PIMAGE_EXPORT_DIRECTORY)(pImageBase + pimNH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
            DWORD*  pName       = (DWORD*)(pImageBase + pimExD->AddressOfNames);
            DWORD*  pFunction   = (DWORD*)(pImageBase + pimExD->AddressOfFunctions);

            wchar_t szSysDirectory[MAX_PATH + 1];
            GetSystemDirectory(szSysDirectory, MAX_PATH);

            wchar_t szWinmmDLL[MAX_PATH + 1];
            lstrcpy(szWinmmDLL, szSysDirectory);
            lstrcat(szWinmmDLL, TEXT("\\winmm.dll"));
            
            wchar_t szWtsapi32DLL[MAX_PATH + 1];
            lstrcpy(szWtsapi32DLL, szSysDirectory);
            lstrcat(szWtsapi32DLL, TEXT("\\wtsapi32.dll"));
            
            HINSTANCE winmm_module = LoadLibrary(szWinmmDLL);
            HINSTANCE wtsapi32_module = LoadLibrary(szWtsapi32DLL);
            for (size_t i = 0; i < pimExD->NumberOfNames; i++)
            {
                MWORD Original = (MWORD)GetProcAddress(winmm_module, (char*)(pImageBase + pName[i]) );
                if(Original)
                {
                    InstallJMP(pImageBase + pFunction[i], Original);
                }
                else
                {
                    Original = (MWORD)GetProcAddress(wtsapi32_module, (char*)(pImageBase + pName[i]) );
                    if(Original)
                    {
                        InstallJMP(pImageBase + pFunction[i], Original);
                    }
                }
            }
        }
    }
}

