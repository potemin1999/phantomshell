/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 12/16/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include <Tree.h>

using namespace phlib;
using namespace psh;

DeclarationNode::DeclarationNode() : BaseTreeNode() {
    this->nodeType            = TreeNodeType::DECLARATION_NODE;
    this->declarationNodeType = (DeclarationNodeType) 0;
}

FuncArgDeclarationNode::FuncArgDeclarationNode(PshType argumentType, String &argumentName)
        : DeclarationNode() {
    this->declarationNodeType = DeclarationNodeType::FUNC_ARG_DECLARATION_NODE;
    this->argumentType        = argumentType;
    this->argumentName        = argumentName;
}

FuncDeclarationNode::FuncDeclarationNode(String &functionName, FuncArgDeclarationNode *arguments, int argumentsCount)
        : DeclarationNode() {
    this->declarationNodeType = DeclarationNodeType::FUNCTION_DECLARATION_NODE;
}

ClassDeclarationNode::ClassDeclarationNode(String &className)
        : DeclarationNode() {
    this->declarationNodeType = DeclarationNodeType::CLASS_DECLARATION_NODE;
}

//Expressions

ExpressionNode::ExpressionNode() : BaseTreeNode() {
    this->nodeType           = TreeNodeType::EXPRESSION_NODE;
    this->expressionNodeType = (ExpressionNodeType) 0;
}

ConstantExpressionNode::ConstantExpressionNode(LiteralNode *literal)
        : ExpressionNode() {
    this->expressionNodeType = ExpressionNodeType::CONSTANT_EXPRESSION_NODE;
    this->literal            = literal;
}

UnaryExpressionNode::UnaryExpressionNode(Operator oper, ExpressionNode *operand)
        : ExpressionNode() {
    this->expressionNodeType = ExpressionNodeType::UNARY_EXPRESSION_NODE;
    this->oper               = oper;
    this->operand            = operand;
}

BinaryExpressionNode::BinaryExpressionNode(Operator oper, ExpressionNode *operand1, ExpressionNode *operand2)
        : ExpressionNode() {
    this->expressionNodeType = ExpressionNodeType::BINARY_EXPRESSION_NODE;
    this->oper               = oper;
    this->operand1           = operand1;
    this->operand2           = operand2;
}

TernaryExpressionNode::TernaryExpressionNode(Operator oper, ExpressionNode *operand1,
                                             ExpressionNode *operand2, ExpressionNode *operand3)
        : ExpressionNode() {
    this->expressionNodeType = ExpressionNodeType::TERNARY_EXPRESSION_NODE;
    this->oper               = oper;
    this->operand1           = operand1;
    this->operand2           = operand2;
    this->operand3           = operand3;
}

IdentifierExpressionNode::IdentifierExpressionNode()
        : ExpressionNode() {
    this->expressionNodeType = ExpressionNodeType::IDENTIFIER_EXPRESSION_NODE;
}

//Literals

LiteralNode::LiteralNode() : BaseTreeNode() {
    this->nodeType        = TreeNodeType::LITERAL_NODE;
    this->literalNodeType = (LiteralNodeType) 0;
}

BooleanLiteralNode::BooleanLiteralNode(bool value)
        : LiteralNode(), value(value) {
    this->literalNodeType = LiteralNodeType::BOOLEAN_LITERAL_NODE;
}

IntegerLiteralNode::IntegerLiteralNode(int value)
        : LiteralNode(), value(value) {
    this->literalNodeType = LiteralNodeType::INTEGER_LITERAL_NODE;
}

FloatLiteralNode::FloatLiteralNode(float value)
        : LiteralNode(), value(value) {
    this->literalNodeType = LiteralNodeType::FLOAT_LITERAL_NODE;
}

CharacterLiteralNode::CharacterLiteralNode(char value)
        : LiteralNode(), value(value) {
    this->literalNodeType = LiteralNodeType::CHARACTER_LITERAL_NODE;
}

StringLiteralNode::StringLiteralNode(phlib::String &value)
        : LiteralNode(), value(value) {
    this->literalNodeType = LiteralNodeType::STRING_LITERAL_NODE;
}


