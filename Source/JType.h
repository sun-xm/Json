#pragma once

#include <ostream>
#include <string>

enum class JType
{
    INT,
    NUM,
    STR,
    OBJ,
    ARR,
    DATE,
    BOOL,
    VAR
};

static std::string to_string(JType type)
{
    switch (type)
    {
        case JType::INT:
            return "JType::INT";

        case JType::NUM:
            return "JType::NUM";

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

        case JType::VAR:
            return "JType::VAR";

        default:
            return "Unknown type";
    }
}

static std::ostream& operator<<(std::ostream& stream, JType type)
{
    stream << to_string(type);
    return stream;
}