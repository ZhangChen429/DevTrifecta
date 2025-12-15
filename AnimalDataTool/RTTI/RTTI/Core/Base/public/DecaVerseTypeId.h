#pragma once
#include <cstdint>
#include <string>

namespace DecaVerseBase{}
namespace DecaVerseCore{}

using TypeId = uint64_t;

// 生成类型ID（基于字符串哈希，如FNV-1a）
TypeId GenerateTypeId(const std::string& typeName);
class DecaVerseTypeId
{
public:
    DecaVerseTypeId();
private:
};


