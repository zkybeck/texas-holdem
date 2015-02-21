#include "String.h"
using namespace Kahnadex;

/**
\brief  chops off the last character of a string and returns it
\param  apsz_String string
\return character chopped
*/
char String::ChopEnd(char* apsz_String) {
	char lc_Return;

	if (!apsz_String) return 0;
	lc_Return = apsz_String[strlen(apsz_String) - 1];
	apsz_String[strlen(apsz_String) - 1] = 0;

	return lc_Return;
}

/**
\brief  chops off the first character of a string and returns it
\param  apsz_String string
\return character chopped
*/
char String::ChopFront(char* apsz_String) {
	char lc_Return;

	if (!apsz_String) return 0;
	lc_Return = apsz_String[0];
	memmove(apsz_String, &apsz_String[1], strlen(apsz_String));

	return lc_Return;
}

/**
\brief  free a previously allocated split struct
\param  struct with data
\return VOID
*/
void String::FreeSplit(SplitString* ap_Split) {
	int i;
	if (!ap_Split) return;
	/*free strings*/
	for (i = 0; i<ap_Split->Count; i++)
		free(ap_Split->String[i]);
	/*free array*/
	free(ap_Split->String);
}

/**
\brief  get the extension from a file name
\param  apsz_String string to check
\param  apsz_Buffer buffer to copy extension to
\return 0 if failed,1 if success
*/
int String::GetExtensionFromFileName(char* apsz_String, char* apsz_Buffer) {
	char  lsz_Buffer[5];
	int   i, j;
	char* lp_HasDot;

	if ((!apsz_String) || (!apsz_Buffer)) return 0;

	apsz_Buffer[0] = 0;
	memset(lsz_Buffer, 0, 5);

	lp_HasDot = strstr(apsz_String, ".");
	if (!lp_HasDot) return 0;

	j = (int)(strlen(apsz_String) - 1);

	for (i = j; i>0; i--) {
		char lsz_Letter[2] = { 0 };
		if (apsz_String[i] == '.') {
			//extension is successfully read
			strcpy(lsz_Buffer, strrev(lsz_Buffer));
			break;
		}
		lsz_Letter[0] = apsz_String[i];
		strcat(lsz_Buffer, lsz_Letter);
		if (i<(j - 5)) break;
	}

	strcpy(apsz_Buffer, lsz_Buffer);
	return 1;
}

//find a substring in a string
int String::IndexOf(const char* apsz_String, const char* apsz_Keyword) {
	int li_Offset = 0, li_Len, li_KeyLen;

	if ((!apsz_String) || (!apsz_Keyword))return(0);
	//attempt to find keyword in string
	li_Len = (int)strlen(apsz_String);
	li_KeyLen = (int)strlen(apsz_Keyword);

	while ((li_Offset + li_KeyLen) <= li_Len) {
		if (!memcmp((apsz_String + li_Offset), apsz_Keyword, li_KeyLen))
			return li_Offset;
		li_Offset++;
	}

	return -1;
}

bool String::IsNumeric(char* apsz_String) {
	if (!apsz_String) return false;

	while (*apsz_String) {
		if ((*apsz_String<'0') || (*apsz_String>'9')) {
			return false;
		}
		apsz_String++;
	}

	return true;
}

/**
\brief  get a number of chars from the left of a string
\param  apsz_String string
\param  ai_Count number of chars to copy
\param  apsz_Buffer buffer to store result
\return VOID
*/
void String::Left(char* apsz_String, WORD ai_Count, char* apsz_Buffer) {
	if ((!apsz_String) || (!apsz_Buffer))return;
	apsz_Buffer[0] = 0;
	if (ai_Count>strlen(apsz_String))return;

	memcpy(apsz_Buffer, apsz_String, ai_Count);
	apsz_Buffer[ai_Count] = 0;
}

/**
\brief  get a piece of a string from a string
\param  apsz_String string
\param  ai_First char to start at
\param  ai_Count number of chars to copy
\param  apsz_Buffer buffer to store result
\return VOID
*/
void String::Mid(char* apsz_String, WORD ai_First, WORD ai_Count, char* apsz_Buffer) {
	int i, li_Offset = 0;

	if ((!apsz_String) || (!apsz_Buffer))return;
	apsz_Buffer[0] = 0;
	if ((ai_First + ai_Count)>(int)strlen(apsz_String))return;

	for (i = ai_First; i<(ai_First + ai_Count); i++)
		apsz_Buffer[li_Offset++] = apsz_String[i];
	apsz_Buffer[li_Offset] = 0;
	return;
}

