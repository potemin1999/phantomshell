//
// Created by ilya on 12/16/18.
//

#ifndef PHANTOMSHELL_TREE_LITERAL_H
#define PHANTOMSHELL_TREE_LITERAL_H

#include "tree_common.h"
#include "string.h"

namespace psh {

typedef enum LiteralNodeType {
    BOOLEAN_LITERAL_NODE   = 0x01,
    INTEGER_LITERAL_NODE   = 0x02,
    FLOAT_LITERAL_NODE     = 0x03,
    CHARACTER_LITERAL_NODE = 0x04,
    STRING_LITERAL_NODE    = 0x05,
} LiteralNodeType;

class LiteralNode : BaseTreeNode {
public:
    LiteralNodeType literal_node_type;

    LiteralNode() : BaseTreeNode() {
        this->node_type         = TreeNodeType::LITERAL_NODE;
        this->literal_node_type = (LiteralNodeType) 0;
    }
};

class BooleanLiteralNode : LiteralNode {
public:
    explicit BooleanLiteralNode(bool value)
            : LiteralNode() {
        this->literal_node_type = LiteralNodeType::BOOLEAN_LITERAL_NODE;
    }
};

class IntegerLiteralNode : LiteralNode {
public:
    explicit IntegerLiteralNode(int value)
            : LiteralNode() {
        this->literal_node_type = LiteralNodeType::INTEGER_LITERAL_NODE;
    }
};

class FloatLiteralNode : LiteralNode {
public:
    explicit FloatLiteralNode(float value)
            : LiteralNode() {
        this->literal_node_type = LiteralNodeType::FLOAT_LITERAL_NODE;
    }
};

class CharacterLiteralNode : LiteralNode {
public:
    explicit CharacterLiteralNode(char value)
            : LiteralNode() {
        this->literal_node_type = LiteralNodeType::CHARACTER_LITERAL_NODE;
    }
};

class StringLiteralNode : LiteralNode {
public:
    explicit StringLiteralNode(phlib::String *value)
            : LiteralNode() {
        this->literal_node_type = LiteralNodeType::STRING_LITERAL_NODE;
    }
};

};
#endif //PHANTOMSHELL_TREE_LITERAL_H
