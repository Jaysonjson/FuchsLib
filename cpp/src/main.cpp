#include "fuchslib/fuchslib.hpp"
#include "fuchslib/shorter.hpp"
#include "fuchslib/windows/regkey.hpp"
#include "fuchslib/utility.hpp"
#include "fuchslib/ruid.hpp"
#include "fuchslib/stack.hpp"
#include "fuchslib/key.hpp"
#include "fuchslib/memory/memoryaddress.hpp"
#include "fuchslib/memory/memoryprocess.hpp"
#include <iostream>


OSPlatform Fuchs::osPlatform{};

#ifdef LINUX
	utsname Fuchs::unameData{};
	//struct sysinfo sysInfo;
#endif

#define FUCHSLIB_TESTING          

#ifdef FUCHSLIB_TESTING

	void regKeys() {
		#ifdef WINDOWS
		Fuchs::Reg::HKey fuchsLibKey{ HKEY_CURRENT_USER, "Software\\FuchsLib" };
		Fuchs::Reg::DWORDKey testKey{ &fuchsLibKey, "test" };
		Fuchs::Reg::STRINGKey strKey{ &fuchsLibKey, "shit" };
		fuchsLibKey.create();
		testKey.set(rand());
		strKey.set("Funny: " + std::to_string(rand()));
		strKey.refresh();
		testKey.refresh();
		std::cout << testKey.get() << std::endl;
		std::cout << strKey.get() << std::endl;
		#endif
	}

	void ruid() {
		std::cout << "RUID" << std::endl;
		std::string ruid = Fuchs::generateRUID();
		std::cout << ruid << "\n" << Fuchs::getRUIDPrefix(ruid) << "\n" << Fuchs::getRUIDSuffix(ruid) << "\n" << Fuchs::ruidPatternMatches(ruid) << std::endl;
		Fuchs::RUIDC ruidC{};
		ruidC.generate();
		std::cout << ruidC << std::endl;
		std::cout << "RUID - END" << std::endl;
	}

	void explorerAndBrowser() {
		std::cout << "opening browser https://google.com" << std::endl;
		Fuchs::Utility::openURLInBrowser("https://google.com");
		std::cout << "opening executable folder: " << Fuchs::Utility::getExecutableDirectory() << std::endl;
		std::cout << "executable: " << Fuchs::Utility::getExecutableFile() << std::endl;
		Fuchs::Utility::openDirectoryInFileManager(Fuchs::Utility::getExecutableDirectory());
	}
	
	void osName() {
		#ifdef LINUX
			std::cout << Fuchs::Utility::getOSName() << " " << Fuchs::Utility::getArchitecture() << " " << Fuchs::Utility::getKernelRelease() << std::endl;
		#endif
	}
	
	void fuchsKey() {
		Fuchs::FuchsKey key{ "test_key", "fuchslib" };
		std::cout << key << std::endl;
		std::cout << key[Fuchs::FuchsKey::AR_KEY] << std::endl;
		std::cout << key[Fuchs::FuchsKey::AR_NAMESPACE] << std::endl;
	}

	void memory() {
		MemoryProcess process{ "DATA.exe" };
		process.attach();
		TCHAR moduleName[10] = TEXT("DATA.exe");
		DWORD gameBase = process.getModuleBaseAddress(moduleName);

		MemoryAddress<int> soulsMemory{&process, gameBase, 0x00FDED50, {0x8,0x8C} };
		MemoryAddress<int> healthMemory{&process, gameBase, 0x00FE3C60, { 0x3C,0x2D4 } };
		MemoryAddress<int> currentStaminaMemory{&process, gameBase, 0x0020394C, { 0xBCC } };
		MemoryAddress<int> maxStaminaMemory{&process, gameBase, 0x00FDED50, { 0x8, 0x30 } };
		MemoryAddress<int> vitalityLevel{&process, gameBase, 0x00B4AC04, { 0x910 } };
		MemoryAddress<std::string> testAddress{&process, gameBase, 0x0, {0x1} };

		std::cout << "BASE: " + std::to_string(gameBase) << std::endl;
		std::cout << "Souladdress: " + std::to_string(soulsMemory.address()) << std::endl;
		std::cout << "Healthaddress: " + std::to_string(healthMemory.address()) << std::endl;
		std::cout << "Souls: " + std::to_string(soulsMemory.read()) << std::endl;
		std::cout << "Health: " + std::to_string(healthMemory.read()) << std::endl;
		soulsMemory.write(500000);
		healthMemory.write(500);
		std::cout << "/NEW/" << std::endl;
		std::cout << "Souls: " + std::to_string(soulsMemory.read()) << std::endl;
		std::cout << "Health: " + std::to_string(healthMemory.read()) << std::endl;
		process.close();
	}

#endif

int main() {
	#ifdef LINUX
		uname(&Fuchs::unameData);
		sysinfo(&sysInfo);
		Fuchs::osPlatform.linux().environment = Fuchs::Utility::getDesktopEnvironment();
	#endif
	#ifdef FUCHSLIB_TESTING
	Fuchs::Stack<uint16, 64, 0> test{};
	test.setAmount(3);
	std::cout << test.getAmount()  << "\n";
	test.referenceAmount() *= 2;
	std::cout << test.getAmount() << std::endl;
	Fuchs::Stack<uint8, 5, 0> test2{};
	Fuchs::Stack<uint16, 995, 0> test3{};	
	Fuchs::Stack<uint8, 0, 50> t{};
	t.setAmount(67);
		regKeys();	
		osName();
		ruid();
		//explorerAndBrowser();
		fuchsKey();
		std::cin.get();
	#endif
		return 0;
}