/**
\brief  remove all whitespace (eg ' ') from a string
\param  apsz_String string to remove whitespace from
\return VOID
*/


//make this remove all but one whitespace (redundant whitespace)


void String::RemoveAllWhiteSpace(char* apsz_String) {
	int li_Offset = 0, li_Len;

	if (!apsz_String)return;
	li_Len = (int)strlen(apsz_String);

	while (li_Offset != li_Len) {
		if (apsz_String[li_Offset] == ' ') {
			memmove(&apsz_String[li_Offset], &apsz_String[li_Offset + 1], (li_Len - li_Offset));
			li_Len = (int)strlen(apsz_String);
		} else
			li_Offset++;
	}
	return;
}

/**
\brief  remove path name (eg: the "c:/my folder/" part) from a string
\param  apsz_String string to remove path from
\return VOID
*/
void String::RemovePathFromFileName(char* apsz_String) {
	char* lp_Ptr;

	if (!apsz_String)return;

	lp_Ptr = apsz_String + strlen(apsz_String);

	while (lp_Ptr != apsz_String) {
		if ((*lp_Ptr == '/') || (*lp_Ptr == '\\'))
			break;
		lp_Ptr--;
	}
	if (lp_Ptr != apsz_String)
		strcpy(apsz_String, ++lp_Ptr);
	return;
}

/**
\brief  replace all occurances of a substring (case sensitive)
(dynamically allocates memory)
\param  apsz_String string
\param  apsz_Keyword string keyword to be replaced
\param  apsz_ReplaceWith string to replace keyword with
\param  ap_Replaced number of words replaced
\return replaced string
*/
char* String::ReplaceAll(char* apsz_String, char* apsz_Keyword, char* apsz_ReplaceWith, int* ap_Replaced) {
	int   li_Occurances = 0, li_Offset = 0, li_Len, li_KeyLen, li_RepLen, li_NewLen;
	char* lpsz_Return = 0, lsz_Temp[2];

	if ((!apsz_String) || (!apsz_Keyword))return(0);
	//find number of occurances
	li_Len = (int)strlen(apsz_String);
	li_KeyLen = (int)strlen(apsz_Keyword);
	li_RepLen = (int)strlen(apsz_ReplaceWith);
	if (ap_Replaced)*ap_Replaced = 0;
	while ((li_Offset + li_KeyLen) <= li_Len) {
		if (!memcmp((apsz_String + li_Offset), apsz_Keyword, li_KeyLen))
			li_Occurances++;
		li_Offset++;
	}
	//allocate memory
	li_NewLen = (li_Len + (li_Occurances*(li_RepLen - li_KeyLen)));
	lpsz_Return = (char*)malloc(li_NewLen);
	lpsz_Return[0] = 0;
	li_Offset = 0;
	if (ap_Replaced)*ap_Replaced = li_Occurances;
	//build replacement string
	while ((li_Offset + li_KeyLen) <= li_Len) {
		if (!memcmp((apsz_String + li_Offset), apsz_Keyword, li_KeyLen)) {
			strcat(lpsz_Return, apsz_ReplaceWith);
			li_Offset += (li_KeyLen - 1);
		} else {
			lsz_Temp[0] = apsz_String[li_Offset];
			lsz_Temp[1] = 0;
			strcat(lpsz_Return, lsz_Temp);
		}
		li_Offset++;
	}
	return(lpsz_Return);
}

/**
\brief  get a number of chars from the right of a string
\param  apsz_String string
\param  ai_Count number of chars to copy
\param  apsz_Buffer buffer to store result
\return VOID
*/
void String::Right(char* apsz_String, WORD ai_Count, char* apsz_Buffer) {
	int li_First;

	if ((!apsz_String) || (!apsz_Buffer))return;
	apsz_Buffer[0] = 0;
	if (ai_Count>strlen(apsz_String))return;

	li_First = ((int)strlen(apsz_String) - ai_Count);

	memcpy(apsz_Buffer, &apsz_String[li_First], ai_Count);
	apsz_Buffer[ai_Count] = 0;
	return;
}

/*under construction*/
/*int String::Search(char* apsz_String,char* apsz_Keyword,byte ac_Direction)
{
long li_Offset=0,li_TotalSize = (long)strlen(apsz_String);
/*0 = beg->end ; 1 = beg<-end*/
/*if((ac_Direction)&&(ac_Direction!=1))
ac_Direction=0;*/
/*while((memcmp(buffer,apsz_Keyword,strlen(apsz_Keyword)))&&(li_Offset<li_TotalSize))
{
}*/
/*return(0);
}*/

