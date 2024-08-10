#include "fuchslib/memory/memoryprocess.hpp"
#include "iostream"

#ifdef _WIN32
#include "atlstr.h"

std::string utf8_encode(const std::wstring& wstr) {
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

bool MemoryProcess::attach() {
	PROCESSENTRY32 procEntry;

	procEntry.dwSize = sizeof(PROCESSENTRY32);
	FUCHS_HANDLE hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcSnap == INVALID_HANDLE_VALUE) return false;

	while (Process32Next(hProcSnap, &procEntry)) {
		//std::wstring wide_string = std::wstring(procEntry.szExeFile);
		//if (!strcmp(exe_.c_str(), utf8_encode(wide_string).c_str())) {
		if (!strcmp(exe_.c_str(), procEntry.szExeFile)) {
			std::cout << "Found process " << procEntry.szExeFile << " with ID " << procEntry.th32ProcessID << std::endl;
			handle_ = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procEntry.th32ProcessID);
			id_ = procEntry.th32ProcessID;
			if (handle_ == NULL) return false;
			CloseHandle(hProcSnap);
			return true;
		}
	}
}

FUCHS_WORD MemoryProcess::getModuleBaseAddress(TCHAR* lpszModuleName) {
	FUCHS_WORD dwModuleBaseAddress = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, id());
	MODULEENTRY32 ModuleEntry32 = { 0 };
	ModuleEntry32.dwSize = sizeof(MODULEENTRY32);
	if (Module32First(hSnapshot, &ModuleEntry32)) {
		do {
			if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0) {
				dwModuleBaseAddress = (FUCHS_WORD)ModuleEntry32.modBaseAddr;
				break;
			}
		} while (Module32Next(hSnapshot, &ModuleEntry32));
	}
	CloseHandle(hSnapshot);
	return dwModuleBaseAddress;
}  
#endif