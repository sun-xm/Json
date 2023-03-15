#include "JObject.h"
#include "JParser.h"
#include <sstream>

using namespace std;

string JField::Serialize() const
{
    ostringstream json;
    this->Serialize(json);
    return json.str();
}

bool JField::Serialize(ostream& json) const
{
    JParser::Serialize(json, *this);
    return !!json;
}

bool JField::Deserialize(const string& json)
{
    auto stream = istringstream(json);

    try
    {
        JParser::Deserialize(stream, *this);
        return true;

    } catch(const exception&)
    {
        return false;
    }
}

bool JField::Deserialize(istream& json)
{
    try
    {
        JParser::Deserialize(json, *this);
        return true;

    } catch(const exception&)
    {
        return false;
    }
}

bool JField::Deserialize(const string& json, string& error, size_t& where)
{
    auto stream = istringstream(json);

    try
    {
        JParser::Deserialize(stream, *this);
        return true;

    } catch(const exception& e)
    {
        where = stream ? (size_t)stream.tellg() : json.length();
        error = e.what();
        return false;
    }
}

bool JField::Deserialize(istream& json, string& error)
{
    try
    {
        JParser::Deserialize(json, *this);
        return true;

    } catch(const exception& e)
    {
        error = e.what();
        return false;
    }
}

JUndVar und;
JUndVar* JVar::UndVar = &und;

JVar* JVar::GetNewItem()
{
    if (JType::ARR != this->subtype)
    {
        this->fields.clear();
        this->Define(JType::ARR);
    }

    auto name = to_string(this->fields.size());
    return &(this->fields[name] = JVar());
}

JVar* JVar::GetNewField(const string& name)
{
    if (JType::OBJ != this->subtype)
    {
        this->fields.clear();
        this->Define(JType::OBJ);
    }

    auto itr = this->fields.find(name);
    if (this->fields.end() != itr)
    {
        return &itr->second;
    }

    return &(this->fields[name] = JVar());
}

bool JVar::ToArr(JArray& arr, string& err) const
{
    arr.Clear();

    if (this->undef)
    {
        return true;
    }

    if (this->null)
    {
        (JField&)arr = nullptr;
        return true;
    }

    if (JType::ARR != this->subtype)
    {
        err = "This is not an array";
        return false;
    }

    arr.Define();

    try
    {
        this->ForEachItem([&arr, &err](const JVar& var)
        {
            auto item = arr.GetNew();

            if (var.IsNull())
            {
                *item = nullptr;
                return;
            }

            if (JType::VAR == item->Type())
            {
                *(JVar*)item = var;
            }
            else if (item->Type() == var.Subtype())
            {
                switch (item->Type())
                {
                    case JType::INT:
                    {
                        *(JInt*)item = var.Int;
                        break;
                    }

                    case JType::NUM:
                    {
                        *(JNum*)item = var.Num;
                        break;
                    }

                    case JType::STR:
                    {
                        *(JStr*)item = var.Str;
                        break;
                    }

                    case JType::ARR:
                    {
                        if (!var.ToArr(*(JArray*)item, err))
                        {
                            throw runtime_error(err);
                        }
                        break;
                    }

                    case JType::OBJ:
                    {
                        if (!var.ToObj(*(JObject*)item, err))
                        {
                            throw runtime_error(err);
                        }
                        break;
                    }

                    default: break;
                }
            }
            else if (JType::NUM == item->Type() && JType::INT == var.Subtype())
            {
                *(JNum*)item = (double)var.Int;
            }
            else
            {
                throw runtime_error("Type mismatch. Expecting: " + to_string(item->Type()) + ". Actual: " + to_string(var.Subtype()));
            }
        });
    } catch (const exception& e)
    {
        err = e.what();
        return false;
    }

    return true;
}

