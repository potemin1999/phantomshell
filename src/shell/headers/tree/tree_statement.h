//
// Created by ilya on 12/16/18.
//

#ifndef PHANTOMSHELL_TREE_STATEMENT_H
#define PHANTOMSHELL_TREE_STATEMENT_H

#include "tree_common.h"
#include "tree_expression.h"

namespace psh {

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


class StatementNode : BaseTreeNode {
public:
    StatementNodeType statement_node_type;

    StatementNode()
            : BaseTreeNode() {
        node_type = TreeNodeType::STATEMENT_NODE;
        this->statement_node_type = (StatementNodeType) 0;
    }
};

class DefVarStatementNode : StatementNode {
public:
    IdentifierExpressionNode *identifier;
    ExpressionNode           *value;

    DefVarStatementNode(IdentifierExpressionNode *identifier,
                        ExpressionNode *value)
            : StatementNode() {
        this->statement_node_type = StatementNodeType::DEF_VAR_STATEMENT_NODE;
        this->identifier          = identifier;
        this->value               = value;
    }
};

class ExpressionStatementNode : StatementNode {
public:
    ExpressionNode *expression;

    explicit ExpressionStatementNode(ExpressionNode *expression)
            : StatementNode() {
        this->statement_node_type = StatementNodeType::EXPRESSION_STATEMENT_NODE;
        this->expression          = expression;
    }
};

class IfStatementNode : StatementNode {
public:
    ExpressionNode *expression;
    StatementNode  *true_statement;
    StatementNode  *false_statement;

    IfStatementNode(ExpressionNode *expression,
                    StatementNode *true_statement,
                    StatementNode *false_statement)
            : StatementNode() {
        this->statement_node_type = StatementNodeType::IF_SELECTION_NODE;
        this->expression          = expression;
        this->true_statement      = true_statement;
        this->false_statement     = false_statement;
    }

    IfStatementNode(ExpressionNode *expression,
                    StatementNode *true_statement)
            : IfStatementNode(expression,
                              true_statement,
                              nullptr) {
    }
};

class CaseSelectionNode : StatementNode {
public:
    ConstantExpressionNode *constant_expression;
    StatementNode          *statement;
    CaseSelectionNode      *next_case;

    CaseSelectionNode(ConstantExpressionNode *constant_expression,
                      StatementNode *statement,
                      CaseSelectionNode *next_case)
            : StatementNode() {
        this->statement_node_type = StatementNodeType::CASE_SELECTION_NODE;
        this->constant_expression = constant_expression;
        this->statement           = statement;
        this->next_case           = next_case;
    }
};

class SwitchStatementNode : StatementNode {
public:
    ExpressionNode    *expression;
    CaseSelectionNode *case_statement;
    StatementNode     *other_statement;

    SwitchStatementNode(ExpressionNode *expression,
                        CaseSelectionNode *case_statement,
                        StatementNode *other_statement)
            : StatementNode() {
        this->statement_node_type = StatementNodeType::SWITCH_SELECTION_NODE;
        this->expression          = expression;
        this->case_statement      = case_statement;
        this->other_statement     = other_statement;
    }
};

class WhileStatementNode : StatementNode {
public:
    ExpressionNode *expression;
    StatementNode  *body_statement;

    WhileStatementNode(ExpressionNode *expression,
                       StatementNode *body_statement)
            : StatementNode() {
        this->statement_node_type = StatementNodeType::WHILE_ITERATION_NODE;
        this->expression          = expression;
        this->body_statement      = body_statement;
    }
};

class DoWhileStatementNode : StatementNode {
public:
    ExpressionNode *expression;
    StatementNode  *body_statement;

    DoWhileStatementNode(ExpressionNode *expression,
                         StatementNode *body_statement)
            : StatementNode() {
        this->statement_node_type = StatementNodeType::DO_WHILE_ITERATION_NODE;
        this->expression          = expression;
        this->body_statement      = body_statement;
    }
};

class ForStatementNode : StatementNode {
public:
    IdentifierExpressionNode *element;
    IdentifierExpressionNode *list;
    StatementNode            *body_statement;

    ForStatementNode(IdentifierExpressionNode *element,
                     IdentifierExpressionNode *list,
                     StatementNode *body_statement)
            : StatementNode() {
        this->statement_node_type = StatementNodeType::FOR_ITERATION_NODE;
        this->element             = element;
        this->list                = list;
        this->body_statement      = body_statement;
    }
};

}
#endif //PHANTOMSHELL_TREE_STATEMENT_H
