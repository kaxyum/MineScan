#include "Utils.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <fstream>

namespace fs = std::filesystem;

std::vector<std::string> Utils::searchFiles(const std::string& keyword, const std::string& path)
{
    std::vector<std::string> files;
    for (const auto &entry: fs::recursive_directory_iterator(path))
    {
        if (entry.is_regular_file())
        {
            std::string filename = entry.path().filename().string();

            std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);

            std::string lowercaseKeyword = keyword;
            std::transform(lowercaseKeyword.begin(), lowercaseKeyword.end(), lowercaseKeyword.begin(), ::tolower);

            if (filename.find(lowercaseKeyword) != std::string::npos)
            {
                files.push_back(entry.path().string());
            }
        }
    }

    return files;
}

std::string Utils::getLastRecycleBinEdit()
{
    std::string lastedit;
    const std::string& trashPath = "C:\\$Recycle.Bin";

    FILETIME mostRecentTime = {0};

    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = FindFirstFileA((trashPath + "\\*").c_str(), &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (strcmp(findFileData.cFileName, ".") != 0 && strcmp(findFileData.cFileName, "..") != 0) {
                FILETIME fileTime = findFileData.ftLastWriteTime;
                if (CompareFileTime(&fileTime, &mostRecentTime) > 0) {
                    mostRecentTime = fileTime;
                }
            }
        } while (FindNextFileA(hFind, &findFileData) != 0);

        FindClose(hFind);
    }

    if (mostRecentTime.dwLowDateTime != 0 || mostRecentTime.dwHighDateTime != 0) {
        SYSTEMTIME stUTC, stLocal;
        FileTimeToSystemTime(&mostRecentTime, &stUTC);
        SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

        lastedit = std::to_string(stLocal.wYear) + "/" + std::to_string(stLocal.wMonth) + "/" + std::to_string(stLocal.wDay) + " " + std::to_string(stLocal.wHour) + ":" + std::to_string(stLocal.wMinute) + ":" + std::to_string(stLocal.wSecond);
    }
    return lastedit;
}

std::string Utils::getFileUsername()
{
    std::string username;
    for (auto it = std::filesystem::current_path().begin(); it != std::filesystem::current_path().end(); ++it) {
        if (*it == "Users" && std::next(it) != std::filesystem::current_path().end()) {
            username = std::next(it)->string();
            break;
        }
    }
    return username;
}

std::string Utils::getXboxUsername()
{
    Utils utils;
    std::ifstream file("C:\\Users\\" + utils.getFileUsername() + "\\AppData\\Roaming\\.minecraft_bedrock\\user_profile.json");
    std::string jsonString(
            (std::istreambuf_iterator<char>(file)),
            (std::istreambuf_iterator<char>())
    );
    file.close();

    size_t namePos = jsonString.find("\"Name\"");
    size_t valueStart = jsonString.find("\"", namePos + 7) + 1;
    size_t valueEnd = jsonString.find("\"", valueStart);

    std::string username = jsonString.substr(valueStart, valueEnd - valueStart);

    return username;
}

int Utils::sendWebhook(const std::string& cheat, const std::vector<std::string>& files)
{
    std::string url = "webhook-url";
    const std::string cmd_1 = "curl -i -H \"Accept: application/json\" -H \"Content-Type:application/json\" -X POST --data \"{\\\"content\\\": \\\"";
    const std::string cmd_2 = "\\\"}\" ";
    std::string filesString;
    for (const auto& file : files) {
        filesString += file + ", ";
    }
    std::string result;
    for (char c : filesString) {
        if (c == '\\') {
            result += "\\\\";
        } else {
            result += c;
        }
    }
    Utils utils;
    std::string command =  cmd_1 + "**Username:** " + utils.getXboxUsername() +"                                                                                                                                                                                                                                                                                                        **Recycle Bin:** Edited the " + utils.getLastRecycleBinEdit() + "                                                                                                                                                                                                                                                          __**Horion Detected**__                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 **Files:**                                                                                                                                                                                                                                                                                                                                                   " + result + cmd_2 + url;
    command.insert(0, "/C ");

    SHELLEXECUTEINFOA ShExecInfo = { 0 };
    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.hwnd = NULL;
    ShExecInfo.lpVerb = NULL;
    ShExecInfo.lpFile = "cmd.exe";
    ShExecInfo.lpParameters = command.c_str();
    ShExecInfo.lpDirectory = NULL;
    ShExecInfo.nShow = SW_HIDE;
    ShExecInfo.hInstApp = NULL;

    if (ShellExecuteExA(&ShExecInfo) == FALSE)
        return -1;

    WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

    DWORD rv;
    GetExitCodeProcess(ShExecInfo.hProcess, &rv);
    CloseHandle(ShExecInfo.hProcess);

    return rv;
}
