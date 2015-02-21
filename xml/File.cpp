#include "File.h"
using namespace Kahnadex::Files;

/**
\brief  see if a file exists
\param  apsz_FilePath filename
\return 1 if file exists, 0 if not
*/
int File::Exists(const char* apsz_FilePath) {
	FILE* lp_File = 0;
	int   li_Return = 0;
	lp_File = fopen(apsz_FilePath, "rb");
	if (lp_File) {
		li_Return = 1;
		fclose(lp_File);
	}
	return(li_Return);
}

/**
\brief  get the size of a file in bytes
\param  apsz_FilePath filename
\return size of the file in bytes (0 if no file)
*/
int File::GetSize(const char* apsz_FilePath) {
	FILE* lp_File = 0;
	int   li_Size = 0;
	lp_File = fopen(apsz_FilePath, "rb");
	if (lp_File) {
		fseek(lp_File, 0, SEEK_END);
		li_Size = ftell(lp_File);
		fclose(lp_File);
	}
	return(li_Size);
}

/**
\brief  close an open MemoryFileStruct handle
\param  ap_File MemoryFileStruct handle
\return VOID
*/
void File::MemClose(MemoryFileStruct* ap_File) {
	if (ap_File)free(ap_File);
	return;
}

/**
\brief  check if memory file is at end of file
\param  ap_File MemoryFileStruct handle
\return 1 if at EOF, 0 if not
*/
char File::MemEOF(MemoryFileStruct* ap_File) {
	return(ap_File->Offset >= (ap_File->FileLength - 1));
}

/**
\brief  read one character from an open memory file
\param  ap_File MemoryFileStruct handle
\return character read
*/
char File::MemGetC(MemoryFileStruct* ap_File) {
	char lc_Char;
	File::MemRead(&lc_Char, 1, ap_File);
	return(lc_Char);
}

/**
\brief  read specified number of characters from an open memory file (or until newline)
\param  apsz_Buffer buffer to store string in
\param  ai_MaxBytesToRead maximum bytes to read
\param  ap_File MemoryFileStruct handle
\return VOID
*/
void File::MemGetS(char* apsz_Buffer, int ai_MaxBytesToRead, MemoryFileStruct* ap_File) {
	char lsz_Char[2] = { 0 };
	int  i = 0;

	apsz_Buffer[0] = 0;

	while (i<ai_MaxBytesToRead) {
		if (File::MemEOF(ap_File))break;
		//read one character
		lsz_Char[0] = File::MemGetC(ap_File);
		//end of line, exit
		if (lsz_Char[0] == 13)continue;
		if (lsz_Char[0] == 10)break;
		//add letter
		strcat(apsz_Buffer, lsz_Char);
	}
	return;
}

/**
\brief  open a memory buffer for reading
\param  ap_FileData memory buffer to read (pointer to the first BYTE)
\param  ai_Length length of memory buffer
\return MemoryFileStruct handle
*/
MemoryFileStruct* File::MemOpen(BYTE* ap_FileData, int ai_Length) {
	MemoryFileStruct* lp_File = (MemoryFileStruct*)malloc(sizeof(MemoryFileStruct));

	memset(lp_File, 0, sizeof(MemoryFileStruct));

	lp_File->FileData = ap_FileData;
	lp_File->FileLength = ai_Length;

	return(lp_File);
}

/**
\brief  read data from an open MemoryFileStruct handle
\param  ap_Buffer memory buffer to store what is read
\param  ai_BytesToRead the number of bytes to read into ap_Buffer
\param  ap_File the MemoryFileStruct handle to read from
\return number of bytes read by this function
*/
int File::MemRead(void* ap_Buffer, size_t ai_BytesToRead, MemoryFileStruct* ap_File) {

	if ((int)(ap_File->Offset + ai_BytesToRead)<ap_File->FileLength) {
		//read full amount
		memcpy(ap_Buffer, &ap_File->FileData[ap_File->Offset], ai_BytesToRead);
		ap_File->Offset += (int)ai_BytesToRead;
		return((int)ai_BytesToRead);
	} else {
		//number of bytes to read exceeds file length.  read to end
		if (ap_File->Offset<ap_File->FileLength) {
			int li_Length = (ap_File->FileLength - ap_File->Offset);
			memcpy(ap_Buffer, &ap_File->FileData[ap_File->Offset], li_Length);
			return(li_Length);
		}
	}
	return(0);
}

/**
\brief  read in an entire file as binary.
(dynamically allocates memory)
\param  apsz_FilePath filename
\param  ap_Size pointer to size(how many bytes were allocated by this function)
\return file contents as a BYTE array
*/
BYTE* File::ReadAllAsBinary(const char* apsz_FilePath, int* ap_Size) {
	FILE* lp_File = 0;
	BYTE* lp_Data = 0;
	int   li_Size = 0;
	//open
	if (ap_Size)*ap_Size = 0;
	if (!apsz_FilePath)goto Exit;
	lp_File = fopen(apsz_FilePath, "rb");
	if (!lp_File)goto Exit;
	//allocate
	fseek(lp_File, 0, SEEK_END);
	li_Size = ftell(lp_File);
	lp_Data = (BYTE*)malloc(li_Size);
	if (ap_Size)*ap_Size = li_Size;
	//read
	fseek(lp_File, 0, SEEK_SET);
	fread(lp_Data, li_Size, 1, lp_File);
	fclose(lp_File);
Exit:
	return(lp_Data);
}

/**
\brief  read in an entire file as text.
(dynamically allocates memory)
\param  apsz_FilePath filename
\return file contents as a character array (null terminated)
*/
char* File::ReadAllAsText(const char* apsz_FilePath) {
	FILE* lp_File = 0;
	int   li_Size = 0;
	char* lpsz_File = 0;
	//open
	if (!apsz_FilePath)goto Exit;
	lp_File = fopen(apsz_FilePath, "r");
	if (!lp_File)goto Exit;
	//allocate
	fseek(lp_File, 0, SEEK_END);
	li_Size = ftell(lp_File);
	lpsz_File = (char*)malloc(li_Size + 1);
	memset(lpsz_File, 0, (li_Size + 1));
	//read
	fseek(lp_File, 0, SEEK_SET);
	fread(lpsz_File, li_Size, 1, lp_File);
	fclose(lp_File);
Exit:
	return(lpsz_File);
}

/**
\brief  read data as a binary file
(dynamically allocates memory)
\param  apsz_FilePath filename to use
\param  ap_Data data to save
\param  ai_Size size of ap_Data in bytes
\return 1 if success, 0 if failure
*/
int File::SaveDataAsBinary(const char* apsz_FilePath, BYTE* ap_Data, int ai_Size) {
	FILE* lp_File;
	int li_Return = 0;
	if ((!apsz_FilePath) || (!ap_Data))goto Exit;
	//open
	lp_File = fopen(apsz_FilePath, "wb");
	if (!lp_File)goto Exit;
	//write,close
	fwrite(ap_Data, ai_Size, 1, lp_File);
	fclose(lp_File);
	li_Return = 1;
Exit:
	return(li_Return);
}