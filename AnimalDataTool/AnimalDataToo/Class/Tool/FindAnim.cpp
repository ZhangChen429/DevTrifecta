#include "FindAnim.h"

#include <iostream>


FindAnim::FindAnim()
{
    
}

bool FindAnim::hasAnimalSuffix(const std::string& filename) {
    const std::string suffix = ".animal";
    if (filename.size() < suffix.size()) return false;
    // 比较文件名末尾与后缀
    return filename.compare(filename.size() - suffix.size(), suffix.size(), suffix) == 0;
}

// 查找目标文件夹下所有后缀为 .Animal 的文件（支持递归/非递归）
std::vector<std::string> FindAnim::find_animal_files(
    const std::string& target_folder,  // 目标文件夹路径
    bool recursive = false             // 是否递归遍历子目录（默认不递归）
) {
    std::vector<std::string> result;
    const std::string suffix = ".anims";

    // 检查目标文件夹是否存在
    if (!fs::exists(target_folder) || !fs::is_directory(target_folder)) {
        std::cerr << "错误：文件夹不存在或不是目录 -> " << target_folder << std::endl;
        return result;
    }

    if (recursive) {
        // 递归遍历：使用 recursive_directory_iterator
        for (const fs::directory_entry& entry : fs::recursive_directory_iterator(target_folder)) {
            if (fs::is_regular_file(entry.path())) {  // 传入 path 类型，匹配函数参数
                std::string filename = entry.path().filename().string();
                if (filename.size() >= suffix.size() &&
                    filename.substr(filename.size() - suffix.size()) == suffix) {
                    result.push_back(entry.path().string());
                    }
            }
        }
    } else {
        // 非递归遍历：使用 directory_iterator
        for (const fs::directory_entry& entry : fs::directory_iterator(target_folder)) {
            if (fs::is_regular_file(entry.path())) {  // 传入 path 类型，匹配函数参数
                std::string filename = entry.path().filename().string();
                if (filename.size() >= suffix.size() &&
                    filename.substr(filename.size() - suffix.size()) == suffix) {
                    result.push_back(entry.path().string());
                    }
            }
        }
    }


    return result;
}