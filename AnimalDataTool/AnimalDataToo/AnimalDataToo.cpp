
#include <iostream>
#include <vector>
#include <string>
#include "libxl.h"
#include "Class/Tool/AnimGroup.h"
#include "Class/Tool/AnimGroupMethod.h"
#include "Class/Tool/FindAnim.h"
#include "Class/Tool/WriteTool.h"


int main(int argc, char* argv[])
{
    std::string folder = "G:\\SoftApp\\Sy2077\\2077\\2077\\CDPR2077\\r6\\depot\\base\\animations";
    std::string folder_quest = "G:\\SoftApp\\Sy2077\\2077\\2077\\CDPR2077\\r6\\depot\\base\\animations\\quest";
    std::string folder_npc =  "G:\\SoftApp\\Sy2077\\2077\\2077\\CDPR2077\\r6\\depot\\base\\animations\\npc";
    std::string folder_facial =  "G:\\SoftApp\\Sy2077\\2077\\2077\\CDPR2077\\r6\\depot\\base\\animations\\facial";
    std::string folder_items =  "G:\\SoftApp\\Sy2077\\2077\\2077\\CDPR2077\\r6\\depot\\base\\animations\\items";
    std::string folder_marketing =  "G:\\SoftApp\\Sy2077\\2077\\2077\\CDPR2077\\r6\\depot\\base\\animations\\marketing";
    std::string folder_synced =  "G:\\SoftApp\\Sy2077\\2077\\2077\\CDPR2077\\r6\\depot\\base\\animations\\synced";
    std::string folder_ui =  "G:\\SoftApp\\Sy2077\\2077\\2077\\CDPR2077\\r6\\depot\\base\\animations\\ui";
    std::string folder_vehicle =  "G:\\SoftApp\\Sy2077\\2077\\2077\\CDPR2077\\r6\\depot\\base\\animations\\vehicle";
    std::string folder_weapon =  "G:\\SoftApp\\Sy2077\\2077\\2077\\CDPR2077\\r6\\depot\\base\\animations\\weapon";
    // 非递归查找（仅当前目录）
   AnimGroupMethod* anim_group_method = new AnimGroupMethod;
    std::string csv_output_path = "Animal文件列表.csv";
    std::string csv_output_path_quest = "Animalquest文件列表.csv";
    std::string csv_output_path_npc = "Animalnpc文件列表.csv";
    std::string csv_output_path_facial = "Animalfacial文件列表.csv";
    std::string csv_output_path_items = "Animalitems文件列表.csv";
    std::string csv_output_path_marketing = "Animalmarketing文件列表.csv";
    std::string csv_output_path_synced = "Animalsynced文件列表.csv";
    std::string csv_output_path_ui = "Animalui文件列表.csv";
    std::string csv_output_path_vehicle = "Animalvehicle文件列表.csv";
    std::string csv_output_path_weapon = "Animalweapon文件列表.csv";
    anim_group_method->AnimSCVCreate(folder,true,csv_output_path);
    anim_group_method->AnimSCVCreate(folder_quest,true,csv_output_path_quest);
    anim_group_method->AnimSCVCreate(folder_npc,true,csv_output_path_npc);
    anim_group_method->AnimSCVCreate(folder_items,true,csv_output_path_facial);
    anim_group_method->AnimSCVCreate(folder_marketing,true,csv_output_path_items);
    anim_group_method->AnimSCVCreate(folder_synced,true,csv_output_path_marketing);
    anim_group_method->AnimSCVCreate(folder_ui,true,csv_output_path_ui);
    anim_group_method->AnimSCVCreate(folder_vehicle,true,csv_output_path_vehicle);
    anim_group_method->AnimSCVCreate(folder_weapon,true,csv_output_path_weapon); 
    std::cout << "开始测试 libxl 库..." << std::endl;

    // 创建一个 Book 对象
    libxl::Book* book = xlCreateBook();

    if (book)
    {
        std::cout << "✓ Book 对象创建成功" << std::endl;

        // 添加一个工作表
        libxl::Sheet* sheet = book->addSheet(L"测试表");

        if (sheet)
        {
            std::cout << "✓ Sheet 创建成功" << std::endl;

            // 写入一些测试数据
            sheet->writeStr(0, 0, L"姓名");
            sheet->writeStr(0, 1, L"年龄");
            sheet->writeStr(0, 2, L"城市");

            sheet->writeStr(1, 0, L"张三");
            sheet->writeNum(1, 1, 25);
            sheet->writeStr(1, 2, L"北京");

            sheet->writeStr(2, 0, L"李四");
            sheet->writeNum(2, 1, 30);
            sheet->writeStr(2, 2, L"上海");

            std::cout << "✓ 数据写入成功" << std::endl;

            // 保存文件
            if (book->save(L"test_output.xlsx"))
            {
                std::cout << "✓ 文件保存成功: test_output.xlsx" << std::endl;
                std::cout << "\n=== libxl 库配置成功！===" << std::endl;
            }
            else
            {
                std::cout << "✗ 文件保存失败" << std::endl;
                std::cout << "错误信息: " << book->errorMessage() << std::endl;
            }
        }
        else
        {
            std::cout << "✗ Sheet 创建失败" << std::endl;
        }

        // 释放资源
        book->release();
    }
    else
    {
        std::cout << "✗ Book 对象创建失败，请检查库配置" << std::endl;
        return 1;
    }

    system("pause");
    return 0;
}
