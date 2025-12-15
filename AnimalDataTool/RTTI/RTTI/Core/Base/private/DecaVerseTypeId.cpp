#include "../public/DecaVerseTypeId.h"

// FNV-1a 哈希算法常量
constexpr uint64_t FNV_OFFSET_BASIS = 14695981039346656037ULL;
constexpr uint64_t FNV_PRIME = 1099511628211ULL;

TypeId GenerateTypeId(const std::string& typeName)
{
    uint64_t hash = FNV_OFFSET_BASIS;
    for (char c : typeName)
    {
        hash ^= static_cast<uint64_t>(c);
        hash *= FNV_PRIME;
    }
    return hash;
}

DecaVerseTypeId::DecaVerseTypeId()
{
}