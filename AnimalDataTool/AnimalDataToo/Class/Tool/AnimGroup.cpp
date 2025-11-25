#include "AnimGroup.h"

std::string escapeCSV(const std::string& field) {
    // 检查是否包含需要转义的特殊字符：, " \n \r
    bool needEscape = (field.find(',') != std::string::npos) ||
                      (field.find('"') != std::string::npos) ||
                      (field.find('\n') != std::string::npos) ||
                      (field.find('\r') != std::string::npos);

    if (!needEscape) {
        return field;  // 无特殊字符，直接返回
    }

    // 有特殊字符：用双引号包裹，且双引号转义为 ""
    std::string escaped = "\"";  // 开头包裹符
    for (char c : field) {
        if (c == '"') {
            escaped += "\"\"";  // 双引号转义（CSV 标准：" → ""）
        } else {
            escaped += c;
        }
    }
    escaped += "\"";  // 结尾包裹符
    return escaped;
}

std::vector<std::string> parseCSVLine(const std::string& line)
 {                                                    
    std::vector<std::string> fields;                                                                                
    std::string field;                                                                                              
    bool inQuotes = false;                                                                                          
                                                                                                                    
    for (size_t i = 0; i < line.length(); ++i) {                                                                    
        char c = line[i];                                                                                           
                                                                                                                    
        if (c == '"') {                                                                                             
            if (inQuotes && i + 1 < line.length() && line[i + 1] == '"') {                                          
                field += '"';                                                                                       
                ++i;                                                                                                
            } else {                                                                                                
                inQuotes = !inQuotes;                                                                               
            }                                                                                                       
        } else if (c == ',' && !inQuotes) {                                                                         
            fields.push_back(field);                                                                                
            field.clear();                                                                                          
        } else {                                                                                                    
            field += c;                                                                                             
        }                                                                                                           
    }                                                                                                               
    fields.push_back(field);                                                                                        
                                                                                                                    
    return fields;                                                                                                  
}

void printStatistics(const std::vector<CSVRow>& rows)
 {                                                             
    std::map<std::string, int> topCategories;                                                                       
    std::map<std::string, int> bodyTypes;                                                                           
                                                                                                                    
    for (const auto& row : rows) {                                                                                  
        if (!row.topCategory.empty()) {                                                                             
            topCategories[row.topCategory]++;                                                                       
        }                                                                                                           
                                                                                                                    
        if (!row.bodyType.empty()) {                                                                                
            std::istringstream iss(row.bodyType);                                                                   
            std::string token;                                                                                      
            while (std::getline(iss, token, ';')) {                                                                 
                token.erase(0, token.find_first_not_of(" "));                                                       
                token.erase(token.find_last_not_of(" ") + 1);                                                       
                if (!token.empty()) {                                                                               
                    bodyTypes[token]++;                                                                             
                }                                                                                                   
            }                                                                                                       
        }                                                                                                           
    }                                                                                                               
                                                                                                                    
    std::cout << "\n";                                                                                              
    std::cout << "============================================================\n";                                  
    std::cout << "                     分类统计信息\n";                                                             
    std::cout << "============================================================\n";                                  
                                                                                                                    
    // 打印顶级分类                                                                                                 
    std::cout << "\n【顶级分类分布】\n";                                                                            
    std::vector<std::pair<std::string, int>> topVec(topCategories.begin(), topCategories.end());                    
    std::sort(topVec.begin(), topVec.end(),                                                                         
              [](const auto& a, const auto& b) { return a.second > b.second; });                                    
                                                                                                                    
    for (const auto& [cat, count] : topVec) {                                                                       
        std::cout << "  " << std::left << std::setw(25) << cat                                                      
                  << ": " << std::right << std::setw(6) << count << "\n";                                           
    }                                                                                                               
                                                                                                                    
    // 打印体型分类                                                                                                 
    if (!bodyTypes.empty()) {                                                                                       
        std::cout << "\n【角色体型分布】\n";                                                                        
        std::vector<std::pair<std::string, int>> bodyVec(bodyTypes.begin(), bodyTypes.end());                       
        std::sort(bodyVec.begin(), bodyVec.end(),                                                                   
                  [](const auto& a, const auto& b) { return a.second > b.second; });                                
                                                                                                                    
        for (const auto& [body, count] : bodyVec) {                                                                 
            std::cout << "  " << std::left << std::setw(25) << body                                                 
                      << ": " << std::right << std::setw(6) << count << "\n";                                       
        }                                                                                                           
    }                                                                                                               
                                                                                                                    
    std::cout << "\n";                                                                                              
}               