#include "StdAfx.h"
#include ".\X2NumbersToLanguage.h"

/** @function	: CNumbersToLanguage
	@brief		: »ý¼ºÀÚ
    @author		: JHKang
	@date		: 2010/08/31
*/
CX2NumbersToLanguage::CX2NumbersToLanguage(void):
m_LanguageData(),
m_eType(TL_KOR)
{
}

/** @function	: ~CNumbersToLanguage
	@brief		: ¼Ò¸êÀÚ
    @author		: JHKang
	@date		: 2010/08/31
*/
CX2NumbersToLanguage::~CX2NumbersToLanguage(void)
{
}

/** @function	: TranslateDigits
	@brief		: ¼ýÀÚ¸¦ ¾ð¾î·Î(IN unsigned long)
    @author		: JHKang
	@date		: 2010/08/31
*/
#ifdef _LANGUAGE_FIX_CHINESE_
void CX2NumbersToLanguage::TranslateDigits(TypeLanguage eType, __int64 i64lNumbers, wstring& strMsg)
#else _LANGUAGE_FIX_CHINESE_
void CX2NumbersToLanguage::TranslateDigits(TypeLanguage eType, __int64 i64lNumbers, string& strMsg)
#endif _LANGUAGE_FIX_CHINESE_
{
	string InData = ToString(i64lNumbers);
	int length = static_cast<int>(InData.length());
	int iCount = 0;

#ifdef _LANGUAGE_FIX_CHINESE_
	wstring (*Lang)[10];
	Lang = m_LanguageData.LD_CHN_SIMPLIFIED;
#else _LANGUAGE_FIX_CHINESE_
	string (*Lang)[10];

	switch (eType)
	{
	case TL_KOR:
		Lang = m_LanguageData.LD_KOR;
		break;
	case TL_CHN:
		Lang = m_LanguageData.LD_CHN;
		break;
#ifdef NUMBER_TO_LANGUAGE_JPN
	case TL_JPN:
		Lang = m_LanguageData.LD_JPN;
		break;
#endif // NUMBER_TO_LANGUAGE_JPN
	}
#endif _LANGUAGE_FIX_CHINESE_

	while (length != 0)
	{
		/// 4ÀÚ¸´¼ö·Î ²÷±â
		int num1 = length / 4;
		int modNum = length % 4;

		if (num1 == 0 && modNum == 0)
		{
#ifdef _LANGUAGE_FIX_CHINESE_
			strMsg += L"ÖÃ";
#else
			strMsg += "¿µ";
#endif _LANGUAGE_FIX_CHINESE_
			break;
		}

		if (modNum < 1)
			num1--;

		/// 4ÀÚ¸´¼ö·Î ²÷Àº ·çÇÁ
		for (int i = num1; i >= 0; i--)
		{
			int num2 = length % 4;
			int temp = num2;
			int digitTotal = 0;

			if (num2 == 0)
				num2 = temp = 4;

			for (int j = 0; j < num2; j++)
			{
				int digit = ToNumbers(InData[iCount]);
				digitTotal += digit;

				temp--;

				if (digit != 0)
					strMsg += Lang[0][digit] + Lang[1][temp];
				else
#ifdef _LANGUAGE_FIX_CHINESE_
					strMsg += L"";
#else
					strMsg += "";
#endif _LANGUAGE_FIX_CHINESE_

				length--;
				iCount++;
			}
			if (digitTotal > 0)
				strMsg += Lang[2][i];
		}
	}
}