/*File.h                                  */
/*------------------------------------------
By Brian Clifton (bclifton@gmail.com)

Author Notes:
This code is free for use in any projects.
The code is provided "as is" without any
expressed or implied warranty

Description:
Code for working with files

Version History:
2005.12.10 - 2.0 - Kahnadex::Files
2004.09.12 - 1.6 - added string.h include
cleaned up code
2004.09.01 - 1.5 - updated email address
2004.08.22 - 1.4 - added MemGetC(),MemGetS(), and MemEOF()
2004.08.15 - 1.3 - added MemClose(),MemOpen(),and MemRead()
2004.01.06 - 1.2 - added ReadAllAsText()
2003.12.26 - 1.1 - files now use C style comments
files are also formatted for doxygen
2003.11.09 - 1.0 - initial release
------------------------------------------*/
#ifndef KAHNADEX_FILES_FILE_H
#define KAHNADEX_FILES_FILE_H

//INCLUDES

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
namespace Kahnadex {
	namespace Files {

		//DATATYPES

		typedef unsigned char BYTE;

		typedef struct {
			int   Offset;
			int   FileLength;
			BYTE* FileData;
		} MemoryFileStruct;

		//FUNCTIONS

		class File {
		public:
			static int   Exists(const char* apsz_FilePath);
			static int   GetSize(const char* apsz_FilePath);
			static void  MemClose(MemoryFileStruct* ap_File);
			static char  MemEOF(MemoryFileStruct* ap_File);
			static char  MemGetC(MemoryFileStruct* ap_File);
			static void  MemGetS(char* apsz_Buffer, int ai_MaxBytesToRead, MemoryFileStruct* ap_File);
			static MemoryFileStruct* MemOpen(BYTE* ap_FileData, int ai_Length);
			static int   MemRead(void* ap_Buffer, size_t ai_BytesToRead, MemoryFileStruct* ap_File);
			static BYTE* ReadAllAsBinary(const char* apsz_FilePath, int* ap_Size);
			static char* ReadAllAsText(const char* apsz_FilePath);
			static int   SaveDataAsBinary(const char* apsz_FilePath, BYTE* ap_Data, int ai_Size);
		};

	}
}
#endif