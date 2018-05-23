#include <iostream>
#include <fstream>
#include "../aGen.hpp"

int main() {
	ag::Generator gen;

	// header - version
	gen.SetHeader(1, 0);

	gen.Function.Create("main");	// define main()

	gen.Function.SetCurrent("main");
		gen.Function.PushStack(5);	// 5
		gen.Function.PushStack(2);	// 2 5
		gen.Function.Add();			// 5 + 2
		gen.Function.Return();		// return 5 + 2

	ag::ByteCode bc = gen.Get();	// get bytecode

	std::ofstream file("sum.bv", std::ios::out | std::ios::binary);
	file.write(reinterpret_cast<const char *>(bc.Get().data()), bc.Count());

	return 0;
}