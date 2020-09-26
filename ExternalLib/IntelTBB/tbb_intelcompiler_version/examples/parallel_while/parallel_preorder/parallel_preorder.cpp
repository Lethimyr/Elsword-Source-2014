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

/* Example program that shows how to use parallel_while to do parallel preorder 
   traversal of a directed acyclic graph. */

#include "tbb/parallel_while.h"
#include "tbb/atomic.h"
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdio>
#include "Graph.h"

using namespace std;

//! Number of trials. Can be changed from command line
int ntrial = 50;

class Body {
    tbb::parallel_while<Body>& my_while; 
public:
    Body( tbb::parallel_while<Body>& w ) : my_while(w) {};

    //------------------------------------------------------------------------
    // Following signatures required by parallel_while
    //------------------------------------------------------------------------
    typedef Cell* argument_type;
    void operator()( Cell* c ) const {
        c->update();
        // Restore ref_count in preparation for subsequent traversal.
        c->ref_count = ArityOfOp[c->op];
        for( size_t k=0; k<c->successor.size(); ++k ) {
            Cell* successor = c->successor[k];
            if( 0 == --(successor->ref_count) ) {
                my_while.add( successor );
            }
        }
    }
};   

class Stream {
    size_t k;
    const vector<Cell*>& my_roots;
public:
    Stream( const vector<Cell*>& root_set ) : my_roots(root_set), k(0) {}
    bool pop_if_present( Cell*& item ) {
        bool result = k<my_roots.size();
        if( result ) 
            item = my_roots[k++];
        return result;
    }
};    

void ParallelPreorderTraversal( const vector<Cell*>& root_set ) {
    tbb::parallel_while<Body> w;
    Stream s(root_set);
    w.run(s,Body(w));
}

//------------------------------------------------------------------------
// Test driver
//------------------------------------------------------------------------

#include <cctype>
#include "tbb/task_scheduler_init.h"
#include "tbb/tick_count.h"

//! A closed range of int.
struct IntRange {
    int low;
    int high;
    void set_from_string( const char* s );
    IntRange( int low_, int high_ ) : low(low_), high(high_) {}
};

void IntRange::set_from_string( const char* s ) {
    char* end;
    high = low = strtol(s,&end,0);
    switch( *end ) {
    case ':': 
        high = strtol(end+1,0,0); 
        break;
    case '\0':
        break;
    default:
        printf("unexpected character = %c\n",*end);
    }
}

//! Number of threads to use.
static IntRange NThread(1,4);

//! If true, then at end wait for user to hit return
static bool PauseFlag = false;

//! Displays usage message
void Usage(char * argv0) {
    fprintf(stderr, "Usage: %s [nthread [ntrials ['pause']]]\n", argv0);
    fprintf(stderr, "where nthread is a non-negative integer, or range of the form low:high [%d:%d]\n", NThread.low, NThread.high);
    fprintf(stderr, "ntrials is a positive integer. Default value is 50, reduce it (e.g. to 5) to shorten example run time\n");
    fprintf(stderr, "The application waits for user to hit return if 'pause' is specified\n");
}

//! Parse the command line.
static void ParseCommandLine( int argc, char* argv[] ) {
    int i = 1;
        if( i<argc && !isdigit(argv[i][0]) ) { 
        // Command line is garbled.
        Usage(argv[0]);
        exit(1);
    }
    if( i<argc )
        NThread.set_from_string(argv[i++]);
    if( i<argc && !isdigit(argv[i][0]) ) { 
        // Command line is garbled.
        Usage(argv[0]);
        exit(1);
    }
    if (i<argc) {
        ntrial = strtol(argv[i++], 0, 0);
    }
    if (ntrial == 0) {
        // Command line is garbled.
        Usage(argv[0]);
        exit(1);
    }
    if (i<argc && strcmp( argv[i], "pause" )==0 ) {
        PauseFlag = true;
    }
}

int main( int argc, char* argv[] ) {
    ParseCommandLine(argc,argv);

    // Start scheduler with given number of threads.
    for( int p=NThread.low; p<=NThread.high; ++p ) {
        tbb::task_scheduler_init init(p);
        srand(2);
        tbb::tick_count::interval_t interval;
        size_t total_root_set_size = 0;
        for( int trial=0; trial<ntrial; ++trial ) {
            Graph g;
            g.create_random_dag(1000);
            vector<Cell*> root_set;
            g.get_root_set(root_set);
            total_root_set_size += root_set.size();

            tbb::tick_count t0 = tbb::tick_count::now();
            for( int i=0; i<10; ++i ) {
                ParallelPreorderTraversal(root_set);
            }
            tbb::tick_count t1 = tbb::tick_count::now();

            interval += t1-t0;
        }
        printf("%g seconds using %d threads (average of %g nodes in root_set)\n",interval.seconds(),p,(double)total_root_set_size/ntrial);
    }

    if (PauseFlag) {
        printf ("Press return key to exit");
        char c;
        int n = scanf("%c", &c);
        if( n!=1 ) {
            fprintf(stderr,"Fatal error: unexpected end of input\n");
            exit(1);
        }
    }

    return 0;
}
