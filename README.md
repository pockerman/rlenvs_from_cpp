# py_from_cpp

Some utilities based on Boost.Python to execute Python code from C++.


## Importing a module

```
#include "pfcpp/py_interpreter.h"
#include <iostream>


int main(){

    try
    {
        std::cout<<"Starting the interpreter..."<<std::endl;
        pfcpp::PyInterpreter interpreter(true);
        auto gym_module = interpreter.import("gym");
    }
    catch(const boost::python::error_already_set& )
    {
        PyErr_Print();
    }

    std::cout<<"Finilize..."<<std::endl;
    return 0;
}
```

## Issues

- ```pyconfig.h``` not found

Run: ```find /usr/include -name pyconfig.h```
Export: ```export CPLUS_INCLUDE_PATH="$CPLUS_INCLUDE_PATH:/path/to/pyconfig"```

