#include <iostream>
#include <fstream>
#include "../aGen.hpp"

int main() {
	ag::Generator gen;

	// header - version
	gen.SetHeader(1, 0);

	size_t nid = gen.AddGlobal("a");

	gen.Function.Create("main");	// define main()
	gen.Function.Create("fib", 1);	// define fib(a)

	gen.Function.SetCurrent("fib");
		gen.Function.PushStack(0);			// 0
		gen.Function.GetArgumentPointer(0);	// arg0 0
		gen.Function.Equal();				// arg0 == 0
		size_t if1 = gen.Function.If();		// if (arg0 == 0)
			gen.Function.PushStack(0);		// 0
			gen.Function.Return();			// return 0;
		gen.Function.SetAddress(if1, gen.Function.GetCurrentAddress());

		gen.Function.PushStack(1);			// 1
		gen.Function.GetArgumentPointer(0);	// arg0 1
		gen.Function.Equal();				// arg0 == 1
		size_t if2 = gen.Function.If();		// if (arg0 == 1)
			gen.Function.PushStack(1);		// 1
			gen.Function.Return();			// return 1;
		gen.Function.SetAddress(if2, gen.Function.GetCurrentAddress());

		gen.Function.GetArgumentPointer(0);	// &arg0
		gen.Function.PushStack(1);			// 1 arg0
		gen.Function.Subtract();			// arg0 - 1
		gen.Function.CallReturn("fib", 1);	// fib(arg0 - 1)

		gen.Function.GetArgumentPointer(0);	// &arg0 fib(arg0 - 1)
		gen.Function.PushStack(2);			// 2 arg0 fib(arg0 - 1)
		gen.Function.Subtract();			// arg0-2 fib(arg0 - 1)
		gen.Function.CallReturn("fib", 1);	// fib(arg0 - 2) fib(arg0 - 1)

		gen.Function.Add();					// fib(arg0 - 1) + fib(arg0 - 2)

		gen.Function.Return();				// return fib(arg0 - 1) + fiv(arg0 - 2);

	gen.Function.SetCurrent("main");
		gen.Function.GetGlobal(nid);		// a
		gen.Function.CallReturn("fib", 1);	// fib(a)
		gen.Function.Return();				// return fib(a)

	ag::ByteCode bc = gen.Get();	// get bytecode

	std::ofstream file("fib.bv", std::ios::out | std::ios::binary);
	file.write(reinterpret_cast<const char *>(bc.Get().data()), bc.Count());

	return 0;
}