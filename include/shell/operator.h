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

#include <phlib/types.h>

#define OP_UNARY   0x00000000 /**< 00 */
#define OP_BINARY  0x40000000 /**< 01 */
#define OP_TERNARY 0x80000000 /**< 10 */

#define OP_TYPE_INCDEC 0x00000000 /**< 000 */
#define OP_TYPE_BIT    0x01000000 /**< 001 */
#define OP_TYPE_ARITHM 0x02000000 /**< 010 */
#define OP_TYPE_PRIOR  0x03000000 /**< 011 */
#define OP_TYPE_COMP   0x04000000 /**< 100 */
#define OP_TYPE_LOGIC  0x05000000 /**< 101 */

#define OP_PRECED_0  0x000000
#define OP_PRECED_1  0x100000
#define OP_PRECED_2  0x200000
#define OP_PRECED_3  0x300000
#define OP_PRECED_4  0x400000
#define OP_PRECED_5  0x500000
#define OP_PRECED_6  0x600000
#define OP_PRECED_7  0x700000
#define OP_PRECED_8  0x800000
#define OP_PRECED_9  0x900000
#define OP_PRECED_10 0xa00000
#define OP_PRECED_11 0xb00000
#define OP_PRECED_12 0xc00000
#define OP_PRECED_13 0xd00000
#define OP_PRECED_14 0xe00000
#define OP_PRECED_15 0xf00000

#define OP_ASSOC_LTR 0x00000
#define OP_ASSOC_RTL 0x40000

#define OP_ARITY_MASK         0xc0000000
#define OP_TYPE_MASK          0x3f000000
#define OP_PRECEDENCE_MASK    0x00f80000
#define OP_ASSOCIATIVITY_MASK 0x00040000
#define OP_ID_MASK            0x0000ffff

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
 *
 */
typedef enum Operator {

    POST_INCREMENT      = OP_UNARY  | OP_TYPE_INCDEC | OP_PRECED_1  | OP_ASSOC_LTR | 0x00,  /**< <div>  ++ </div> */
    POST_DECREMENT      = OP_UNARY  | OP_TYPE_INCDEC | OP_PRECED_1  | OP_ASSOC_LTR | 0x01,  /**< <div> -- </div> */
    BITWISE_NOT         = OP_UNARY  | OP_TYPE_BIT    | OP_PRECED_3  | OP_ASSOC_RTL | 0x30,  /**< <div> ~ </div> */
    LOGICAL_NOT         = OP_UNARY  | OP_TYPE_LOGIC  | OP_PRECED_10 | OP_ASSOC_RTL | 0x50,  /**< <div> ! / not </div> */
    MULTIPLICATION      = OP_BINARY | OP_TYPE_ARITHM | OP_PRECED_6  | OP_ASSOC_LTR | 0x22,  /**< <div> * </div> */
    DIVISION            = OP_BINARY | OP_TYPE_ARITHM | OP_PRECED_6  | OP_ASSOC_LTR | 0x23,  /**< <div> / </div>*/
    ADDITION            = OP_BINARY | OP_TYPE_ARITHM | OP_PRECED_7  | OP_ASSOC_LTR | 0x20,  /**< <div> + </div> */
    SUBTRACTION         = OP_BINARY | OP_TYPE_ARITHM | OP_PRECED_7  | OP_ASSOC_LTR | 0x21,  /**< <div> - </div> */
    ASSIGNMENT          = OP_BINARY | OP_TYPE_ARITHM | OP_PRECED_15 | OP_ASSOC_RTL | 0x10,  /**< <div> = </div>*/
    BITWISE_SHIFT_LEFT  = OP_BINARY | OP_TYPE_BIT    | OP_PRECED_2  | OP_ASSOC_LTR | 0x34,  /**< <div> << </div>*/
    BITWISE_SHIFT_RIGHT = OP_BINARY | OP_TYPE_BIT    | OP_PRECED_2  | OP_ASSOC_LTR | 0x35,  /**< <div> >> </div>*/
    BITWISE_AND         = OP_BINARY | OP_TYPE_BIT    | OP_PRECED_4  | OP_ASSOC_LTR | 0x31,  /**< <div> /\ </div>*/
    BITWISE_OR          = OP_BINARY | OP_TYPE_BIT    | OP_PRECED_5  | OP_ASSOC_LTR | 0x32,  /**< <div> \/ </div>*/
    BITWISE_XOR         = OP_BINARY | OP_TYPE_BIT    | OP_PRECED_5  | OP_ASSOC_LTR | 0x33,  /**< <div> \'/ </div>*/
    PAREN_OPEN          = OP_BINARY | OP_TYPE_PRIOR  | OP_PRECED_1  | OP_ASSOC_LTR | 0x60,  /**< <div> ( </div>*/
    PAREN_CLOSE         = OP_BINARY | OP_TYPE_PRIOR  | OP_PRECED_1  | OP_ASSOC_LTR | 0x61,  /**< <div> ) </div>*/
    EQUAL_TO            = OP_BINARY | OP_TYPE_COMP   | OP_PRECED_14 | OP_ASSOC_LTR | 0x40,  /**< <div> == </div>*/
    NOT_EQUAL_TO        = OP_BINARY | OP_TYPE_COMP   | OP_PRECED_14 | OP_ASSOC_LTR | 0x41,  /**< <div> != </div>*/
    GREATER_THAN        = OP_BINARY | OP_TYPE_COMP   | OP_PRECED_14 | OP_ASSOC_LTR | 0x42,  /**< <div> > </div>*/
    LESS_THAN           = OP_BINARY | OP_TYPE_COMP   | OP_PRECED_14 | OP_ASSOC_LTR | 0x43,  /**< <div> < </div>*/
    NOT_GREATER_THAN    = OP_BINARY | OP_TYPE_COMP   | OP_PRECED_14 | OP_ASSOC_LTR | 0x44,  /**< <div> <= </div>*/
    NOT_LESS_THAN       = OP_BINARY | OP_TYPE_COMP   | OP_PRECED_14 | OP_ASSOC_LTR | 0x45,  /**< <div> >= </div>*/
    LOGICAL_AND         = OP_BINARY | OP_TYPE_LOGIC  | OP_PRECED_11 | OP_ASSOC_LTR | 0x51,  /**< <div> and </div>*/
    LOGICAL_OR          = OP_BINARY | OP_TYPE_LOGIC  | OP_PRECED_12 | OP_ASSOC_LTR | 0x52,  /**< <div> or </div>*/
    LOGICAL_XOR         = OP_BINARY | OP_TYPE_LOGIC  | OP_PRECED_12 | OP_ASSOC_LTR | 0x53,  /**< <div> xor </div>*/
    LOGICAL_IMPLICATION = OP_BINARY | OP_TYPE_LOGIC  | OP_PRECED_13 | OP_ASSOC_LTR | 0x54   /**< <div> -> </div>*/

} Operator;

