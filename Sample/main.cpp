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

    ifstream json("person.json", ios::binary);
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

    JVar var;
    json.clear();
    json.seekg(0);
    if (var.Deserialize(json, e))
    {
        cout << var << endl;
        cout << var["skills"][1] << endl;
    }
    else
    {
        cout << JError(json, e) << endl;
    }

    return 0;
}