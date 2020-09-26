#include <il/ilut.h>
#include <il/il.h>
#include <il/ilu.h>

#include <string>
#include <vector>

using namespace std;

/*
Supports loading of: 
.cut 
.dcx 
.dds 
.ico 
.gif 
.jpg 
.lbm 
.lif 
.mdl 
.pcd 
.pcx 
.pic 
.png 
.pnm 
.psd 
.psp 
.raw 
.sgi 
.tga 
.tif 
.wal 
.act 
.pal 
.hdr 
Doom graphics 

Supports saving of: 
.bmp 
.dds 
.jpg 
.pcx 
.png 
.pnm 
.raw 
.sgi 
.tga 
.tif 
.pal 
.hdr 
*/


class TextureConverter
{
	protected:

		vector< string > m_TextureFilePaths;

	public:

		TextureConverter() {}
		~TextureConverter() {}

		void Push_Back( string _TextureFilePath );
		void ConvertToTGA( string _OutputTextrueFilePath, string _ToExt, string _ToFormat);
		void Clear();

	protected:
};