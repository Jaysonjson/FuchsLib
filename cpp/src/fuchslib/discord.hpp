/* Jayson */
#pragma once

//TODO: TIDY
#undef FUCHSLIB_BUILD_DISCORD
#ifdef FUCHSLIB_BUILD_DISCORD
#define CURL_STATICLIB

#include <fuchslib/shorter.hpp>
#include <curl/curl.h>
#include <discord/discord.h>
#include <memory>
#include <csignal>
#include <thread>
#include <chrono>
#include <fstream>
#include "json.hpp"

inline size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*)ptr, size * nmemb);
    return size * nmemb;
}

inline size_t callbackfunction(void* ptr, size_t size, size_t nmemb, void* userdata) {
    FILE* stream = (FILE*)userdata;
    if (!stream) return 0;
    size_t written = fwrite((FILE*)ptr, size, nmemb, stream);
    return written;
}


namespace Fuchs::Discord {

    namespace {
        volatile bool interrupted = false;
    }

    class Discord {
        discord::User currentUser_;
        discord::Core* core_;
        discord::Activity activity_{};
        bool created_ = false;
        bool refreshActivity_ = false;
        std::thread dThread;
    public:

        discord::Core* getCore() { return this->core_; }
        discord::User& getCurrentUser() { return this->currentUser_; }
        discord::Activity& getActivity() { return this->activity_; }
        bool isCreated() { return this->created_; }
        void interrupt() { interrupted = true; }

        void refreshActivity() { this->refreshActivity_ = true; }

        void start(discord::ClientId clientId, uintmax flags) {
            if (!dThread.joinable()) dThread = std::thread{ [this, clientId, flags] {
                this->create(clientId, flags);
            } };
        }

        static nlohmann::json getUserRawData(uintmax id, const std::string& token) {
            return getData("https://discord.com/api/users/" + std::to_string(id));
        }

        static nlohmann::json getGuildRawData(uintmax id, const string& token) {
            return getData("https://discord.com/api/guilds/" + std::to_string(id));
        }

        static nlohmann::json getData(const std::string& apiUrl, const std::string& token) {
            auto curl = curl_easy_init();
            if (curl) {
                cstring url = apiUrl.c_str();
                curl_easy_setopt(curl, CURLOPT_URL, url);
                std::string response_string;
                curl_slist* headers = NULL;
                headers = curl_slist_append(headers, string{ "Authorization: Bot " + token }.c_str());
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
                curl_easy_perform(curl);
                curl_easy_cleanup(curl);
                return nlohmann::json::parse(response_string);
            }
            return nlohmann::json{ "{}" };
        }

        static std::string getAvatarId(const nlohmann::json& rawData) {
            if (!rawData.contains("avatar")) return "";
            return rawData.at("avatar");
        }

        static std::string getAvatarId(uintmax id, const std::string& token) {
            nlohmann::json js = getUserRawData(id, token);
            if (!js.contains("avatar")) return "";
            return js.at("avatar");
        }

        static std::string getAvatarURL(const nlohmann::json& rawData, uint16 size) {
            std::string avatar = getAvatarId(rawData);
            if (avatar.empty()) return "";
            std::string id = rawData.at("id");
            return std::string{ "https://cdn.discordapp.com/avatars/" + id + "/" + avatar + ".webp?size=" + std::to_string(size) };
        }

        static std::string getAvatarURL(uintmax id, uint16 size, const std::string& token) {
            std::string avatar = getAvatarId(id, token);
            if (avatar.empty()) return "";
            return std::string{ "https://cdn.discordapp.com/avatars/" + std::to_string(id) + "/" + avatar + ".webp?size=" + std::to_string(size) };
        }

        static bool saveAvatar(uintmax id, const std::string& file, uint16 size, const std::string& token) {
            return saveAvatar(file, getAvatarURL(id, size, token));
        }

        static bool saveAvatar(const nlohmann::json& rawUserData, const std::string& file, uint16 size) {
            return saveAvatar(file, getAvatarURL(rawUserData, size));
        }


        static bool saveAvatar(const std::string& file, const std::string& urlStr) {
            FILE* fp = fopen(file.c_str(), "wb");
            if (!fp) return false;
            if (urlStr.empty()) return false;
            const char* url = urlStr.c_str();
            auto curl = curl_easy_init();
            if (curl) {
                curl_easy_setopt(curl, CURLOPT_URL, url);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callbackfunction);
                curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

                CURLcode rc = curl_easy_perform(curl);
                if (rc) {
                    return false;
                }

                long res_code = 0;
                curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &res_code);
                if (!((res_code == 200 || res_code == 201) && rc != CURLE_ABORTED_BY_CALLBACK)) {
                    return false;
                }
                curl_easy_cleanup(curl);
            }
            fflush(fp);
            fclose(fp);
            return true;
        }

    private:
        discord::Result updateActivity() { discord::Result res; getCore()->ActivityManager().UpdateActivity(getActivity(), [&res](discord::Result result) { res = result; }); return res; }

        discord::Result createCore(discord::ClientId clientId, uintmax flags) {
            discord::Result result = discord::Core::Create(clientId, flags, &core_);
            if (!getCore()) {
                //Con::log("Could not instantiate discord core -> " + static_cast<int>(result), cout);
                MOTOR_LOG("Could not instantiate discord core -> " + static_cast<int>(result));
            }
            return result;
        }

        void connectUser() { getCore()->UserManager().OnCurrentUserUpdate.Connect([this]() { getCore()->UserManager().GetCurrentUser(&currentUser_); }); }

        void create(discord::ClientId clientId, uintmax flags) {
            if (createCore(clientId, flags) == discord::Result::Ok) {
                connectUser();
                this->created_ = true;
                signal(SIGINT, [](int) { interrupted = true; });
                do {
                    getCore()->RunCallbacks();
                    if (refreshActivity_) updateActivity(); refreshActivity_ = false;
                    std::this_thread::sleep_for(std::chrono::milliseconds(120));
                } while (!interrupted);
            }
        }
    };
}

#endif