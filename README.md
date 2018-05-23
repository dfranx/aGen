#  aGen
aGen is a header-only C++17 library that can be used to generate BlueVM bytecode.
It can be used as a layer for compilers.

## How to use
Since this is a header-only library no compiling is needed. You put
`aGen.hpp` in your project and after that start using aGen by including
the header.

There are few examples in the `examples` directory. The examples and aGen
require no dependencies. 

**NOTE**: you need to have a compiler that supports C\+\+17 features
to compile projects that use this library.

## Example
Include library in your project:
```c++
#include "aGen.hpp"
```

Define new bytecode generator
```c++
ag::Generator gen;
```

Set bytecode version
```c++
gen.SetHeader(1, 0);
```

Define new function
```c++
gen.Function.Create("main");
```

Make that function active - function in which we will place our code
```c++
gen.Function.SetCurrent("main");
```

Put some code in the active function:
```c++
gen.Function.PushStack(5);	// 5
gen.Function.PushStack(2);	// 2 5
gen.Function.Add();		// 5 + 2
gen.Function.Return();		// return 5 + 2
```

Get bytecode that we generated with those commands:
```c++
ag::ByteCode bc = gen.Get();
```

## License
This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details



