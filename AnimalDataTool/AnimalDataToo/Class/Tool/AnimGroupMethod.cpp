#include "AnimGroupMethod.h"

#include <iostream>

#include "FindAnim.h"
#include "WriteTool.h"

AnimGroupMethod::AnimGroupMethod()
{
}

void AnimGroupMethod::AnimSCVCreate(std::string Infolder, bool recursive,std::string csv_output_path)
{
    FindAnim* find_anim=new FindAnim;
    std::vector<std::string> files =find_anim-> find_animal_files(Infolder,recursive);
    // 若需递归查找子目录，调用：find_animal_files(folder, true)
    delete find_anim;  // 释放内存
    
    WriteTool* write_tool = new WriteTool;
    
    // 输出结果
    if (files.empty()) {
        std::cout << "未找到 .Animal 后缀的文件" << std::endl;
    } else {
        std::cout << "找到 " << files.size() << " 个 .Animal 文件：" << std::endl;
        for (const auto& file : files) {
            std::cout << " - " << file << std::endl;
            
        }
    }
    bool write_success = write_tool->write_to_csv(files, csv_output_path);
    if (write_success) {
        std::cout << "提示：用 Excel 直接打开 " << csv_output_path << " 即可查看表格" << std::endl;
    } else {
        std::cerr << "警告：CSV 文件写入失败！" << std::endl;
    }
}

