//
// Created by ilya on 12/16/18.
//

#ifndef PHANTOMSHELL_TREE_H
#define PHANTOMSHELL_TREE_H

#include "string.h"
#include "operator.h"
#include "psh_types.h"

namespace psh {

/**
 * @brief Basic AST node types
 */
typedef enum TreeNodeType {
    DECLARATION_NODE = 0x01,
    EXPRESSION_NODE  = 0x02,
    LITERAL_NODE     = 0x03,
    STATEMENT_NODE   = 0x04,
} TreeNodeType;

/**
 * @brief Subtypes of literal nodes
 */
typedef enum LiteralNodeType {
    BOOLEAN_LITERAL_NODE   = 0x01,
    INTEGER_LITERAL_NODE   = 0x02,
    FLOAT_LITERAL_NODE     = 0x03,
    CHARACTER_LITERAL_NODE = 0x04,
    STRING_LITERAL_NODE    = 0x05,
} LiteralNodeType;

/**
 * @brief Subtypes of expression nodes
 */
typedef enum ExpressionNodeType {
    CONSTANT_EXPRESSION_NODE   = 0x01,
    UNARY_EXPRESSION_NODE      = 0x02,
    BINARY_EXPRESSION_NODE     = 0x03,
    TERNARY_EXPRESSION_NODE    = 0x04,
    IDENTIFIER_EXPRESSION_NODE = 0x05
} ExpressionNodeType;

/**
 * @brief Subtypes of statements nodes
 */
typedef enum StatementNodeType {
    DEF_VAR_STATEMENT_NODE    = 0x01,
    EXPRESSION_STATEMENT_NODE = 0x02,
    IF_SELECTION_NODE         = 0x03,
    SWITCH_SELECTION_NODE     = 0x04,
    CASE_SELECTION_NODE       = 0x05,
    WHILE_ITERATION_NODE      = 0x06,
    DO_WHILE_ITERATION_NODE   = 0x07,
    FOR_ITERATION_NODE        = 0x08
} StatementNodeType;

/**
 * @brief subtypes of declaration nodes
 */
typedef enum DeclarationNodeType {
    FUNCTION_DECLARATION_NODE = 0x01,
    FUNC_ARG_DECLARATION_NODE = 0x02,
    CLASS_DECLARATION_NODE    = 0x03
} DeclarationNodeType;

/**
 * @brief Basic class for all AST nodes
 */
class BaseTreeNode {
public:
    TreeNodeType node_type; /**< real node type*/
};

/**
 * @brief This type of nodes can store user-printed literals
 */
class LiteralNode : BaseTreeNode {
public:
    LiteralNodeType literal_node_type;

    LiteralNode();
};

class BooleanLiteralNode : LiteralNode {
public:
    PshBoolean value;

    explicit BooleanLiteralNode(bool value);

    explicit BooleanLiteralNode(PshBoolean &value);
};

class IntegerLiteralNode : LiteralNode {
public:
    PshInteger value;

    explicit IntegerLiteralNode(int value);

    explicit IntegerLiteralNode(PshInteger &value);
};

class FloatLiteralNode : LiteralNode {
public:
    PshFloat value;

    explicit FloatLiteralNode(float value);

    explicit FloatLiteralNode(PshFloat &value);
};

class CharacterLiteralNode : LiteralNode {
public:
    PshCharacter value;

    explicit CharacterLiteralNode(char value);

    explicit CharacterLiteralNode(PshCharacter &value);
};

class StringLiteralNode : LiteralNode {
public:
    PshString value;

    explicit StringLiteralNode(phlib::String &value);

    explicit StringLiteralNode(PshString &value);
};


/**
 * @brief Expression nodes stores any kind of expressions
 */
class ExpressionNode : BaseTreeNode {
public:
    ExpressionNodeType expression_node_type;

    ExpressionNode();
};

/**
 * @brief Wraps literal nodes as constant expression
 */
class ConstantExpressionNode : ExpressionNode {
public:
    LiteralNode *literal;

    explicit ConstantExpressionNode(LiteralNode *literal);
};

/**
 * @brief Stores unary operator and its operand
 */
class UnaryExpressionNode : ExpressionNode {
public:
    Operator       oper;
    ExpressionNode *operand;

