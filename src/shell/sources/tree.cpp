/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 12/16/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include "tree.h"

using namespace psh;

DeclarationNode::DeclarationNode() : BaseTreeNode() {
    this->node_type             = TreeNodeType::DECLARATION_NODE;
    this->declaration_node_type = (DeclarationNodeType) 0;
}

FuncArgDeclarationNode::FuncArgDeclarationNode(Type argument_type, phlib::String &argument_name)
        : DeclarationNode() {
    this->declaration_node_type = DeclarationNodeType::FUNC_ARG_DECLARATION_NODE;
    this->argument_type         = argument_type;
    this->argument_name         = argument_name;
}

FuncDeclarationNode::FuncDeclarationNode(phlib::String &function_name, FuncArgDeclarationNode *arguments)
        : DeclarationNode() {
    this->declaration_node_type = DeclarationNodeType::FUNCTION_DECLARATION_NODE;
}

ClassDeclarationNode::ClassDeclarationNode(phlib::String &class_name)
        : DeclarationNode() {
    this->declaration_node_type = DeclarationNodeType::CLASS_DECLARATION_NODE;
}

//Expressions

ExpressionNode::ExpressionNode() : BaseTreeNode() {
    node_type            = TreeNodeType::EXPRESSION_NODE;
    expression_node_type = (ExpressionNodeType) 0;
}

ConstantExpressionNode::ConstantExpressionNode(LiteralNode *literal)
        : ExpressionNode() {
    this->expression_node_type = ExpressionNodeType::CONSTANT_EXPRESSION_NODE;
    this->literal              = literal;
}

UnaryExpressionNode::UnaryExpressionNode(Operator oper, ExpressionNode *operand)
        : ExpressionNode() {
    this->expression_node_type = ExpressionNodeType::UNARY_EXPRESSION_NODE;
    this->oper                 = oper;
    this->operand              = operand;
}

BinaryExpressionNode::BinaryExpressionNode(Operator oper, ExpressionNode *operand1, ExpressionNode *operand2)
        : ExpressionNode() {
    this->expression_node_type = ExpressionNodeType::BINARY_EXPRESSION_NODE;
    this->oper                 = oper;
    this->operand1             = operand1;
    this->operand2             = operand2;
}

TernaryExpressionNode::TernaryExpressionNode(Operator oper, ExpressionNode *operand1,
                                             ExpressionNode *operand2, ExpressionNode *operand3)
        : ExpressionNode() {
    this->expression_node_type = ExpressionNodeType::TERNARY_EXPRESSION_NODE;
    this->oper                 = oper;
    this->operand1             = operand1;
    this->operand2             = operand2;
    this->operand3             = operand3;
}

IdentifierExpressionNode::IdentifierExpressionNode()
        : ExpressionNode() {
    expression_node_type = ExpressionNodeType::IDENTIFIER_EXPRESSION_NODE;
}

//Literals

LiteralNode::LiteralNode() : BaseTreeNode() {
    this->node_type         = TreeNodeType::LITERAL_NODE;
    this->literal_node_type = (LiteralNodeType) 0;
}

BooleanLiteralNode::BooleanLiteralNode(bool value)
        : LiteralNode() {
    this->literal_node_type = LiteralNodeType::BOOLEAN_LITERAL_NODE;
}

IntegerLiteralNode::IntegerLiteralNode(int value)
        : LiteralNode() {
    this->literal_node_type = LiteralNodeType::INTEGER_LITERAL_NODE;
}

FloatLiteralNode::FloatLiteralNode(float value)
        : LiteralNode() {
    this->literal_node_type = LiteralNodeType::FLOAT_LITERAL_NODE;
}

CharacterLiteralNode::CharacterLiteralNode(char value)
        : LiteralNode() {
    this->literal_node_type = LiteralNodeType::CHARACTER_LITERAL_NODE;
}

StringLiteralNode::StringLiteralNode(phlib::String *value)
        : LiteralNode() {
    this->literal_node_type = LiteralNodeType::STRING_LITERAL_NODE;
}

//Statements

StatementNode::StatementNode() : BaseTreeNode() {
    node_type = TreeNodeType::STATEMENT_NODE;
    this->statement_node_type = (StatementNodeType) 0;
}

DefVarStatementNode::DefVarStatementNode(IdentifierExpressionNode *identifier, ExpressionNode *value)
        : StatementNode() {
    this->statement_node_type = StatementNodeType::DEF_VAR_STATEMENT_NODE;
    this->identifier          = identifier;
    this->value               = value;
}

ExpressionStatementNode::ExpressionStatementNode(ExpressionNode *expression)
        : StatementNode() {
    this->statement_node_type = StatementNodeType::EXPRESSION_STATEMENT_NODE;
    this->expression          = expression;
}

IfStatementNode::IfStatementNode(ExpressionNode *expression, StatementNode *true_statement,
                                 StatementNode *false_statement)
        : StatementNode() {
    this->statement_node_type = StatementNodeType::IF_SELECTION_NODE;
    this->expression          = expression;
    this->true_statement      = true_statement;
    this->false_statement     = false_statement;
}

IfStatementNode::IfStatementNode(ExpressionNode *expression, StatementNode *true_statement)
        : IfStatementNode(expression,true_statement,nullptr) {
}

CaseSelectionNode::CaseSelectionNode(ConstantExpressionNode *constant_expression, StatementNode *statement,
                                     CaseSelectionNode *next_case)
        : StatementNode() {
    this->statement_node_type = StatementNodeType::CASE_SELECTION_NODE;
    this->constant_expression = constant_expression;
    this->statement           = statement;
    this->next_case           = next_case;
}

SwitchStatementNode::SwitchStatementNode(ExpressionNode *expression, CaseSelectionNode *case_statement,
                                         StatementNode *other_statement)
        : StatementNode() {
    this->statement_node_type = StatementNodeType::SWITCH_SELECTION_NODE;
    this->expression          = expression;
    this->case_statement      = case_statement;
    this->other_statement     = other_statement;
}

WhileStatementNode::WhileStatementNode(ExpressionNode *expression, StatementNode *body_statement)
        : StatementNode() {
    this->statement_node_type = StatementNodeType::WHILE_ITERATION_NODE;
    this->expression          = expression;
    this->body_statement      = body_statement;
}

DoWhileStatementNode::DoWhileStatementNode(ExpressionNode *expression, StatementNode *body_statement)
        : StatementNode() {
    this->statement_node_type = StatementNodeType::DO_WHILE_ITERATION_NODE;
    this->expression          = expression;
    this->body_statement      = body_statement;
}

ForStatementNode::ForStatementNode(IdentifierExpressionNode *element, IdentifierExpressionNode *list,
                                   StatementNode *body_statement)
        : StatementNode() {
    this->statement_node_type = StatementNodeType::FOR_ITERATION_NODE;
    this->element             = element;
    this->list                = list;
    this->body_statement      = body_statement;
}
