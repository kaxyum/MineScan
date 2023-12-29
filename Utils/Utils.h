#ifndef CHECKING_UTILS_H
#define CHECKING_UTILS_H

#include <iostream>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

class Utils {
public:
    std::vector<std::string> searchFiles(const std::string& keyword, const std::string& path);
    std::string getFileUsername();
    int sendWebhook(const std::string& cheat, const std::vector<std::string>& files);
    std::string getXboxUsername();
    std::string getLastRecycleBinEdit();
};


#endif //CHECKING_UTILS_H
