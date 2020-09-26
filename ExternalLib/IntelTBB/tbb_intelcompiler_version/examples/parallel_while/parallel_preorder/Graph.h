/*
    Copyright 2005-2010 Intel Corporation.  All Rights Reserved.

    The source code contained or described herein and all documents related
    to the source code ("Material") are owned by Intel Corporation or its
    suppliers or licensors.  Title to the Material remains with Intel
    Corporation or its suppliers and licensors.  The Material is protected
    by worldwide copyright laws and treaty provisions.  No part of the
    Material may be used, copied, reproduced, modified, published, uploaded,
    posted, transmitted, distributed, or disclosed in any way without
    Intel's prior express written permission.

    No license under any patent, copyright, trade secret or other
    intellectual property right is granted to or conferred upon you by
    disclosure or delivery of the Materials, either expressly, by
    implication, inducement, estoppel or otherwise.  Any license under such
    intellectual property rights must be express and approved by Intel in
    writing.
*/

#include "Matrix.h"
#include "tbb/atomic.h"
#include <vector>

namespace TBB = tbb;

enum OpKind {
    // Use Cell's value
    OP_VALUE,
    // Unary negation
    OP_NEGATE,
    // Addition
    OP_ADD,
    // Subtraction
    OP_SUB,
    // Multiplication
    OP_MUL
};

static const int ArityOfOp[] = {0,1,2,2,2};

class Cell {
public:
    //! Operation for this cell
    OpKind op;

    //! Inputs to this cell
    Cell* input[2];
   
    //! Type of value stored in a Cell
    typedef Matrix value_type;

    //! Value associated with this Cell
    value_type value;

    //! Set of cells that use this Cell as an input
    std::vector<Cell*> successor;

    //! Reference count of number of inputs that are not yet updated.
    TBB::atomic<int> ref_count;

    //! Update the Cell's value.
    void update();

    //! Default construtor
    Cell() {}
};

//! A directed graph where the vertices are Cells.
class Graph {
    std::vector<Cell> my_vertex_set;
public:
    //! Create a random acyclic directed graph
    void create_random_dag( size_t number_of_nodes );

    //! Print the graph
    void print();

    //! Get set of cells that have no inputs.
    void get_root_set( std::vector<Cell*>& root_set );
};

