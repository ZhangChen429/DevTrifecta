#include "WriteTool.h"

#include <fstream>
#include <iostream>


WriteTool::WriteTool()
{
    
}

// 核心功能：将文件列表写入 CSV 文件（Excel 可直接打开）
bool WriteTool::write_to_csv(const std::vector<std::string>& files, const std::string& csv_path) {
    // 创建文件输出流（std::ofstream），指定路径和打开模式
    std::ofstream csv_file(csv_path, std::ios::out | std::ios::trunc);
    if (!csv_file.is_open()) {  // 检查文件是否成功打开
        std::cerr << "错误：无法创建/打开 CSV 文件 -> " << csv_path << std::endl;
        return false;
    }

    // 1. 写入 CSV 表头（第一行：序号、文件名称、完整路径）
    csv_file << "序号,文件名称,完整路径" << std::endl;  // CSV 用逗号分隔列

    // 2. 写入文件列表数据
    for (size_t i = 0; i < files.size(); ++i) {
        fs::path file_path(files[i]);
        std::string filename = file_path.filename().string();  // 文件名（含后缀）
        std::string full_path = file_path.string();            // 完整路径

        // CSV 规则：若内容含逗号/引号，需用双引号包裹（避免列错乱）
        // 简化处理：直接给文件名和路径加双引号（兼容所有情况）
        csv_file << (i + 1)  // 序号（从 1 开始）
                 << "," << "\"" << filename << "\""  // 文件名（双引号包裹）
                 << "," << "\"" << full_path << "\""  // 完整路径（双引号包裹）
                 << std::endl;  // 换行
    }

    // 3. 关闭文件流（自动刷新数据）
    csv_file.close();
    std::cout << "CSV 文件已成功生成：" << csv_path << std::endl;
    return true;
}