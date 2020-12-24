# Automatic c++ Json parser
Use Json as a typical c++ object. No need to get()/set().

## Usage:

  ### Inherit your json object from JObject and declare it with macro JOBJECT():
  ```
  #include <JObject.h>
  
  struct Person : public JObject
  {
      JInt    id;
      JString name;

      JOBJECT(Person);
  };

  struct Employee : public JObject
  {
      Person         person;
      JArray<Person> staff;

      JOBJECT(Employee);
  };
  ```

  ### Declare json fields of the object in a cpp file:
  ```
  BEG_JFIELDS(Person)
      JFIELD(id),
      JFIELD(name)
  END_JFIELDS

  BEG_JFIELDS(Employee)
      JFIELD(person),
      JFIELD(staff)
  END_JFIELDS
  ```
  
  ### Deserialize Employee object from a json string:
  ```
  {
    "person":{
      "id": 0,
      "name": "geoge"
    }
    "staff":[
      {"id": 1, "name":"susan"},
      {"id": 2, "name":"mike"}
    ]
  }
  
  #include <iostream>
  
  int main(int argc, char* argv[])
  {
      Employee e;
      e.Deserialize("{\"person\":{\"id\":0,\"name\":\"geoge\"},\"staff\":[{\"id\":1,\"name\":\"susan\"},{\"id\":2,\"name\":\"mike\"}]}");
      
      std::cout << e.person.name << std::endl;
      std::cout << e.staff[1].name << std::endl;
      
      return 0;
  }
  ```
  
  A json object can also inherit another. See more details in Runner example.
  
## Compile Runner example:
  ```
  cd Json
  mkdir build
  cd build
  cmake ..
  make
  ```