/**
 * @brief Return unique id of given operator
 * @param oper is operator to get id of
 * @return id of @p oper
 */
uint16 get_operator_id(Operator oper);

/**
 * @brief Check if operator is left-to-right associative
 * @param oper to check
 * @return true if operator @p oper has left-to-right associativity
 */
bool is_ltr_associative(Operator oper);

/**
 * @brief Gets operator arity
 * @param oper to get info about
 * @return 1 if operator @p oper is unary
 * @return 2 if operator @p oper is binary
 * @return 3 if operator @p oper is ternary
 */
uint8 get_operator_arity(Operator oper);

/**
 * @brief Checks, whether operator is unary
 * @param oper to get info about
 * @return true if operator @p oper is unary, false otherwise
 */
bool is_unary_operator(Operator oper);

/**
 * @brief Checks, whether operator is binary
 * @param oper to get info about
 * @return true if operator @p oper is binary, false otherwise
 */
bool is_binary_operator(Operator oper);

/**
 * @brief Checks, whether operator is ternary
 * @param oper is an Operator to get info about
 * @return true if operator @p oper is ternary, false otherwise
 */
bool is_ternary_operator(Operator oper);

/**
 * @brief Get precedence of operator
 * @param oper is an Operator to get info about
 * @return uint8 with precedence [0:15]
 */
uint8 get_operator_precedence(Operator oper);

/**
 * @brief Checks if this operator is increment or decrement operator
 * @param oper is an Operator to to check
 * @return true if @p oper is increment or decrement
 */
bool is_incdec_operator(Operator oper);

/**
 * @brief Checks if this operator is bitwise operator
 * @param oper is an Operator to to check
 * @return true if @p oper is bitwise
 */
bool is_bitwise_operator(Operator oper);

/**
 * @brief Checks if this operator is arithmetic operator
 * @param oper to check
 * @return true if @p oper is arithmetic
 */
bool is_arithmetic_operator(Operator oper);

/**
 * @brief Checks if this operator is priority operator
 * @param oper to check
 * @return true if @p oper is priority
 */
bool is_priority_operator(Operator oper);

/**
 * @brief Checks if this operator is comparison operator
 * @param oper to check
 * @return true if @p oper is comparison
 */
bool is_comparison_operator(Operator oper);

/**
 * @brief Checks if this operator is logical operator
 * @param oper to check
 * @return true if @p oper is logical
 */
bool is_logical_operator(Operator oper);

} //namespace psh

/*
 *  IMPLEMENTATIONS OF INLINE FUNCTIONS ARE BELOW
 */

inline uint16 psh::get_operator_id(psh::Operator oper) {
    return oper & OP_ID_MASK;
}

inline bool psh::is_ltr_associative(psh::Operator oper) {
    return (oper & OP_ASSOCIATIVITY_MASK) == OP_ASSOC_LTR;
}

inline uint8 psh::get_operator_arity(psh::Operator oper) {
    ubyte arity_bits = (oper & OP_ARITY_MASK);
    return (arity_bits >> 30);
}

inline bool psh::is_unary_operator(psh::Operator oper) {
    return psh::get_operator_arity(oper) == 1;
}

inline bool psh::is_binary_operator(psh::Operator oper) {
    return psh::get_operator_arity(oper) == 2;
}

inline bool psh::is_ternary_operator(psh::Operator oper) {
    return psh::get_operator_arity(oper) == 3;
}

inline uint8 psh::get_operator_precedence(psh::Operator oper) {
    return ((oper & OP_PRECEDENCE_MASK) >> 8) & 0x8000;
}

inline bool psh::is_incdec_operator(psh::Operator oper) {
    return (oper & OP_TYPE_MASK) == OP_TYPE_INCDEC;
}

inline bool psh::is_bitwise_operator(psh::Operator oper) {
    return (oper & OP_TYPE_MASK) == OP_TYPE_BIT;
}

inline bool psh::is_arithmetic_operator(psh::Operator oper) {
    return (oper & OP_TYPE_MASK) == OP_TYPE_ARITHM;
}

inline bool psh::is_priority_operator(psh::Operator oper) {
    return (oper & OP_TYPE_MASK) == OP_TYPE_PRIOR;
}

inline bool psh::is_comparison_operator(psh::Operator oper) {
    return (oper & OP_TYPE_MASK) == OP_TYPE_COMP;
}

inline bool psh::is_logical_operator(psh::Operator oper) {
    return (oper & OP_TYPE_MASK) == OP_TYPE_LOGIC;
}

#endif //PHANTOMSHELL_OPERATOR_H
