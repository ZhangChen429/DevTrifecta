#pragma once
#include <vector>
#include <xstring>
#include <filesystem>  // C++17 原生文件系统库
namespace fs = std::filesystem;  // 简化命名空间
class FindAnim
{
public:
    FindAnim();
    bool hasAnimalSuffix(const std::string& filename);
    std::vector<std::string> find_animal_files(const std::string& target_folder, bool recursive);
};
