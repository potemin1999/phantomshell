//
// Created by ilya on 12/16/18.
//

#ifndef PHANTOMSHELL_TREE_COMMON_H
#define PHANTOMSHELL_TREE_COMMON_H

#include "types.h"

namespace psh {

typedef enum TreeNodeType {
    DECLARATION_NODE = 0x01,
    EXPRESSION_NODE  = 0x02,
    LITERAL_NODE     = 0x03,
    STATEMENT_NODE   = 0x04,
} TreeNodeType;


class BaseTreeNode {
public:
    TreeNodeType node_type;
};

}
#endif //PHANTOMSHELL_TREE_COMMON_H
