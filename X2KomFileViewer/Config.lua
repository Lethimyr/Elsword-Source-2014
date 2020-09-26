

--[[ KomFileViewer 툴 옵션 ]]

CX2KomFileManager:SetThreadCount( 3 )

CX2KomFileManager:InitKomFileViewerOption
{
	m_iKFS_MIN_SIZE = 27,
	m_iKFS_MAX_SIZE = 33,
}

CX2KomFileManager:AddEncryptionKey
{
	m_iEncryptionKey = 3338185218,
}

CX2KomFileManager:AddEncryptionKey
{
	m_iEncryptionKey = 642231260,
}

CX2KomFileManager:AddEncryptionKey
{
	m_iEncryptionKey = 2550184943,
}

CX2KomFileManager:SetPathsForCommand
{
	m_wstrDepotPath			= { "D:\\ProjectX2_svn\\resource\\dat", },
	
	m_wstrLuacCommand		= {"D:\\projectx2\\Bin\\루아컴파일툴\\luac.exe", },
	
	m_wstrXConvertCommand	= {"D:\\projectx2\\bin\\XMeshConvert\\kogskinmesh.exe", },
}