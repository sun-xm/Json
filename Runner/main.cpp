#include "Person.h"
#include "JPrint.h"
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    string e;

    JStr jstr;
    if (jstr.Deserialize(istringstream("\"1\t2\\\\3\\\"4\""), e))
    {
        cout << jstr << endl;
    }
    else
    {
        cout << e << endl;
    }

    JInt jint;
    if (jint.Deserialize(istringstream("0x1aF"), e))
    {
        cout << jint << endl;
    }
    else
    {
        cout << e << endl;
    }

    JUint juint;
    if (juint.Deserialize(istringstream("123"), e))
    {
        cout << juint << endl;
    }
    else
    {
        cout << e << endl;
    }

    JFlt jflt;
    if (jflt.Deserialize(istringstream("123.456"), e))
    {
        cout << jflt << endl;
    }
    else
    {
        cout << e << endl;
    }

    JDate jdate;
    if (jdate.Deserialize(istringstream("\"1999-04-23T18:25:43.511Z\""), e))
    {
        cout << jdate << endl;
    }
    else
    {
        cout << e << endl;
    }

    JArr<JArr<JInt>> jarrarr;
    if (jarrarr.Deserialize(istringstream("[[1, 2], [2, 3], [3, 4]]"), e))
    {
        jarrarr.ForEachItem([](const JArr<JInt>& jarr)
        {
            jarr.ForEachItem([](const JInt& jint)
            {
                cout << jint << ',';
            });
            cout << endl;
        });
        cout << endl;
    }
    else
    {
        cout << e << endl;
    }

    ifstream json("person.json");
    if (!json.is_open())
    {
        cout << "Failed to open person.json" << endl;
        return 0;
    }

    Person person;
    if (person.Deserialize(json, e))
    {
        cout << person << endl;
        cout << person.Serialize() << endl;
    }
    else
    {
        cout << JError(json, e) << endl;
    }

    Employee employee;
    json.clear();
    json.seekg(0);
    if (employee.Deserialize(json, e))
    {
        cout << employee << endl;
        cout << employee.Serialize() << endl;
    }
    else
    {
        cout << JError(json, e) << endl;
    }

    return 0;
}