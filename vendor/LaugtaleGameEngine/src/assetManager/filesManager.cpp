#include <filesystem>

#include "filesManager.h"
#include "filesFactory.h"

namespace LTE
{
    file *createFile(const std::string& fileType){ return NULL; }
    
    std::string fileManager::filesBasePath = "./res/";
    router<file*> fileManager::filesTree;
    
    void fileManager::init()
    {
        for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(filesBasePath))
        {
            if (!dirEntry.is_directory() && std::string(dirEntry.path().extension()) != "")
            {
                filesTree.addRoute(std::string(dirEntry.path().relative_path()).erase(0, 2).append("/"));

                std::string fileType = std::string(dirEntry.path().extension()).erase(0, 1);
                std::string filePath = std::string(dirEntry.path()).erase(0, 2);

                file *fileInDirEntry = filesFactory::createFile(filePath, fileType);
                fileInDirEntry->loadFileData();
                filesTree.addValue(filePath, fileInDirEntry);
            }
        }
    }

    void fileManager::close()
    {
        filesTree.clearAndDelete();
    }

    file *fileManager::getFile(const std::string& filePath)
    {
        return filesTree[filePath];
    }
}