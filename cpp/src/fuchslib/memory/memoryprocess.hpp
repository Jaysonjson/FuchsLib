#pragma once

#include "fuchsmemory.hpp"

#ifdef _WIN32
#include "windows.h"
#include "TlHelp32.h"
//#include "atlstr.h"
#include "string"

class MemoryProcess {
	FUCHS_HANDLE handle_ = nullptr;
	FUCHS_PID id_ = 0;
	std::string exe_ = "";
public:

	MemoryProcess(std::string procName): exe_(procName) {}
	~MemoryProcess() {
		close();
	}

public:

	bool attach();

	FUCHS_HANDLE handle() {
		return this->handle_;
	}

	FUCHS_PID id() {
		return this->id_;
	}

	/* Close the Handle */
	void close() {
		CloseHandle(handle());
	}

	/* Kill the Process */
	void terminate(UINT exitCode = 1) {
		TerminateProcess(handle(), exitCode);
		close();
	}

	FUCHS_WORD getModuleBaseAddress(TCHAR* lpszModuleName);
};
#endif