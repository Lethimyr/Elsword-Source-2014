#include "DevilNet.h"
#include "debug.h"

void TextureConverter::Push_Back( string _TextureFilePath )
{
	m_TextureFilePaths.push_back( _TextureFilePath );
}

void TextureConverter::ConvertToTGA( string _OutputTextrueFilePath, string _ToExt )
{
	ILuint	ImgId;
	ILenum	Error;

	CHAR drive[_MAX_DRIVE];
	CHAR dir[_MAX_DIR];
	CHAR fname[_MAX_FNAME];
	CHAR ext[_MAX_EXT];
	
	char temp[MAX_PATH];

	// Initialize DevIL.
	ilInit();

	// Override the reading functions.
	//ilSetRead(iOpenRead, iCloseRead, iEof, iGetc, iRead, iReadSeek, iReadTell);
	//ilSetWrite(iOpenWrite, iCloseWrite, iPutc, iWriteSeek, iWriteTell, iWrite);

	string _OutputTextureFileFullPath;

	for ( int i = 0; i < (int)m_TextureFilePaths.size(); i++ )
	{
		// Generate the main image name to use.
		ilGenImages(1, &ImgId);
		// Bind this image name.
		ilBindImage(ImgId);

		_OutputTextureFileFullPath = _OutputTextrueFilePath;

		strcpy(temp,  m_TextureFilePaths[i].c_str() );

		debugPrintf( "%s", m_TextureFilePaths[i].c_str() );

		if(!ilLoadImage( temp )) 
		{
			//printf("Could not open file...exiting.\n");
			//getchar();
			MessageBox(NULL, "파일 경로가 잘못되었습니다.", "텍스쳐 로드 에러", MB_OK);
		}

		ilEnable(IL_FILE_OVERWRITE);

		_splitpath( m_TextureFilePaths[i].c_str(), drive, dir, fname, ext );
		
		 // _OutputTextureFileFullPath += 텍스쳐 이름;

		_OutputTextureFileFullPath += fname;
		_OutputTextureFileFullPath += _ToExt;

		strcpy(temp, _OutputTextureFileFullPath.c_str());

		ilSaveImage( temp );


		// Reset the reading / writing functions when we're done loading specially.
		//  This isn't required here, since we're exiting, but here's how it's done:
		ilResetRead();
		ilResetWrite();

		// We're done with the image, so let's delete it.
		ilDeleteImages(1, &ImgId);

	}




	// Simple Error detection loop that displays the Error to the user in a human-readable form.
	while ((Error = ilGetError())) {
		printf("Error: %s\n", iluErrorString(Error));
	}
}

void TextureConverter::Clear()
{
	m_TextureFilePaths.clear();
}