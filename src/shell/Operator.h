/**
 * @headerfile
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/18/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_OPERATOR_H
#define PHANTOMSHELL_OPERATOR_H

#include "Types.h"

#define OP_UNARY   0x00000000u /**< 00 */
#define OP_BINARY  0x40000000u /**< 01 */
#define OP_TERNARY 0x80000000u /**< 10 */

#define OP_TYPE_INCDEC 0x00000000u /**< 000 */
#define OP_TYPE_BIT    0x01000000u /**< 001 */
#define OP_TYPE_ARITHM 0x02000000u /**< 010 */
#define OP_TYPE_PRIOR  0x03000000u /**< 011 */
#define OP_TYPE_COMP   0x04000000u /**< 100 */
#define OP_TYPE_LOGIC  0x05000000u /**< 101 */

#define OP_PRECED_0  0x000000u
#define OP_PRECED_1  0x100000u
#define OP_PRECED_2  0x200000u
#define OP_PRECED_3  0x300000u
#define OP_PRECED_4  0x400000u
#define OP_PRECED_5  0x500000u
#define OP_PRECED_6  0x600000u
#define OP_PRECED_7  0x700000u
#define OP_PRECED_8  0x800000u
#define OP_PRECED_9  0x900000u
#define OP_PRECED_10 0xa00000u
#define OP_PRECED_11 0xb00000u
#define OP_PRECED_12 0xc00000u
#define OP_PRECED_13 0xd00000u
#define OP_PRECED_14 0xe00000u
#define OP_PRECED_15 0xf00000u

#define OP_ASSOC_LTR 0x00000u
#define OP_ASSOC_RTL 0x40000u

#define OP_ARITY_MASK         0xc0000000u
#define OP_TYPE_MASK          0x3f000000u
#define OP_PRECEDENCE_MASK    0x00f80000u
#define OP_ASSOCIATIVITY_MASK 0x00040000u
#define OP_ID_MASK            0x0000ffffu

#define OP_ARITY_SHIFT       30u
#define OP_PRECEDENCE_SHIFT  8u


