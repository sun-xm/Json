#pragma once

#include <ostream>
#include <string>

enum class JType
{
    INT,
    FLT,
    STR,
    OBJ,
    ARR,
    DATE,
    BOOL
};

static std::string to_string(JType type)
{
    switch (type)
    {
        case JType::INT:
            return "JType::INT";

        case JType::FLT:
            return "JType::FLT";

        case JType::STR:
            return "JType::STR";

        case JType::OBJ:
            return "JType::OBJ";

        case JType::ARR:
            return "JType::ARR";

        case JType::DATE:
            return "JType::DATE";

        case JType::BOOL:
            return "JType::BOOL";

        default:
            return "Unknown type";
    }
}

static std::ostream& operator<<(std::ostream& stream, JType type)
{
    stream << to_string(type);
    return stream;
}