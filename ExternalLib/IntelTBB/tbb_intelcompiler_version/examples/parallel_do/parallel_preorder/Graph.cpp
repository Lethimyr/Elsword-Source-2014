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

#include <cstdio>
#include <cstdlib>
#include "Graph.h"

using namespace std;

void Graph::create_random_dag( size_t number_of_nodes ) {
    my_vertex_set.resize(number_of_nodes);
    for( size_t k=0; k<number_of_nodes; ++k ) {
        Cell& c = my_vertex_set[k];
        int op = int((rand()>>8)%5u);
        if( op>int(k) ) op = int(k);
        switch( op ) {
            default:
                c.op = OP_VALUE;
                c.value = Cell::value_type((float)k);
                break;
            case 1:
                c.op = OP_NEGATE;
                break;
            case 2:
                c.op = OP_SUB;
                break;
            case 3: 
                c.op = OP_ADD;
                break;
            case 4: 
                c.op = OP_MUL;
                break;
        }
        for( int j=0; j<ArityOfOp[c.op]; ++j ) {
            Cell& input = my_vertex_set[rand()%k];
            c.input[j] = &input;
        }
    }
}

void Graph::print() {
    for( size_t k=0; k<my_vertex_set.size(); ++k ) {
        printf("Cell %d:",int(k));
        for( size_t j=0; j<my_vertex_set[k].successor.size(); ++j )
            printf(" %d",int(my_vertex_set[k].successor[j] - &my_vertex_set[0]));
        printf("\n");
    }
}

void Graph::get_root_set( vector<Cell*>& root_set ) {
    for( size_t k=0; k<my_vertex_set.size(); ++k ) {
        my_vertex_set[k].successor.clear();
    }
    root_set.clear();
    for( size_t k=0; k<my_vertex_set.size(); ++k ) {
        Cell& c = my_vertex_set[k];
        c.ref_count = ArityOfOp[c.op];
        for( int j=0; j<ArityOfOp[c.op]; ++j ) {
            c.input[j]->successor.push_back(&c);
        }
        if( ArityOfOp[c.op]==0 )
            root_set.push_back(&my_vertex_set[k]);
    }
}

void Cell::update() {
    switch( op ) {
        case OP_VALUE:
            break;
        case OP_NEGATE:
            value = -(input[0]->value);
            break;
        case OP_ADD:
            value = input[0]->value + input[1]->value;
            break;
        case OP_SUB:
            value = input[0]->value - input[1]->value;
            break;
        case OP_MUL:
            value = input[0]->value * input[1]->value;
            break;
    }
}

