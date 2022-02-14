#include "Person.h"

BEG_JFIELDS(Address)
    JFIELD(country),
    JFIELD(city),
    JFIELD(street),
    JFIELD(zipcode)
END_JFIELDS

BEG_JFIELDS(Person)
    JFIELD(age),
    JFIELD(name),
    JFIELD_KEY(birth, "b-day"), // map "birth" field to key "b-day" in json. otherwise variable name is used as key
    JFIELD(married),
    JFIELD(weight),
    JFIELD(address),
    JFIELD(skills)
END_JFIELDS

BEG_JFIELDS(Employee)
    JFIELD(id),
    JFIELD(department)
END_JFIELDS