    UnaryExpressionNode(Operator oper,
                        ExpressionNode *operand);
};

/**
 * @brief Stores binary operator and its operands
 */
class BinaryExpressionNode : ExpressionNode {
public:
    Operator       oper;
    ExpressionNode *operand1;
    ExpressionNode *operand2;

    BinaryExpressionNode(Operator oper,
                         ExpressionNode *operand1,
                         ExpressionNode *operand2);
};

/**
 * @brief Stores ternary operator and its operands
 */
class TernaryExpressionNode : ExpressionNode {
public:
    Operator       oper;
    ExpressionNode *operand1;
    ExpressionNode *operand2;
    ExpressionNode *operand3;

    TernaryExpressionNode(Operator oper,
                          ExpressionNode *operand1,
                          ExpressionNode *operand2,
                          ExpressionNode *operand3);
};

/**
 * @brief Wraps value, represented as identifier
 */
class IdentifierExpressionNode : ExpressionNode {
public:
    IdentifierExpressionNode();
};


class StatementNode : BaseTreeNode {
public:
    StatementNodeType statement_node_type;

    StatementNode();
};

class DefVarStatementNode : StatementNode {
public:
    IdentifierExpressionNode *identifier;
    ExpressionNode           *value;

    DefVarStatementNode(IdentifierExpressionNode *identifier,
                        ExpressionNode *value);
};

class ExpressionStatementNode : StatementNode {
public:
    ExpressionNode *expression;

    explicit ExpressionStatementNode(ExpressionNode *expression);
};

class IfStatementNode : StatementNode {
public:
    ExpressionNode *expression;
    StatementNode  *true_statement;
    StatementNode  *false_statement;

    IfStatementNode(ExpressionNode *expression,
                    StatementNode *true_statement,
                    StatementNode *false_statement);

    IfStatementNode(ExpressionNode *expression,
                    StatementNode *true_statement);
};

class CaseSelectionNode : StatementNode {
public:
    ConstantExpressionNode *constant_expression;
    StatementNode          *statement;
    CaseSelectionNode      *next_case;

    CaseSelectionNode(ConstantExpressionNode *constant_expression,
                      StatementNode *statement,
                      CaseSelectionNode *next_case);
};

class SwitchStatementNode : StatementNode {
public:
    ExpressionNode    *expression;
    CaseSelectionNode *case_statement;
    StatementNode     *other_statement;

    SwitchStatementNode(ExpressionNode *expression,
                        CaseSelectionNode *case_statement,
                        StatementNode *other_statement);
};

class WhileStatementNode : StatementNode {
public:
    ExpressionNode *expression;
    StatementNode  *body_statement;

    WhileStatementNode(ExpressionNode *expression,
                       StatementNode *body_statement);
};

class DoWhileStatementNode : StatementNode {
public:
    ExpressionNode *expression;
    StatementNode  *body_statement;

    DoWhileStatementNode(ExpressionNode *expression,
                         StatementNode *body_statement);
};

class ForStatementNode : StatementNode {
public:
    IdentifierExpressionNode *element;
    IdentifierExpressionNode *list;
    StatementNode            *body_statement;

    ForStatementNode(IdentifierExpressionNode *element,
                     IdentifierExpressionNode *list,
                     StatementNode *body_statement);
};


class DeclarationNode : BaseTreeNode {
public:
    DeclarationNodeType declaration_node_type;

    DeclarationNode();
};

class FuncArgDeclarationNode : DeclarationNode {
public:
    PshType       argument_type;
    phlib::String argument_name;

    FuncArgDeclarationNode(PshType argument_type,
                           phlib::String &argument_name);
};

class FuncDeclarationNode : DeclarationNode {
public:
    FuncDeclarationNode(phlib::String &function_name,
                        FuncArgDeclarationNode *arguments);
};

class ClassDeclarationNode : DeclarationNode {
public:
    phlib::String class_name;

    ClassDeclarationNode(phlib::String &class_name);
};


}

#endif //PHANTOMSHELL_TREE_H