/** @brief Default Shell namespace */
namespace psh {

/**
 * @brief Describes all operators used in Phantom Shell
 *
 * Only inline function are used for operations with enum Operator,
 * due to this reason we do not have to use .cpp file
 *
 *  [31:30]   [29:24]    [23:19]       [18]         [17:16]   [15:0]
 *   arity     type    precedence  associativity   reserved     id
 *
 *  Arity: @n
 *   @ref Operator::OP_UNARY - unary operator, @n
 *   @ref Operator::OP_BINARY - binary operator, @n
 *   @ref Operator::OP_TERNARY - ternary operator, uses type field to be defined, @n
 *
 *  Types: @n
 *   @ref Operator::OP_TYPE_INCDEC - increment decrement operators, @n
 *   @ref Operator::OP_TYPE_BIT - bitwise operators, @n
 *   @ref Operator::OP_TYPE_ARITHM - arithmetic operators, @n
 *   @ref Operator::OP_TYPE_PRIOR - priority operators, @n
 *   @ref Operator::OP_TYPE_COMP - comparison operators, @n
 *   @ref Operator::OP_TYPE_LOGIC - logical operators, @n
 *
 *  Associativity: @n
 *   @ref Operator::OP_ASSOC_LTR - left-to-right associativity, @n
 *   @ref Operator::OP_ASSOC_RTL - right-to-left associativity, @n
 *
 *  Precedence: from 0 up to 15, shifted left by 8 bits @n
 *   0  - highest precedence, @n
 *   15 - lowest precedence, @n
 *
 *  Id: from 0x00 up to 0x7f including @n
 *   id can be used to execute these operators in O(1) time
 *
 * @formatter:off
 */
typedef enum Operator {

    POST_INCREMENT      = OP_UNARY  | OP_TYPE_INCDEC | OP_PRECED_1  | OP_ASSOC_LTR | 0x01u,  /**< <div> ++  </div> */
    POST_DECREMENT      = OP_UNARY  | OP_TYPE_INCDEC | OP_PRECED_1  | OP_ASSOC_LTR | 0x02u,  /**< <div> --  </div> */
    BITWISE_NOT         = OP_UNARY  | OP_TYPE_BIT    | OP_PRECED_3  | OP_ASSOC_RTL | 0x03u,  /**< <div>  ~  </div> */
    LOGICAL_NOT         = OP_UNARY  | OP_TYPE_LOGIC  | OP_PRECED_10 | OP_ASSOC_RTL | 0x04u,  /**< <div> ! / not </div> */
    MULTIPLICATION      = OP_BINARY | OP_TYPE_ARITHM | OP_PRECED_6  | OP_ASSOC_LTR | 0x05u,  /**< <div>  *  </div> */
    DIVISION            = OP_BINARY | OP_TYPE_ARITHM | OP_PRECED_6  | OP_ASSOC_LTR | 0x06u,  /**< <div>  /  </div>*/
    ADDITION            = OP_BINARY | OP_TYPE_ARITHM | OP_PRECED_7  | OP_ASSOC_LTR | 0x07u,  /**< <div>  +  </div> */
    SUBTRACTION         = OP_BINARY | OP_TYPE_ARITHM | OP_PRECED_7  | OP_ASSOC_LTR | 0x08u,  /**< <div>  -  </div> */
    ASSIGNMENT          = OP_BINARY | OP_TYPE_ARITHM | OP_PRECED_15 | OP_ASSOC_RTL | 0x09u,  /**< <div>  =  </div>*/
    BITWISE_SHIFT_LEFT  = OP_BINARY | OP_TYPE_BIT    | OP_PRECED_2  | OP_ASSOC_LTR | 0x0au,  /**< <div> <<  </div>*/
    BITWISE_SHIFT_RIGHT = OP_BINARY | OP_TYPE_BIT    | OP_PRECED_2  | OP_ASSOC_LTR | 0x0bu,  /**< <div> >>  </div>*/
    BITWISE_AND         = OP_BINARY | OP_TYPE_BIT    | OP_PRECED_4  | OP_ASSOC_LTR | 0x0cu,  /**< <div> /\  </div>*/
    BITWISE_OR          = OP_BINARY | OP_TYPE_BIT    | OP_PRECED_5  | OP_ASSOC_LTR | 0x0du,  /**< <div> \/  </div>*/
    BITWISE_XOR         = OP_BINARY | OP_TYPE_BIT    | OP_PRECED_5  | OP_ASSOC_LTR | 0x0eu,  /**< <div> \'/ </div>*/
    PAREN_OPEN          = OP_BINARY | OP_TYPE_PRIOR  | OP_PRECED_1  | OP_ASSOC_LTR | 0x0fu,  /**< <div>  (  </div>*/
    PAREN_CLOSE         = OP_BINARY | OP_TYPE_PRIOR  | OP_PRECED_1  | OP_ASSOC_LTR | 0x10u,  /**< <div>  )  </div>*/
    EQUAL_TO            = OP_BINARY | OP_TYPE_COMP   | OP_PRECED_14 | OP_ASSOC_LTR | 0x11u,  /**< <div> ==  </div>*/
    NOT_EQUAL_TO        = OP_BINARY | OP_TYPE_COMP   | OP_PRECED_14 | OP_ASSOC_LTR | 0x12u,  /**< <div> !=  </div>*/
    GREATER_THAN        = OP_BINARY | OP_TYPE_COMP   | OP_PRECED_14 | OP_ASSOC_LTR | 0x13u,  /**< <div>  >  </div>*/
    LESS_THAN           = OP_BINARY | OP_TYPE_COMP   | OP_PRECED_14 | OP_ASSOC_LTR | 0x14u,  /**< <div>  <  </div>*/
    NOT_GREATER_THAN    = OP_BINARY | OP_TYPE_COMP   | OP_PRECED_14 | OP_ASSOC_LTR | 0x15u,  /**< <div> <=  </div>*/
    NOT_LESS_THAN       = OP_BINARY | OP_TYPE_COMP   | OP_PRECED_14 | OP_ASSOC_LTR | 0x16u,  /**< <div> >=  </div>*/
    LOGICAL_AND         = OP_BINARY | OP_TYPE_LOGIC  | OP_PRECED_11 | OP_ASSOC_LTR | 0x17u,  /**< <div> and </div>*/
    LOGICAL_OR          = OP_BINARY | OP_TYPE_LOGIC  | OP_PRECED_12 | OP_ASSOC_LTR | 0x18u,  /**< <div> or  </div>*/
    LOGICAL_XOR         = OP_BINARY | OP_TYPE_LOGIC  | OP_PRECED_12 | OP_ASSOC_LTR | 0x19u,  /**< <div> xor </div>*/
    LOGICAL_IMPLICATION = OP_BINARY | OP_TYPE_LOGIC  | OP_PRECED_13 | OP_ASSOC_LTR | 0x1au   /**< <div> ->  </div>*/

} Operator;

/**
 * @brief Return unique id of given operator
 * @param oper is operator to get id of
 * @return id of @p oper
 */
UInt16 GetOperatorId(Operator oper);

/**
 * @brief Check if operator is left-to-right associative
 * @param oper to check
 * @return true if operator @p oper has left-to-right associativity
 */
bool IsLtRAssociative(Operator oper);

/**
 * @brief Gets operator arity
 * @param oper to get info about
 * @return 1 if operator @p oper is unary
 * @return 2 if operator @p oper is binary
 * @return 3 if operator @p oper is ternary
 */
UInt8 GetOperatorArity(Operator oper);

/**
 * @brief Checks, whether operator is unary
 * @param oper to get info about
 * @return true if operator @p oper is unary, false otherwise
 */
bool IsUnaryOperator(Operator oper);

/**
 * @brief Checks, whether operator is binary
 * @param oper to get info about
 * @return true if operator @p oper is binary, false otherwise
 */
bool IsBinaryOperator(Operator oper);

/**
 * @brief Checks, whether operator is ternary
 * @param oper is an Operator to get info about
 * @return true if operator @p oper is ternary, false otherwise
 */
bool IsTernaryOperator(Operator oper);

/**
 * @brief Get precedence of operator
 * @param oper is an Operator to get info about
 * @return uint8 with precedence [0:15]
 */
UInt8 GetOperatorPrecedence(Operator oper);

/**
 * @brief Checks if this operator is increment or decrement operator
 * @param oper is an Operator to to check
 * @return true if @p oper is increment or decrement
 */
bool IsIncrementOrDecrementOperator(Operator oper);

/**
 * @brief Checks if this operator is bitwise operator
 * @param oper is an Operator to to check
 * @return true if @p oper is bitwise
 */
bool IsBitwiseOperator(Operator oper);

/**
 * @brief Checks if this operator is arithmetic operator
 * @param oper to check
 * @return true if @p oper is arithmetic
 */
bool IsArithmeticOperator(Operator oper);

/**
 * @brief Checks if this operator is priority operator
 * @param oper to check
 * @return true if @p oper is priority
 */
bool IsPriorityOperator(Operator oper);

/**
 * @brief Checks if this operator is comparison operator
 * @param oper to check
 * @return true if @p oper is comparison
 */
bool IsComparisonOperator(Operator oper);

/**
 * @brief Checks if this operator is logical operator
 * @param oper to check
 * @return true if @p oper is logical
 */
bool IsLogicalOperator(Operator oper);

/**
 * @brief Converts operator code to string
 * @param oper to convert
 * @return char* name of operator
 */
const char* OperatorToString(Operator oper);

/**
 * @brief Converts operator code to symbol, representing this operator
 * @param oper to convert
 * @return char* symbol of operator
 */
const char* OperatorToSymbol(Operator oper);

} //namespace psh

