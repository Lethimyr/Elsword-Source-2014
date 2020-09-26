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

#ifndef __BOARD_H__ 
#define __BOARD_H__

#define WIN32_LEAN_AND_MEAN

#ifndef _CONSOLE
#include <windows.h>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
#define LabelPtr Label^
#define BoardPtr Board^
#else
#define LabelPtr int*
#define BoardPtr Board*
#endif

struct Matrix 
{
    int width;
    int height;
    char* data;
};

#ifndef _CONSOLE
public ref class Board : public System::Windows::Forms::UserControl
#else
class Board
#endif
    {
    public:
        Board(int width, int height, int squareSize, LabelPtr counter);        
        virtual ~Board();
        void seed(int s);
        void seed(const BoardPtr s);
#ifndef _CONSOLE
    protected: 
        virtual void OnPaint(PaintEventArgs^ e) override;        
        void Board::draw(Graphics^ g);

    private:
        System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
        }
#pragma endregion

    private: delegate void drawDelegate(Int32);
    public:
        //! Called from the Evolution thread
        void draw( Int32 nCurIteration )
        {
            if (this->InvokeRequired)
            {
                drawDelegate^ d = gcnew drawDelegate(this, &Board::draw);
                IAsyncResult^ result = BeginInvoke(d, nCurIteration);
                EndInvoke(result);
                return;
            }
            m_counter->Text = nCurIteration.ToString();
            Invalidate();
        }
#endif
    public:
        Matrix *m_matrix;    

    private:
#ifndef _CONSOLE
        SolidBrush^ m_occupiedBrush;
        SolidBrush^ m_freeBrush;
        Graphics^ m_graphics;
        Graphics^ m_mem_dc;
        Bitmap^ m_bmp;
#endif
        int m_width;
        int m_height;
        int m_squareSize;
        LabelPtr m_counter;
    };
#endif
