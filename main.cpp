//main.cpp
//poker
//------------------------------------------
#include "main.h"
//------------------------------------------
int main(int argc,char** argv)
{
	TexasHoldem x;
	char*       in,*out;
	int         len;

	
	char* ifile = "in.xml";
	char* ofile = "out.xml";


	in = (char*)File::ReadAllAsBinary(ifile,&len);
	x.Load(in,len);
	x.Process();
	out = x.Save(&len);
	File::SaveDataAsBinary(ofile,(BYTE*)out,len);

	free(in);
	free(out);

	return(0);
}
//------------------------------------------