/*
 *  IMPLEMENTATIONS OF INLINE FUNCTIONS ARE BELOW
 *  @formatter:on
 */

inline UInt16 psh::GetOperatorId(psh::Operator oper) {
    return oper & OP_ID_MASK;
}

inline bool psh::IsLtRAssociative(psh::Operator oper) {
    return (oper & OP_ASSOCIATIVITY_MASK) == OP_ASSOC_LTR;
}

inline UInt8 psh::GetOperatorArity(psh::Operator oper) {
    UByte arityBits = (oper & OP_ARITY_MASK);
    return (arityBits >> OP_ARITY_SHIFT);
}

inline bool psh::IsUnaryOperator(psh::Operator oper) {
    return psh::GetOperatorArity(oper) == 1;
}

inline bool psh::IsBinaryOperator(psh::Operator oper) {
    return psh::GetOperatorArity(oper) == 2;
}

inline bool psh::IsTernaryOperator(psh::Operator oper) {
    return psh::GetOperatorArity(oper) == 3;
}

inline UInt8 psh::GetOperatorPrecedence(psh::Operator oper) {
    const auto result_mask = 0x8000u;
    return ((oper & OP_PRECEDENCE_MASK) >> OP_PRECEDENCE_SHIFT) & result_mask;
}

inline bool psh::IsIncrementOrDecrementOperator(psh::Operator oper) {
    return (oper & OP_TYPE_MASK) == OP_TYPE_INCDEC;
}

