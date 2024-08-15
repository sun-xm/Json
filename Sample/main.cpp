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
    size_t w;

    ifstream json("person.json", ios::binary);
    if (!json.is_open())
    {
        cout << "Failed to open person.json" << endl;
        return 0;
    }

    Person person;
    if (person.Deserialize(json, e, w))
    {
        cout << person << endl;
        cout << person.Serialize() << endl;
    }
    else
    {
        cout << JError(json, e, w) << endl;
    }

    Employee employee;
    json.clear();
    json.seekg(0);
    if (employee.Deserialize(json, e, w))
    {
        cout << employee << endl;
        cout << employee.Serialize() << endl;
    }
    else
    {
        cout << JError(json, e, w) << endl;
    }

    JVar var;
    json.clear();
    json.seekg(0);
    if (var.Deserialize(json, e, w))
    {
        cout << var << endl;
        cout << var["skills"][1] << endl;
    }
    else
    {
        cout << JError(json, e, w) << endl;
    }

    return 0;
}