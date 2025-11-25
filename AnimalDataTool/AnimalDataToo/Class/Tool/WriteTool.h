#pragma once
#include <string>
#include <vector>
#include <filesystem>  // C++17 原生文件系统库
namespace fs = std::filesystem;  // 简化命名空间
class WriteTool
{
public:
    WriteTool();
    bool write_to_csv(const std::vector<std::string>& files, const std::string& csv_path);
};
