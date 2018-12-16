//
// Created by ilya on 12/16/18.
//

#ifndef PHANTOMSHELL_TREE_EXPRESSION_H
#define PHANTOMSHELL_TREE_EXPRESSION_H

#include "operator.h"
#include "tree_common.h"
#include "tree_literal.h"

namespace psh {

enum ExpressionNodeType {
    CONSTANT_EXPRESSION_NODE   = 0x01,
    UNARY_EXPRESSION_NODE      = 0x02,
    BINARY_EXPRESSION_NODE     = 0x03,
    TERNARY_EXPRESSION_NODE    = 0x04,
    IDENTIFIER_EXPRESSION_NODE = 0x05
};

class ExpressionNode : BaseTreeNode {
public:
    ExpressionNodeType expression_node_type;

    ExpressionNode() : BaseTreeNode() {
        node_type            = TreeNodeType::EXPRESSION_NODE;
        expression_node_type = (ExpressionNodeType) 0;
    }
};

class ConstantExpressionNode : ExpressionNode {
public:
    LiteralNode *literal;

    explicit ConstantExpressionNode(LiteralNode *literal)
            : ExpressionNode() {
        this->expression_node_type = ExpressionNodeType::CONSTANT_EXPRESSION_NODE;
        this->literal              = literal;
    }
};

class UnaryExpressionNode : ExpressionNode {
public:
    Operator       oper;
    ExpressionNode *operand;

    UnaryExpressionNode(Operator oper,
                        ExpressionNode *operand)
            : ExpressionNode() {
        this->expression_node_type = ExpressionNodeType::UNARY_EXPRESSION_NODE;
        this->oper                 = oper;
        this->operand              = operand;
    }
};

class BinaryExpressionNode : ExpressionNode {
public:
    Operator       oper;
    ExpressionNode *operand1;
    ExpressionNode *operand2;

    BinaryExpressionNode(Operator oper,
                         ExpressionNode *operand1,
                         ExpressionNode *operand2)
            : ExpressionNode() {
        this->expression_node_type = ExpressionNodeType::BINARY_EXPRESSION_NODE;
        this->oper                 = oper;
        this->operand1             = operand1;
        this->operand2             = operand2;
    }
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
                          ExpressionNode *operand3)
            : ExpressionNode() {
        this->expression_node_type = ExpressionNodeType::TERNARY_EXPRESSION_NODE;
        this->oper                 = oper;
        this->operand1             = operand1;
        this->operand2             = operand2;
        this->operand3             = operand3;
    }
};

class IdentifierExpressionNode : ExpressionNode {
public:
    IdentifierExpressionNode()
            : ExpressionNode() {
        expression_node_type = ExpressionNodeType::IDENTIFIER_EXPRESSION_NODE;
    }
};

}
#endif //PHANTOMSHELL_TREE_EXPRESSION_H