inline bool psh::IsBitwiseOperator(psh::Operator oper) {
    return (oper & OP_TYPE_MASK) == OP_TYPE_BIT;
}

inline bool psh::IsArithmeticOperator(psh::Operator oper) {
    return (oper & OP_TYPE_MASK) == OP_TYPE_ARITHM;
}

inline bool psh::IsPriorityOperator(psh::Operator oper) {
    return (oper & OP_TYPE_MASK) == OP_TYPE_PRIOR;
}

inline bool psh::IsComparisonOperator(psh::Operator oper) {
    return (oper & OP_TYPE_MASK) == OP_TYPE_COMP;
}

inline bool psh::IsLogicalOperator(psh::Operator oper) {
    return (oper & OP_TYPE_MASK) == OP_TYPE_LOGIC;
}

inline const char *psh::OperatorToString(psh::Operator oper) {
    switch (oper) {
        case EQUAL_TO: return "EQUAL_TO";
        case ADDITION: return "ADDITION";
        case DIVISION: return "DIVISION";
        case LESS_THAN: return "LESS_THAN";
        case ASSIGNMENT: return "ASSIGNMENT";
        case PAREN_OPEN: return "PAREN_OPEN";
        case BITWISE_OR: return "BITWISE_OR";
        case LOGICAL_OR: return "LOGICAL_OR";
        case SUBTRACTION: return "SUBTRACTION";
        case BITWISE_NOT: return "BITWISE_NOT";
        case LOGICAL_NOT: return "LOGICAL_NOT";
        case BITWISE_AND: return "BITWISE_AND";
        case BITWISE_XOR: return "BITWISE_XOR";
        case PAREN_CLOSE: return "PAREN_CLOSE";
        case LOGICAL_AND: return "LOGICAL_AND";
        case LOGICAL_XOR: return "LOGICAL_XOR";
        case NOT_EQUAL_TO: return "NOT_EQUAL_TO";
        case GREATER_THAN: return "GREATER_THAN";
        case NOT_LESS_THAN: return "NOT_LESS_THAN";
        case MULTIPLICATION: return "MULTIPLICATION";
        case POST_INCREMENT: return "POST_INCREMENT";
        case POST_DECREMENT: return "POST_DECREMENT";
        case NOT_GREATER_THAN: return "NOT_GREATER_THAN";
        case BITWISE_SHIFT_LEFT: return "BITWISE_SHIFT_LEFT";
        case BITWISE_SHIFT_RIGHT: return "BITWISE_SHIFT_RIGHT";
        case LOGICAL_IMPLICATION: return "LOGICAL_IMPLICATION";
        default: return "NOT AN OPERATOR";
    }
}

inline const char *psh::OperatorToSymbol(psh::Operator oper) {
    switch (oper) {
        case EQUAL_TO: return "==";
        case ADDITION: return "+";
        case DIVISION: return "/";
        case LESS_THAN: return "<";
        case ASSIGNMENT: return "=";
        case PAREN_OPEN: return "(";
        case BITWISE_OR: return "\\/";
        case LOGICAL_OR: return "or";
        case SUBTRACTION: return "-";
        case BITWISE_NOT: return "~";
        case LOGICAL_NOT: return "!";
        case BITWISE_AND: return "";
        case BITWISE_XOR: return "\\'/";
        case PAREN_CLOSE: return ")";
        case LOGICAL_AND: return "and";
        case LOGICAL_XOR: return "xor";
        case NOT_EQUAL_TO: return "!=";
        case GREATER_THAN: return ">";
        case NOT_LESS_THAN: return ">=";
        case MULTIPLICATION: return "*";
        case POST_INCREMENT: return "++";
        case POST_DECREMENT: return "--";
        case NOT_GREATER_THAN: return "<=";
        case BITWISE_SHIFT_LEFT: return "<<";
        case BITWISE_SHIFT_RIGHT: return ">>";
        case LOGICAL_IMPLICATION: return "->";
        default: return " ";
    }
}

#endif //PHANTOMSHELL_OPERATOR_H
