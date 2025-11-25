#pragma once
#include <iostream>                                                                                                 
#include <fstream>                                                                                                  
#include <sstream>                                                                                                  
#include <string>                                                                                                   
#include <vector>                                                                                                   
#include <map>                                                                                                      
#include <regex>                                                                                                    
#include <algorithm>                                                                                                
#include <codecvt>                                                                                                  
#include <iomanip>
#include <locale>                                                                                                   
                                                                                                                    
// CSV行数据结构                                                                                                    
struct CSVRow {                                                                                                     
    std::string index;                                                                                              
    std::string filename;                                                                                           
    std::string fullpath;                                                                                           
                                                                                                                    
    // 分类信息                                                                                                     
    std::string relativePath;                                                                                       
    std::string topCategory;                                                                                        
    std::string subCategory;                                                                                        
    std::string bodyType;                                                                                           
    std::string actionType;                                                                                         
    std::string sceneType;                                                                                          
    std::string weaponType;                                                                                         
    std::string cyberwareType;                                                                                      
    std::string characterPrefix;                                                                                    
    std::string specialTags;                                                                                        
    int depth;                                                                                                      
};                                                                                                                  
                                                                                                                    
class AnimsClassifier {                                                                                             
private:                                                                                                            
    // 分类规则映射                                                                                                 
    std::map<std::string, std::regex> bodyTypePatterns;                                                             
    std::map<std::string, std::regex> actionPatterns;                                                               
    std::map<std::string, std::regex> scenePatterns;                                                                
    std::vector<std::string> weaponTypes;                                                                           
    std::vector<std::string> cyberwareTypes;                                                                        
    std::map<std::string, std::string> characterPrefixes;                                                           
                                                                                                                    
public:                                                                                                             
    AnimsClassifier() {                                                                                             
        initializePatterns();                                                                                       
    }                                                                                                               
                                                                                                                    
    void initializePatterns() {                                                                                     
        // 体型分类模式                                                                                             
        bodyTypePatterns["male_average"] = std::regex("male_average|man_average", std::regex::icase);               
        bodyTypePatterns["female_average"] = std::regex("female_average|woman_average", std::regex::icase);         
        bodyTypePatterns["male_big"] = std::regex("male_big|man_big", std::regex::icase);                           
        bodyTypePatterns["male_fat"] = std::regex("male_fat|man_fat", std::regex::icase);                           
        bodyTypePatterns["male_massive"] = std::regex("male_massive|man_massive", std::regex::icase);               
        bodyTypePatterns["male_child"] = std::regex("male_child|child_male|boy", std::regex::icase);                
        bodyTypePatterns["female_child"] = std::regex("female_child|child_female|girl", std::regex::icase);         
        bodyTypePatterns["female_chubby"] = std::regex("female_chubby|woman_chubby", std::regex::icase);            
                                                                                                                    
        // 动作类型模式                                                                                             
        actionPatterns["stand"] = std::regex("\\bstand", std::regex::icase);                                        
        actionPatterns["sit"] = std::regex("\\bsit", std::regex::icase);                                            
        actionPatterns["walk"] = std::regex("\\bwalk", std::regex::icase);                                          
        actionPatterns["run"] = std::regex("\\brun", std::regex::icase);                                            
        actionPatterns["idle"] = std::regex("\\bidle", std::regex::icase);                                          
        actionPatterns["combat"] = std::regex("\\bcombat", std::regex::icase);                                      
        actionPatterns["attack"] = std::regex("\\battack", std::regex::icase);                                      
        actionPatterns["takedown"] = std::regex("\\btakedown", std::regex::icase);                                  
        actionPatterns["finisher"] = std::regex("\\bfinisher", std::regex::icase);                                  
        actionPatterns["lean"] = std::regex("\\blean", std::regex::icase);                                          
        actionPatterns["lie"] = std::regex("\\blie", std::regex::icase);                                            
        actionPatterns["kneel"] = std::regex("\\bkneel", std::regex::icase);                                        
        actionPatterns["crouch"] = std::regex("\\bcrouch", std::regex::icase);                                      
                                                                                                                    
        // 场景类型模式                                                                                             
        scenePatterns["interactive_scene"] = std::regex("interactive_scene", std::regex::icase);                    
        scenePatterns["open_world"] = std::regex("open_world", std::regex::icase);                                  
        scenePatterns["gameplay"] = std::regex("gameplay", std::regex::icase);                                      
        scenePatterns["cutscene"] = std::regex("cutscene", std::regex::icase);                                      
                                                                                                                    
        // 武器类型                                                                                                 
        weaponTypes = {                                                                                             
            "handgun", "revolver", "smg", "rifle_assault", "rifle_precision",                                       
            "rifle_sniper", "shotgun", "lmg", "katana", "knife", "baton",                                           
            "melee_fists", "one_handed_blunt", "two_handed_blunt"                                                   
        };                                                                                                          
                                                                                                                    
        // 义体类型                                                                                                 
        cyberwareTypes = {                                                                                          
            "mantisblade", "monowire", "launcher", "strongarms",                                                    
            "personal_link", "armshield", "jammer"                                                                  
        };                                                                                                          
                                                                                                                    
        // 角色前缀                                                                                                 
        characterPrefixes["pma"] = "Player_Male_Average";                                                           
        characterPrefixes["pwa"] = "Player_Female_Average";                                                         
        characterPrefixes["cw"] = "Cyberware";                                                                      
        characterPrefixes["ma"] = "Male_Average";                                                                   
        characterPrefixes["wa"] = "Female_Average";                                                                 
        characterPrefixes["face"] = "Facial";                                                                       
    }                                                                                                               
                                                                                                                    
