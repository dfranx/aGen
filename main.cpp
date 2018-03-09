#include <iostream>
#include <fstream>
#include <agen.h>

int main() {
	ag::Generator gen;
	
	// header - version
	gen.SetHeader(0, 1);

	gen.Function.Create("multiply", ag::Type::Int, { ag::Type::Int, ag::Type::Int });
	gen.Function.Create("main", ag::Type::Int, {});

	gen.Function.SetCurrent("main");
		gen.Function.PushStack(2);				// 2
		gen.Function.PushStack(10);				// 10 2
		gen.Function.CallReturn("multiply", 2);	// multiply(10, 2)
		gen.Function.Duplicate();
		gen.Function.Call("print", 1);
		gen.Function.Return();					// return multiply(10, 2)

	gen.Function.SetCurrent("multiply");
		gen.Function.GetArgument(0);// arg0
		gen.Function.GetArgument(1);// arg1 arg0
		gen.Function.Multiply();	// arg1*arg0
		gen.Function.Return();		// return arg1*arg0

	ag::ByteCode bc = gen.Get();// get bytecode

	std::ofstream file("test.bv", std::ios::out | std::ios::binary);
	file.write(reinterpret_cast<const char *>(bc.Get().data()), bc.Count());

	std::cout << "[end]" << std::endl;
	getchar();
	return 0;
}