BooleanLiteralNode::BooleanLiteralNode(PshBoolean &value)
        : LiteralNode(), value(value) {
    this->literalNodeType = LiteralNodeType::BOOLEAN_LITERAL_NODE;
}

IntegerLiteralNode::IntegerLiteralNode(PshInteger &value)
        : LiteralNode(), value(value) {
    this->literalNodeType = LiteralNodeType::INTEGER_LITERAL_NODE;
}

FloatLiteralNode::FloatLiteralNode(PshFloat &value)
        : LiteralNode(), value(value) {
    this->literalNodeType = LiteralNodeType::FLOAT_LITERAL_NODE;
}

CharacterLiteralNode::CharacterLiteralNode(PshCharacter &value)
        : LiteralNode(), value(value) {
    this->literalNodeType = LiteralNodeType::CHARACTER_LITERAL_NODE;
}

StringLiteralNode::StringLiteralNode(PshString &value)
        : LiteralNode(), value(value) {
    this->literalNodeType = LiteralNodeType::STRING_LITERAL_NODE;
}

//Statements

StatementNode::StatementNode() : BaseTreeNode() {
    this->nodeType          = TreeNodeType::STATEMENT_NODE;
    this->statementNodeType = (StatementNodeType) 0;
}

DefVarStatementNode::DefVarStatementNode(IdentifierExpressionNode *identifier, ExpressionNode *value)
        : StatementNode() {
    this->statementNodeType = StatementNodeType::DEF_VAR_STATEMENT_NODE;
    this->identifier        = identifier;
    this->value             = value;
}

ExpressionStatementNode::ExpressionStatementNode(ExpressionNode *expression)
        : StatementNode() {
    this->statementNodeType = StatementNodeType::EXPRESSION_STATEMENT_NODE;
    this->expression        = expression;
}

IfStatementNode::IfStatementNode(ExpressionNode *expression, StatementNode *trueStatement,
                                 StatementNode *falseStatement)
        : StatementNode() {
    this->statementNodeType = StatementNodeType::IF_SELECTION_NODE;
    this->expression        = expression;
    this->trueStatement     = trueStatement;
    this->falseStatement    = falseStatement;
}

IfStatementNode::IfStatementNode(ExpressionNode *expression, StatementNode *trueStatement)
        : IfStatementNode(expression, trueStatement, nullptr) {
}

CaseSelectionNode::CaseSelectionNode(ConstantExpressionNode *constantExpression, StatementNode *statement,
                                     CaseSelectionNode *nextCase)
        : StatementNode() {
    this->statementNodeType  = StatementNodeType::CASE_SELECTION_NODE;
    this->constantExpression = constantExpression;
    this->statement          = statement;
    this->nextCase           = nextCase;
}

SwitchStatementNode::SwitchStatementNode(ExpressionNode *expression, CaseSelectionNode *caseStatement,
                                         StatementNode *otherStatement)
        : StatementNode() {
    this->statementNodeType = StatementNodeType::SWITCH_SELECTION_NODE;
    this->expression        = expression;
    this->caseStatement     = caseStatement;
    this->otherStatement    = otherStatement;
}

WhileStatementNode::WhileStatementNode(ExpressionNode *expression, StatementNode *bodyStatement)
        : StatementNode() {
    this->statementNodeType = StatementNodeType::WHILE_ITERATION_NODE;
    this->expression        = expression;
    this->bodyStatement     = bodyStatement;
}

DoWhileStatementNode::DoWhileStatementNode(ExpressionNode *expression, StatementNode *bodyStatement)
        : StatementNode() {
    this->statementNodeType = StatementNodeType::DO_WHILE_ITERATION_NODE;
    this->expression        = expression;
    this->bodyStatement     = bodyStatement;
}

ForStatementNode::ForStatementNode(IdentifierExpressionNode *element, IdentifierExpressionNode *list,
                                   StatementNode *bodyStatement)
        : StatementNode() {
    this->statementNodeType = StatementNodeType::FOR_ITERATION_NODE;
    this->element           = element;
    this->list              = list;
    this->bodyStatement     = bodyStatement;
}
