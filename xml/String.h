/*String.h                                */
/*------------------------------------------
By Brian Clifton (bclifton@gmail.com)

Author Notes:
This code is free for use in any projects.
The code is provided "as is" without any 
expressed or implied warranty

Description:
String helper functions

Version History:
2006.03.04 - 2.3 - trim now removes CR and LF
2006.02.26 - 2.2 - added IndexOf
2005.12.11 - 2.1 - SplitString struct now defined C++ style
                   got rid of typedef for byte and word
2005.12.10 - 2.0 - Kahnadex
2004.09.12 - 1.8 - cleaned up/optimized code
2004.06.20 - 1.7 - updated email address
2004.02.27 - 1.6 - added GetExtensionFromFileName
2003.12.26 - 1.5 - files now use C style comments
				   files are also formatted for doxygen
2003.11.11 - 1.4 - added ChopFront,RemovePathFromFileName,TrimFrontUntil
2003.11.09 - 1.3 - added ChopEnd,RemoveAllWhiteSpace,ReplaceAll,TrimEndUntil
2003.11.03 - 1.2 - optimized Split
2003.10.30 - 1.1 - added Trim,Split,FreeSplit
                   beginning work on search/replace
2003.10.28 - 1.0 - initial release
------------------------------------------*/
#ifndef KAHNADEX_STRING_H
#define KAHNADEX_STRING_H
//------------------------------------------
//INCLUDES
//------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
namespace Kahnadex{
//------------------------------------------
//DATATYPES
//------------------------------------------
typedef unsigned short WORD;

struct SplitString
{
	int    Count;
	char** String;
};
//------------------------------------------
//FUNCTIONS
//------------------------------------------
class String
{
	public:
		static char  ChopEnd(char* apsz_String);
		static char  ChopFront(char* apsz_String);
		static void  FreeSplit(SplitString* ap_Split);
		static int   GetExtensionFromFileName(char* apsz_String,char* apsz_Buffer);
		static int   IndexOf(const char* apsz_String,const char* apsz_Keyword);
		static int   IsNumeric(char* apsz_String);
		static void  Left(char* apsz_String,WORD ai_Count,char* apsz_Buffer);
		static void  Mid(char* apsz_String,WORD ai_First,WORD ai_Count,char* apsz_Buffer);
		static void  RemoveAllWhiteSpace(char* apsz_String);
		static void  RemovePathFromFileName(char* apsz_String);
		static char* ReplaceAll(char* apsz_String,char* apsz_Keyword,char* apsz_ReplaceWith,int* ap_Replaced);
		static void  Right(char* apsz_String,WORD ai_Count,char* apsz_Buffer);
		static SplitString* Split(char* apsz_String,char ac_Token);
		static void  ToLower(char* apsz_String);
		static void  ToUpper(char* apsz_String);
		static void  Trim(char* apsz_String);
		static int   TrimEndUntil(char* apsz_String,char ac_Char);
		static int   TrimFrontUntil(char* apsz_String,char ac_Char);
};
//------------------------------------------
}
#endif