    // 提取相对路径                                                                                                 
    std::string extractRelativePath(const std::string& fullPath) {                                                  
        size_t pos = fullPath.find("\\animations\\");                                                               
        if (pos == std::string::npos) {                                                                             
            pos = fullPath.find("/animations/");                                                                    
        }                                                                                                           
                                                                                                                    
        if (pos != std::string::npos) {                                                                             
            std::string relative = fullPath.substr(pos + 12); // 跳过 "animations\"                                 
            std::replace(relative.begin(), relative.end(), '\\', '/');                                              
            return relative;                                                                                        
        }                                                                                                           
        return fullPath;                                                                                            
    }                                                                                                               
                                                                                                                    
    // 获取顶级分类                                                                                                 
    std::string getTopCategory(const std::string& relPath) {                                                        
        size_t pos = relPath.find('/');                                                                             
        if (pos != std::string::npos) {                                                                             
            return relPath.substr(0, pos);                                                                          
        }                                                                                                           
        return "";                                                                                                  
    }                                                                                                               
                                                                                                                    
    // 获取子分类                                                                                                   
    std::string getSubCategory(const std::string& relPath) {                                                        
        size_t pos1 = relPath.find('/');                                                                            
        if (pos1 != std::string::npos) {                                                                            
            size_t pos2 = relPath.find('/', pos1 + 1);                                                              
            if (pos2 != std::string::npos) {                                                                        
                return relPath.substr(pos1 + 1, pos2 - pos1 - 1);                                                   
            }                                                                                                       
        }                                                                                                           
        return "";                                                                                                  
    }                                                                                                               
                                                                                                                    
    // 根据模式分类                                                                                                 
    std::string classifyByPatterns(const std::string& path,                                                         
                                   const std::map<std::string, std::regex>& patterns) {                             
        std::vector<std::string> matches;                                                                           
        for (const auto& [name, pattern] : patterns) {                                                              
            if (std::regex_search(path, pattern)) {                                                                 
                matches.push_back(name);                                                                            
            }                                                                                                       
        }                                                                                                           
                                                                                                                    
        if (matches.empty()) return "";                                                                             
                                                                                                                    
        std::string result;                                                                                         
        for (size_t i = 0; i < matches.size(); ++i) {                                                               
            if (i > 0) result += "; ";                                                                              
            result += matches[i];                                                                                   
        }                                                                                                           
        return result;                                                                                              
    }                                                                                                               
                                                                                                                    
    // 分类武器类型                                                                                                 
    std::string classifyWeaponType(const std::string& path) {                                                       
        std::string lowerPath = path;                                                                               
        std::transform(lowerPath.begin(), lowerPath.end(), lowerPath.begin(), ::tolower);                           
                                                                                                                    
        for (const auto& weapon : weaponTypes) {                                                                    
            if (lowerPath.find(weapon) != std::string::npos) {                                                      
                return weapon;                                                                                      
            }                                                                                                       
        }                                                                                                           
        return "";                                                                                                  
    }                                                                                                               
                                                                                                                    
    // 分类义体类型                                                                                                 
    std::string classifyCyberwareType(const std::string& path) {                                                    
        std::string lowerPath = path;                                                                               
        std::transform(lowerPath.begin(), lowerPath.end(), lowerPath.begin(), ::tolower);                           
                                                                                                                    
        for (const auto& cyber : cyberwareTypes) {                                                                  
            if (lowerPath.find(cyber) != std::string::npos) {                                                       
                return cyber;                                                                                       
            }                                                                                                       
        }                                                                                                           
        return "";                                                                                                  
    }                                                                                                               
                                                                                                                    
