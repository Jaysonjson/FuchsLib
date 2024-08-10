#pragma once

#include "vector"
#include "memoryprocess.hpp"
#include "iomanip"
#include <sstream>

template <typename T = int>
class MemoryAddress {
	FUCHS_WORD gameOffset_ = 0x0;
	std::vector<FUCHS_WORD> offsets_{};
	FUCHS_WORD address_;
	MemoryProcess* process_ = nullptr;
public:
	MemoryAddress(MemoryProcess* process, FUCHS_WORD gameBase, FUCHS_WORD gameOffset, const std::vector<FUCHS_WORD>& offsets) {
		FUCHS_WORD baseAddress = NULL;
		this->process_ = process;
		ReadProcessMemory(process_->handle(), (LPVOID)(gameBase + gameOffset), &baseAddress, sizeof(baseAddress), NULL);
		address_ = baseAddress;
		if (offsets.size() > 0) {
			for (int i = 0; i < offsets.size() - 1; i++) {
				ReadProcessMemory(process_->handle(), (LPVOID)(address_ + offsets.at(i)), &address_, sizeof(address_), NULL);
			}
			address_ += offsets.at(offsets.size() - 1);
		}
		this->gameOffset_ = gameOffset;
		this->offsets_ = offsets;
	}

public:

	/* Return Size in Bytes of Data Type*/
	size_t dataSize() {
		return sizeof(T);
	}

	/* Write new Data to the Address */
	void write(T valToWrite) {
		if(this->process_->handle() != nullptr) {
			FUCHS_WORD oldprotect;
			WriteProcessMemory(process_->handle(), (LPVOID)address_, &valToWrite, dataSize(), 0);
		}
	}

	void writeProtected(T valToWrite) {
		FUCHS_WORD oldprotect;
		VirtualProtectEx(this->process_->handle(), (LPVOID)address_, dataSize(), PAGE_EXECUTE_READWRITE, &oldprotect);
		WriteProcessMemory(process_->handle(), (LPVOID)address_, &valToWrite, dataSize(), 0);
		VirtualProtectEx(this->process_->handle(), (LPVOID)address_, dataSize(), oldprotect, &oldprotect);
	}

	/* Read Data from the Address */
	T read() {
		try {
			T rpmBuffer;
			ReadProcessMemory(process_->handle(), (LPVOID)address_, &rpmBuffer, dataSize(), 0);
			return rpmBuffer;
		}
		catch (std::exception e) {
			T v = T();
			return v;
		}
	}

#define _CRT_SECURE_NO_WARNINGS
	void readString(char* dstArray) {
		uintptr_t sizeoffset = 0x14;
		if (sizeof(int*) == 8)
		{
			sizeoffset = 0x18;
		}
		uintptr_t arraySize;
		ReadProcessMemory(this->process_->handle(), (BYTE*)(address_ + sizeoffset), &arraySize, sizeof(arraySize), 0);
		if (arraySize > 15)
		{
			uintptr_t addrOfCharArray;
			ReadProcessMemory(this->process_->handle(), (BYTE*)(address_ + sizeof(void*)), &addrOfCharArray, sizeof(void*), 0);
			char buffer[500];
			ReadProcessMemory(this->process_->handle(), (BYTE*)(addrOfCharArray), &buffer, arraySize + 1, 0);
			memcpy(dstArray, &buffer, strlen(buffer) + 1);
		}
		else
		{
			ReadProcessMemory(this->process_->handle(), (BYTE*)(address_ + sizeof(void*)), dstArray, arraySize, 0);
		}

	}

	/* Return the Address as DWORD */
	FUCHS_WORD address() {
		return this->address_;
	}

	std::string addressAsHex() {
		std::stringstream stream;
		stream << std::hex << address();
		return stream.str();
	}

	/* Return the BaseOffset as DWORD */
	FUCHS_WORD baseOffset() {
		return this->gameOffset_;
	}

	std::vector<FUCHS_WORD> offsets() {
		return this->offsets_;
	}

	MemoryProcess* process() {
		return this->process_;
	}

	void forceSetAddress(FUCHS_WORD address) {
		this->address_ = address;
	}

	T type() {
		return T;
	}
};