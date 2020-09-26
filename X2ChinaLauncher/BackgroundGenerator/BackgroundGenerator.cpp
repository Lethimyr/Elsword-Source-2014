// BackgroundGenerator.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//png파일 내용을 파일로 저장
	unsigned char* mem;
	long memSize;

	{
		FILE *fp = fopen("../res/LauncherBackground.png", "rb");

		long sPos = ftell(fp);
		fseek(fp, 0, SEEK_END);
		long ePos = ftell(fp);

		memSize = ePos-sPos;
		mem = (unsigned char*)malloc(memSize*sizeof(unsigned char));

		fseek(fp, 0, SEEK_SET);

		fread(mem, sizeof(unsigned char), memSize, fp);
		//		for(int i=0; i<memSize; i++)
		//			fread(&mem[i], sizeof(unsigned char), 1, fp);

		fclose(fp);
	}

	{
		FILE *fp = fopen("..\\LauncherBackgroundData.h", "w+");

		fprintf(fp, "int g_iLauncherBackgroundDataSize=%d;\n", memSize);
		fprintf(fp, "unsigned char g_ucLauncherBackgroundData[%d]={", memSize);

		for(int i=0; i<memSize; i++)
		{
			//fprintf(fp, "\"");
			//fwrite(&mem[i], 1, sizeof(unsigned char), fp);
			unsigned char temp = mem[i];
			fprintf(fp, "0x%x", temp);
			//fprintf(fp, "\"");
			fprintf(fp, ", ");

			if(i%10 == 0)
				fprintf(fp, "\n");
		}

		fprintf(fp, "};");

		fclose(fp);
	}

	free(mem);


	return 0;
}

