#include "fuchslib/utility.hpp"

#ifdef LINUX
#include <sys/resource.h>
#endif

namespace Fuchs::Utility {
	nd uintmax injectLibrary(const std::string& path, void* data) {
		return 0;
	}

	//TODO: WINDOWS
	std::string getCMDOutput(const std::string& cmd) {
#ifdef UNIX
    FILE *fp;
    char output[1035];
    fp = popen(cmd.c_str(), "r");
    if (fp == nullptr) {
       // qDebug() << std::string("Failed to run command " + call).c_str();
        exit(1);
    }
    std::string out;
    while (fgets(output, sizeof(output), fp) != nullptr) {
      out += output;
    }

    pclose(fp);
    //return fgets(output, sizeof(output), fp);
    return out;
#endif

#ifdef WINDOWS
	std::vector<char> buffer{};
		std::string result;
		FILE* pipe(_popen(cmd.c_str(), "rt"));
		if (!pipe) {
			return "";
		}
		while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
			result += buffer.data();
		}
		return std::move(result);
#endif

		return "";
	}

	//TODO: WINDOWS
	std::string getOSName() {
#ifdef LINUX
		std::string osData = getCMDOutput("cat /etc/os-release");
		if (osData.contains("PRETTY_NAME=")) {
			std::string buffer = osData.substr(osData.find("PRETTY_NAME=") + 13);
			return buffer.substr(0, buffer.find_first_of('"'));
		}
#endif
		return "";
	}

#ifdef LINUX
	int setResourceLimit(uint8 megabytes) {
		const rlim_t kStackSize = megabytes * 1024 * 1024;
		struct rlimit rl {};
		int result;

		result = getrlimit(RLIMIT_STACK, &rl);
		if (result == 0) {
			if (rl.rlim_cur < kStackSize) {
				rl.rlim_cur = kStackSize;
				result = setrlimit(RLIMIT_STACK, &rl);
			}
		}
		return result;
	}

	Data::LINUX_DESKTOP_ENVIRONMENT getDesktopEnvironment() {
		std::string output = getCMDOutput("ls /usr/bin/*session");
		if (output.contains("plasma_session")) return Data::LINUX_DESKTOP_ENVIRONMENT::PLASMA;
		if (output.contains("gnome-session")) return Data::LINUX_DESKTOP_ENVIRONMENT::GNOME;
		if (output.contains("mate-session")) return Data::LINUX_DESKTOP_ENVIRONMENT::MATE;
		if (output.contains("lxsession")) return Data::LINUX_DESKTOP_ENVIRONMENT::LXDE;
		if (output.contains("icewm-session")) return Data::LINUX_DESKTOP_ENVIRONMENT::JWM;
		if (output.contains("xfce4-session")) return Data::LINUX_DESKTOP_ENVIRONMENT::XFCE4;
		return Data::LINUX_DESKTOP_ENVIRONMENT::UNKNOWN;
	}

#endif
}