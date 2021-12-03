#include <windows.h>
#include <stdio.h>
#include <iostream>
# define DLLEXPORT __declspec (dllexport)

DLLEXPORT void test()
{
    MessageBoxA(NULL, "HELLO THERE", "From Notepad", NULL);
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: test(); break;
    case DLL_THREAD_ATTACH: test(); break;
    case DLL_THREAD_DETACH: ; break;
    case DLL_PROCESS_DETACH: ; break;
    }
    return TRUE;
}