bool JVar::ToObj(JObject& obj, string& err) const
{
    obj.Clear();

    if (this->undef)
    {
        return true;
    }

    if (this->null)
    {
        (JField&)obj = nullptr;
        return true;
    }

    if (JType::OBJ != this->subtype)
    {
        err = "This is not an object";
        return false;
    }

    obj.Define();

    try
    {
        this->ForEachField([&obj, &err](const string& name, const JVar& var)
        {
            if (var.IsUndefined())
            {
                return;
            }

            auto field = obj.GetField(name);
            if (!field)
            {
                return;
            }

            if (var.IsNull())
            {
                *field = nullptr;
                return;
            }

            if (JType::VAR == field->Type())
            {
                *(JVar*)field = var;
            }
            else if (field->Type() == var.Subtype())
            {
                switch (field->Type())
                {
                    case JType::INT:
                    {
                        *(JInt*)field = var.Int;
                        break;
                    }

                    case JType::NUM:
                    {
                        *(JNum*)field = var.Num;
                        break;
                    }

                    case JType::STR:
                    {
                        *(JStr*)field = var.Str;
                        break;
                    }

                    case JType::ARR:
                    {
                        if (!var.ToArr(*(JArray*)field, err))
                        {
                            throw runtime_error(err);
                        }
                        break;
                    }

                    case JType::OBJ:
                    {
                        if (!var.ToObj(*(JObject*)field, err))
                        {
                            throw runtime_error(err);
                        }
                        break;
                    }

                    default: break;
                }
            }
            else if (JType::NUM == field->Type() && JType::INT == var.Subtype())
            {
                *(JNum*)field = (double)var.Int;
            }
            else
            {
                throw runtime_error("Type mismatch: " + name + ". Expecting: " + to_string(field->Type()) + ". Actual: " + to_string(var.Subtype()));
            }
        });
    } catch (const exception& e)
    {
        err = e.what();
        return false;
    }

    return true;
}

JVar& JVar::operator[](size_t index)
{
    if (JType::ARR != this->Subtype())
    {
        throw out_of_range("Is not an array");
    }

    if (index >= this->fields.size())
    {
        throw out_of_range("Index is out of range");
    }

    return this->fields.find(to_string(index))->second;
}

const JVar& JVar::operator[](size_t index) const
{
    if (JType::ARR != this->Subtype())
    {
        throw out_of_range("Is not an array");
    }

    if (index >= this->fields.size())
    {
        throw out_of_range("Index is out of range");
    }

    return this->fields.find(to_string(index))->second;
}

JVar& JVar::operator[](const string& field)
{
    if (JType::OBJ != this->Subtype() && JType::VAR != this->Subtype())
    {
        throw runtime_error("Is not an object");
    }

    return *this->GetNewField(field);
}

const JVar& JVar::operator[](const string& field) const
{
    if (JType::OBJ != this->Subtype())
    {
        throw runtime_error("Is not an object");
    }

    if (!this->HasValue() || !this->HasField(field))
    {
        return (JVar&)*UndVar;
    }

    return this->fields.find(field)->second;
}

JVar& JVar::operator=(const JField& field)
{
    this->Clear();

    if (field.IsUndefined())
    {
        return *this;
    }

    if (field.IsNull())
    {
        *this = nullptr;
        return *this;
    }

    switch (field.Type())
    {
        case JType::INT:
        {
            *this = ((JInt&)field)();
            break;
        }

        case JType::NUM:
        {
            *this = ((JNum&)field)();
            break;
        }

        case JType::STR:
        {
            *this = ((JStr&)field)();
            break;
        }

        case JType::BOOL:
        {
            *this = ((JBool&)field)();
            break;
        }

        case JType::DATE:
        {
            *this = field.Serialize();
            break;
        }

        case JType::ARR:
        {
            this->Define(JType::ARR);

            ((JArray&)field).ForEach([this](const JField& field)
            {
                auto item = this->GetNewItem();
                *item = field;
            });

            break;
        }

        case JType::OBJ:
        {
            this->Define(JType::OBJ);

            ((JObject&)field).ForEach([this](const string& name, const JField& field)
            {
                auto f = this->GetNewField(name);
                *f = field;
            });

            break;
        }

        case JType::VAR:
        {
            *this = (JVar&)field;
            break;
        }

        default: break;
    }

    return *this;
}