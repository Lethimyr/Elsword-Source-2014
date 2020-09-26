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

class Matrix {
    static const int n = 10;
    float array[n][n];
public:
    Matrix() {}
    Matrix( float z ) {
        for( int i=0; i<n; ++i )
            for( int j=0; j<n; ++j )
                array[i][j] = i==j ? z : 0;
    }
    friend Matrix operator-( const Matrix& x ) {
        Matrix result;
        for( int i=0; i<n; ++i )
            for( int j=0; j<n; ++j )
                result.array[i][j] = -x.array[i][j];
        return result;
    }
    friend Matrix operator+( const Matrix& x, const Matrix& y ) {
        Matrix result;
        for( int i=0; i<n; ++i )
            for( int j=0; j<n; ++j )
                result.array[i][j] = x.array[i][j] + y.array[i][j];
        return result;
    }
    friend Matrix operator-( const Matrix& x, const Matrix& y ) {
        Matrix result;
        for( int i=0; i<n; ++i )
            for( int j=0; j<n; ++j )
                result.array[i][j] = x.array[i][j] - y.array[i][j];
        return result;
    }
    friend Matrix operator*( const Matrix& x, const Matrix& y ) {
        Matrix result(0);
        for( int i=0; i<n; ++i ) 
            for( int k=0; k<n; ++k )
                for( int j=0; j<n; ++j )
                    result.array[i][j] += x.array[i][k] * y.array[k][j];
        return result;
    }
};
