#pragma once

#include "JObject.h"
#include <ostream>

void JPrint(std::ostream& stream, const JArr& arr, const std::string& name, int indent);
void JPrint(std::ostream& stream, const JField& field, const std::string& name, int indent);
void JPrint(std::ostream& stream, const JObject& object, const std::string& name, int indent);
std::ostream& operator<<(std::ostream& stream, const JDate& date);
std::ostream& operator<<(std::ostream& stream, const JString& str);
std::ostream& operator<<(std::ostream& stream, const JField& field);
std::ostream& operator<<(std::ostream& stream, const JParserError& error);