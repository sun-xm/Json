#include "JObject.h"
#include <codecvt>
#include <sstream>

using namespace std;

#ifdef JOBJECT_NS
using namespace JOBJECT_NS;
#endif


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

bool JField::Serialize(ostream&& json) const
{
    return this->Serialize((ostream&)json);
}

bool JField::Deserialize(istream& json)
{
    try
    {
        JParser::Deserialize(json, *this);
        return true;

    } catch(const exception&)
    {
        this->Clear();
        return false;
    }
}

bool JField::Deserialize(istream&& json)
{
    return this->Deserialize((istream&)json);
}

bool JField::Deserialize(istream& json, string& error, size_t& where)
{
    try
    {
        where = -1;
        error.clear();
        JParser::Deserialize(json, *this);
        return true;

    } catch(const exception& e)
    {
        if (!json)
        {
            json.clear();
            json.seekg(0, ios::end);
        }
        where = json.tellg();
        error = e.what();
        this->Clear();
        return false;
    }
}

bool JField::Deserialize(istream&& json, string& error, size_t& where)
{
    return this->Deserialize((istream&)json, error, where);
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
        this->Clear();
        return false;
    }
}

bool JField::Deserialize(string&& json)
{
    return this->Deserialize((const string&)json);
}

bool JField::Deserialize(const string& json, string& error, size_t& where)
{
    return this->Deserialize(istringstream(json), error, where);
}

bool JField::Deserialize(string&& json, string& error, size_t& where)
{
    return this->Deserialize(istringstream(json), error, where);
}

bool JVar::ToArr(JArray& arr, string& err) const
{
    arr.Clear();

    if (this->IsUndefined())
    {
        return true;
    }

    if (this->IsNull())
    {
        (JField&)arr = nullptr;
        return true;
    }

    if (JType::ARR != this->subtype)
    {
        err = "This is not an array";
        return false;
    }

    try
    {
        this->ForEachItem([&arr, &err](const JVar& var)
        {
            if (var.IsUndefined())
            {
                return false;
            }

            auto item = arr.NewItem();

            if (var.IsNull())
            {
                *item = nullptr;
                return false;
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
                        ((JBase*)item)->Set(var.Int());
                        break;
                    }

                    case JType::NUM:
                    {
                        ((JBase*)item)->Set(var.Num());
                        break;
                    }

                    case JType::STR:
                    {
                        ((JBase*)item)->Set(var.Str());
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
                ((JBase*)item)->Set((double)var.Int());
            }
            else
            {
                throw runtime_error("Type mismatch. Expecting: " + to_string(item->Type()) + ". Actual: " + to_string(var.Subtype()));
            }

            return false;
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

    if (this->IsUndefined())
    {
        return true;
    }

    if (this->IsNull())
    {
        (JField&)obj = nullptr;
        return true;
    }

    if (JType::OBJ != this->subtype)
    {
        err = "This is not an object";
        return false;
    }

    try
    {
        this->ForEachField([&obj, &err](const string& name, const JVar& var)
        {
            if (var.IsUndefined())
            {
                return false;
            }

            auto field = obj.GetField(name);
            if (!field)
            {
                return false;
            }

            if (var.IsNull())
            {
                *field = nullptr;
                return false;
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
                        ((JBase*)field)->Set(var.Int());
                        break;
                    }

                    case JType::NUM:
                    {
                        ((JBase*)field)->Set(var.Num());
                        break;
                    }

                    case JType::STR:
                    {
                        ((JBase*)field)->Set(var.Str());
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
                ((JBase*)field)->Set((double)var.Int());
            }
            else
            {
                throw runtime_error("Type mismatch: " + name + ". Expecting: " + to_string(field->Type()) + ". Actual: " + to_string(var.Subtype()));
            }

            return false;
        });
    } catch (const exception& e)
    {
        err = e.what();
        return false;
    }

    return true;
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
            *this = ((JBase&)field).GetI();
            break;
        }

        case JType::NUM:
        {
            *this = ((JBase&)field).GetN();
            break;
        }

        case JType::STR:
        {
            *this = ((JBase&)field).GetS();
            break;
        }

        case JType::BOOL:
        {
            *this =((JBase&)field).GetB();
            break;
        }

        case JType::ARR:
        {
            ((JArray&)field).ForEach([this](const JField& field)
            {
                auto item = this->NewItem();
                *item = field;
                return false;
            });

            break;
        }

        case JType::OBJ:
        {
            ((JObject&)field).ForEach([this](const string& name, const JField& field)
            {
                auto f = this->GetField(name);
                *f = field;
                return false;
            });

            break;
        }

        case JType::VAR:
        {
            auto& var = (JVariant&)field;
            switch (var.Subtype())
            {
                case JType::BOOL:
                {
                    *this = ((JBase&)var).GetB();
                    break;
                }

                case JType::INT:
                {
                    *this = ((JBase&)var).GetI();
                    break;
                }

                case JType::NUM:
                {
                    *this = ((JBase&)var).GetN();
                    break;
                }

                case JType::STR:
                {
                    *this = ((JBase&)var).GetS();
                    break;
                }

                case JType::ARR:
                {
                    function<void(const JVariant&)> foreach = [this](const JVariant& var)
                    {
                        (*this->NewItem()) = var;
                    };
                    var.ForEach(foreach);
                    break;
                }

                case JType::OBJ:
                {
                    function<void(const string&, const JVariant&)> foreach = [this](const string& name, const JVariant& var)
                    {
                        (*this->GetField(name)) = var;
                    };
                    var.ForEach(foreach);
                    break;
                }

                default: break; // ignore JType::VAR
            }
            break;
        }

        default: break;
    }

    return *this;
}