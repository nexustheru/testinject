#include <iostream>
#include <Windows.h>
#include <cstdio>
#include <tlhelp32.h>
#include <string>
using namespace std;
//15892

bool Inject(const char* dllPath, LPCTSTR procName)
{
    HANDLE Process;
    LPVOID Memory;
    LPVOID LoadLibrary;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (Process32First(snapshot, &entry) == TRUE)
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
            if (!lstrcmpi(entry.szExeFile, procName) == 0)
            {
                Process = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, 0, entry.th32ProcessID);
                
                LoadLibrary = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");
                Memory = (LPVOID)VirtualAllocEx(Process, NULL, strlen(dllPath) + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
                WriteProcessMemory(Process, (LPVOID)Memory, dllPath, strlen(dllPath) + 1, NULL);
                CreateRemoteThread(Process, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibrary, (LPVOID)Memory, NULL, NULL);
                CloseHandle(Process);
                VirtualFreeEx(Process, (LPVOID)Memory, 0, MEM_RELEASE);
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}
int main()
{
    const char* dllPath = "dllinject.dll";
    const char* pidPath = "notepad++.exe";
    bool res = Inject(dllPath, L"notepad++.exe");
    if (res)
    {
        std::cout << "dll injected" << std::endl;
    }
    else
    {
        std::cout << "dll not injected" << std::endl;
    }
    system("PAUSE");
    return 0;
}