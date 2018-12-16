//
// Created by ilya on 12/16/18.
//

#ifndef PHANTOMSHELL_TREE_H
#define PHANTOMSHELL_TREE_H

#include "string.h"
#include "operator.h"
#include "psh_types.h"

namespace psh {

typedef enum TreeNodeType {
    DECLARATION_NODE = 0x01,
    EXPRESSION_NODE  = 0x02,
    LITERAL_NODE     = 0x03,
    STATEMENT_NODE   = 0x04,
} TreeNodeType;

typedef enum LiteralNodeType {
    BOOLEAN_LITERAL_NODE   = 0x01,
    INTEGER_LITERAL_NODE   = 0x02,
    FLOAT_LITERAL_NODE     = 0x03,
    CHARACTER_LITERAL_NODE = 0x04,
    STRING_LITERAL_NODE    = 0x05,
} LiteralNodeType;

typedef enum ExpressionNodeType {
    CONSTANT_EXPRESSION_NODE   = 0x01,
    UNARY_EXPRESSION_NODE      = 0x02,
    BINARY_EXPRESSION_NODE     = 0x03,
    TERNARY_EXPRESSION_NODE    = 0x04,
    IDENTIFIER_EXPRESSION_NODE = 0x05
} ExpressionNodeType;

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

typedef enum DeclarationNodeType {
    FUNCTION_DECLARATION_NODE = 0x01,
    FUNC_ARG_DECLARATION_NODE = 0x02,
    CLASS_DECLARATION_NODE    = 0x03
} DeclarationNodeType;

class BaseTreeNode {
public:
    TreeNodeType node_type;
};

class LiteralNode : BaseTreeNode {
public:
    LiteralNodeType literal_node_type;

    LiteralNode();
};

class BooleanLiteralNode : LiteralNode {
public:
    explicit BooleanLiteralNode(bool value);
};

class IntegerLiteralNode : LiteralNode {
public:
    explicit IntegerLiteralNode(int value);
};

class FloatLiteralNode : LiteralNode {
public:
    explicit FloatLiteralNode(float value);
};

class CharacterLiteralNode : LiteralNode {
public:
    explicit CharacterLiteralNode(char value);
};

class StringLiteralNode : LiteralNode {
public:
    explicit StringLiteralNode(phlib::String *value);
};



class ExpressionNode : BaseTreeNode {
public:
    ExpressionNodeType expression_node_type;

    ExpressionNode();
};

class ConstantExpressionNode : ExpressionNode {
public:
    LiteralNode *literal;

    explicit ConstantExpressionNode(LiteralNode *literal);
};

class UnaryExpressionNode : ExpressionNode {
public:
    Operator       oper;
    ExpressionNode *operand;

    UnaryExpressionNode(Operator oper,
                        ExpressionNode *operand);
};

class BinaryExpressionNode : ExpressionNode {
public:
    Operator       oper;
    ExpressionNode *operand1;
    ExpressionNode *operand2;

    BinaryExpressionNode(Operator oper,
                         ExpressionNode *operand1,
                         ExpressionNode *operand2);
};

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
    Type          argument_type;
    phlib::String argument_name;

    FuncArgDeclarationNode(Type argument_type,
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
