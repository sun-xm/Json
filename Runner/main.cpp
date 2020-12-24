#include "Person.h"
#include "JPrint.h"
#include <algorithm>
#include <fstream>
#include <iostream>

using namespace std;

string Load(const string& file)
{
#ifdef WIN32
    ifstream in("..\\..\\Runner\\" + file);
#else
    ifstream in("../Runner/" + file);
#endif
    return string(istreambuf_iterator<char>(in), istreambuf_iterator<char>());
}

int main(int argc, char* argv[])
{
    JInt jint;
    jint.Deserialize("123");
    cout << jint << endl;
    cout << jint.Serialize() << endl << endl;

    JFloat jflt;
    jflt.Deserialize(".123");
    cout << jflt << endl;
    cout << jflt.Serialize() << endl << endl;

    JString jstr;
    jstr.Deserialize("\"123\"");
    cout << jstr << endl;
    cout << jstr.Serialize() << endl << endl;

    JParserError e;

    cout << "Person:" << endl;
    Person person;
    if (person.Deserialize(Load("person.json"), e))
    {
        cout << person << endl;
        cout << person.Serialize() << endl;

        cout << person.age   << endl;
        cout << person.name  << endl;
        cout << person.birth << endl << endl;

    }
    else
    {
        cerr << e << endl;
    }

    cout << "Employee:" << endl;
    Employee employee;
    if (employee.Deserialize(Load("person.json"), e))
    {
        cout << employee.Serialize() << endl;

        if (employee.id.HasValue())
        {
            cout << employee.id << endl;
        }
    }
    else
    {
        cerr << e << endl;
    }

    return 0;
}