#ifndef __AGEN_OP_CODE_H__
#define __AGEN_OP_CODE_H__

namespace ag
{
	enum class OpCode
	{
		Unknown,
		FunctionStart,
		Return,
		PushStack,
		PopStack,
		GetConstant,
		Add,
		Subtract,
		Multiply,
		Divide,
		Increment,
		Decrement,
		Negate,
		Modulo,
		BitOr,
		BitAnd,
		BitNot,
		BitXor,
		BitLeftShift,
		BitRightShift,
		Equal,
		NotEqual,
		Not,
		Greater,
		Less,
		GreaterEqual,
		LessEqual,
		Nop,
		Convert,
		Duplicate,
		Swap,
		GetLocal,
		SetLocal,
		GetGlobal,
		SetGlobal,
		NewArray,
		Call,
		CallReturn,
		IsTypeOf,
		If,
		Goto
	};
}

#endif // __AGEN_OP_CODE_H__