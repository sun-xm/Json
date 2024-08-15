#pragma once

#include "JObject.h"
#include <ostream>

void JPrint(std::ostream& stream, const JField& field, const std::string& name, int indent);
void JPrint(std::ostream& stream, const JArray& array, const std::string& name, int indent);
void JPrint(std::ostream& stream, const JObject& object, const std::string& name, int indent);
std::string JError(std::istream& json, const std::string& err, size_t where);
std::ostream& operator<<(std::ostream& stream, const JDate& date);
std::ostream& operator<<(std::ostream& stream, const JStr& str);
std::ostream& operator<<(std::ostream& stream, const JField& field);