    // 获取角色前缀                                                                                                 
    std::string getCharacterPrefix(const std::string& filename) {                                                   
        std::string lowerName = filename;                                                                           
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);                           
                                                                                                                    
        for (const auto& [prefix, meaning] : characterPrefixes) {                                                   
            if (lowerName.find(prefix + "_") == 0) {                                                                
                return meaning;                                                                                     
            }                                                                                                       
        }                                                                                                           
        return "";                                                                                                  
    }                                                                                                               
                                                                                                                    
    // 获取特殊标签                                                                                                 
    std::string getSpecialTags(const std::string& path) {                                                           
        std::vector<std::string> tags;                                                                              
        std::string lowerPath = path;                                                                               
        std::transform(lowerPath.begin(), lowerPath.end(), lowerPath.begin(), ::tolower);                           
                                                                                                                    
        if (lowerPath.find("facial") != std::string::npos ||                                                        
            lowerPath.find("face_") != std::string::npos) {                                                         
            tags.push_back("Facial");                                                                               
        }                                                                                                           
        if (lowerPath.find("sync") != std::string::npos) {                                                          
            tags.push_back("Sync");                                                                                 
        }                                                                                                           
        if (lowerPath.find("finisher") != std::string::npos) {                                                      
            tags.push_back("Finisher");                                                                             
        }                                                                                                           
        if (lowerPath.find("takedown") != std::string::npos) {                                                      
            tags.push_back("Takedown");                                                                             
        }                                                                                                           
        if (lowerPath.find("transition") != std::string::npos) {                                                    
            tags.push_back("Transition");                                                                           
        }                                                                                                           
        if (lowerPath.find("fpp") != std::string::npos) {                                                           
            tags.push_back("FPP");                                                                                  
        }                                                                                                           
        if (lowerPath.find("tpp") != std::string::npos) {                                                           
            tags.push_back("TPP");                                                                                  
        }                                                                                                           
        if (lowerPath.find("work") != std::string::npos) {                                                          
            tags.push_back("Work");                                                                                 
        }                                                                                                           
        if (lowerPath.find("gesture") != std::string::npos) {                                                       
            tags.push_back("Gesture");                                                                              
        }                                                                                                           
        if (lowerPath.find("idle") != std::string::npos) {                                                          
            tags.push_back("Idle");                                                                                 
        }                                                                                                           
        if (lowerPath.find("combat") != std::string::npos) {                                                        
            tags.push_back("Combat");                                                                               
        }                                                                                                           
                                                                                                                    
        if (tags.empty()) return "";                                                                                
                                                                                                                    
        std::string result;                                                                                         
        for (size_t i = 0; i < tags.size(); ++i) {                                                                  
            if (i > 0) result += "; ";                                                                              
            result += tags[i];                                                                                      
        }                                                                                                           
        return result;                                                                                              
    }                                                                                                               
                                                                                                                    
    // 获取目录深度                                                                                                 
    int getDepth(const std::string& relPath) {                                                                      
        return std::count(relPath.begin(), relPath.end(), '/');                                                     
    }                                                                                                               
                                                                                                                    
    // 对一行数据进行分类                                                                                           
    void classifyRow(CSVRow& row) {                                                                                 
        row.relativePath = extractRelativePath(row.fullpath);                                                       
        row.topCategory = getTopCategory(row.relativePath);                                                         
        row.subCategory = getSubCategory(row.relativePath);                                                         
        row.bodyType = classifyByPatterns(row.relativePath, bodyTypePatterns);                                      
        row.actionType = classifyByPatterns(row.relativePath, actionPatterns);                                      
        row.sceneType = classifyByPatterns(row.relativePath, scenePatterns);                                        
        row.weaponType = classifyWeaponType(row.relativePath);                                                      
        row.cyberwareType = classifyCyberwareType(row.relativePath);                                                
        row.characterPrefix = getCharacterPrefix(row.filename);                                                     
        row.specialTags = getSpecialTags(row.relativePath);                                                         
        row.depth = getDepth(row.relativePath);                                                                     
    }                                                                                                               
};                                                                                                                  
                                                                                                                    
// CSV工具函数                                                                                                      
std::string escapeCSV(const std::string& field);                                                                                                                   
                                                                                                                    
std::vector<std::string> parseCSVLine(const std::string& line);                                                                                                         
                                                                                                                    
void printStatistics(const std::vector<CSVRow>& rows);