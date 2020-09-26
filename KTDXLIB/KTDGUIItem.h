#pragma once

class CKTDGUIItem : public CKTDGUIStatic
{

	public:
		CKTDGUIItem(void);
		virtual ~CKTDGUIItem(void);

		int GetWidth() { return m_Width; }
		int GetHeight() { return m_Height; }

		void SetWidth( int width ) { m_Width = width; }
		void SetHeight( int height ) { m_Height = height; }
 
	protected:

		int m_Width;
		int m_Height;
};
