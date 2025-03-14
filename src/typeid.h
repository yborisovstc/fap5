#ifndef __FAP3_TYPEID_H
#define __FAP3_TYPEID_H

#include <string_view>

/** @brief Type ID class
 * Identifies types of elements
 * */
struct TypeId
{
    TypeId(const char* aStr): mString(aStr), mHash(std::hash<std::string_view>{}(mString)) {}
    std::string_view mString;
    std::size_t mHash;
};

#endif