/**
\brief  split a string based on a token
(dynamically allocates memory)
\param  apsz_String string to split
\param  ac_Token token to split on
\return struct with count and each tokenized item separate
*/
SplitString* String::Split(char* apsz_String, char ac_Token) {
	/*
	notes strtok is thread safe on windows; maybe this
	should use strtok_r if <unistd.h> is available
	*/
	SplitString* lp_Ss = 0;
	char** lp_Words = 0;
	char*  lp_Word, *lp_Ptr, *lp_CopyData1, *lp_CopyData2;
	char   lsz_Token[2] = { ac_Token, 0 };
	int    li_Words = 0, i = 0;
	if (!apsz_String)return(0);
	//copy data
	lp_CopyData1 = strdup(apsz_String);
	lp_CopyData2 = strdup(apsz_String);
	lp_Ptr = lp_CopyData1;
	//try to tokenize; get count
	lp_Word = strtok(lp_Ptr, lsz_Token);
	if (!lp_Word)return(0);
	++li_Words;
	while (strtok(0, lsz_Token))
		++li_Words;
	free(lp_CopyData1);
	//allocate
	lp_Ss = (SplitString*)malloc(sizeof(SplitString));
	lp_Ss->Count = li_Words;
	lp_Words = (char**)malloc(sizeof(char*)*li_Words);
	//re-tokenize and copy data
	lp_Ptr = apsz_String;
	lp_Word = strtok(lp_Ptr, lsz_Token);
	lp_Words[i] = strdup(lp_Word);
	while (lp_Word = strtok(0, lsz_Token))
		lp_Words[++i] = strdup(lp_Word);
	lp_Ss->String = lp_Words;
	//(strtok null terminates after a token)
	strcpy(apsz_String, lp_CopyData2);
	free(lp_CopyData2);
	return(lp_Ss);
}

/**
\brief  convert a string to all lowercase
\param  apsz_String string to convert
\return VOID
*/
void String::ToLower(char* apsz_String) {
	if (!apsz_String)return;
	while (*apsz_String) {
		if ((*apsz_String >= 'A') && (*apsz_String <= 'Z'))
			*apsz_String += 32;
		apsz_String++;
	}
	return;
}

/**
\brief  convert a string to all uppercase
\param  apsz_String string to convert
\return VOID
*/
void String::ToUpper(char* apsz_String) {
	if (!apsz_String)return;
	while (*apsz_String) {
		if ((*apsz_String >= 'a') && (*apsz_String <= 'z'))
			*apsz_String -= 32;
		apsz_String++;
	}
	return;
}

/**
\brief  trim whitespace at the front and end of a string
\param  apsz_String string to trim
\return VOID
*/
void String::Trim(char* apsz_String) {
	char* lp_Pos;
	int   li_Len;

	if (!apsz_String)return;
	lp_Pos = (apsz_String + strlen(apsz_String) - 1);

	//trim end
	while ((lp_Pos + 1) != apsz_String) {
		if (*lp_Pos == ' ' || *lp_Pos == 10 || *lp_Pos == 13)
			*lp_Pos = 0;
		else
			break;
		lp_Pos--;
	}

	//trim front
	li_Len = (int)strlen(apsz_String);
	while (li_Len) {
		if (*apsz_String != ' ' && *apsz_String != 10 && *apsz_String != 13)break;
		memmove(apsz_String, &apsz_String[1], li_Len--);
	}
	return;
}

/**
\brief  trim from end of string until hitting a character
\param  apsz_String string to trim
\param  ac_Char char to stop trimming at
\return number of characters trimmed
*/
int String::TrimEndUntil(char* apsz_String, char ac_Char) {
	char* lp_Pos;
	int   li_CharsTrimmed = 0;

	if (!apsz_String)return(0);

	lp_Pos = (apsz_String + strlen(apsz_String) - 1);

	while ((lp_Pos + 1) != apsz_String) {
		if (*lp_Pos != ac_Char) {
			*lp_Pos = 0;
			li_CharsTrimmed++;
		} else
			break;
		lp_Pos--;
	}
	return(li_CharsTrimmed);
}

/**
\brief  trim from front of string until hitting a character
\param  apsz_String string to trim
\param  ac_Char char to stop trimming at
\return number of characters trimmed
*/
int String::TrimFrontUntil(char* apsz_String, char ac_Char) {
	int li_Len, li_CharsTrimmed = 0;

	if (!apsz_String) return 0;

	li_Len = (int)strlen(apsz_String);
	while (li_Len) {
		if (*apsz_String != ac_Char) {
			memmove(apsz_String, &apsz_String[1], li_Len--);
			li_CharsTrimmed++;
		} else
			break;
	}
	return li_CharsTrimmed;
}