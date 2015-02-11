//XML.cpp
//see header file for more information
//------------------------------------------
#include "XML.h"
using namespace Kahnadex;
using namespace Kahnadex::Files;
using namespace Kahnadex::XML;
//------------------------------------------
//XML ATTRIBUTE LIST
//------------------------------------------
void XmlAttributeList::AddAttribute(XmlAttribute* ap_Attribute)
{
	if(ap_Attribute)
		mv_AttributeList.push_back(ap_Attribute);
	return;
}
//------------------------------------------
/**
\brief  free attribute memory, clear list
\return VOID
*/
void XmlAttributeList::Clear(void)
{
	vector<XmlAttribute*>::iterator it = mv_AttributeList.begin();
	while(it != mv_AttributeList.end()){
		delete (*it);
		it = mv_AttributeList.erase(it);
	}
	return;
}
//------------------------------------------
/**
\brief  get the first attribute in the list
\return XmlAttribute, or null if none are present
*/
XmlAttribute* XmlAttributeList::FirstAttribute(void)
{
	if(mv_AttributeList.size()>0)
		return(mv_AttributeList[0]);
	return(0);
}
//------------------------------------------
/**
\brief  return an item with a specific index
\param  ai_Item the index of the item to retrieve
\return an XmlAttribute or null if invalid index
*/
XmlAttribute* XmlAttributeList::Item(int ai_Item)
{
	if((ai_Item>=0)&&(ai_Item<(int)mv_AttributeList.size()))
		return(mv_AttributeList[ai_Item]);
	return(0);
}
//------------------------------------------
/**
\brief  render the representation of child attributes as XML
\param  as_Output string to append XML for this item
\return VOID
*/
void XmlAttributeList::Render(string& as_Output)
{
	int i;
	for(i=0;i<mv_AttributeList.size();i++){
		as_Output.append(mv_AttributeList[i]->Name+"=\""+mv_AttributeList[i]->Value+"\"");
		if(i!=mv_AttributeList.size()-1)
			as_Output.append(" ");
	}
	return;
}
//------------------------------------------
//XML NODE LIST
//------------------------------------------
void XmlNodeList::AddNode(XmlNode* ap_Node)
{
	if(ap_Node)
		mv_NodeList.push_back(ap_Node);
	return;
}
//------------------------------------------
/**
\brief  free node memory, clear list
\return VOID
*/
void XmlNodeList::Clear(void)
{
	vector<XmlNode*>::iterator it = mv_NodeList.begin();
	while(it != mv_NodeList.end()){
		delete (*it);
		it = mv_NodeList.erase(it);
	}
	return;
}
//------------------------------------------
/**
\brief  get the first node in the list
\return XmlNode, or null if none are present
*/
XmlNode* XmlNodeList::FirstNode(void)
{
	if(mv_NodeList.size()>0)
		return(mv_NodeList[0]);
	return(0);
}
//------------------------------------------
/**
\brief  return an item with a specific index
\param  ai_Item the index of the item to retrieve
\return an XmlNode or null if invalid index
*/
XmlNode* XmlNodeList::Item(int ai_Item)
{
	if((ai_Item>=0)&&(ai_Item<(int)mv_NodeList.size()))
		return(mv_NodeList[ai_Item]);
	return(0);
}
//------------------------------------------
/**
\brief  render the representation of child nodes as XML
\param  as_Output string to append XML for this item
\return VOID
*/
void XmlNodeList::Render(string& as_Output)
{
	int i;
	for(i=0;i<mv_NodeList.size();i++){
		mv_NodeList[i]->Render(as_Output);
	}
	return;
}
//------------------------------------------
//XML NODE
//------------------------------------------
/**
\brief deconstructor
*/
XmlNode::~XmlNode()
{
	mh_Attributes.Clear();
	mh_ChildNodes.Clear();
}
//------------------------------------------
/**
\brief  render the representation of this node as XML
\param  as_Output string to append XML for this item
\return VOID
*/
void XmlNode::Render(string& as_Output)
{
	if(mh_ChildNodes.Count()==0){
		if(mh_Attributes.Count()>0){
			as_Output.append("<"+Name+" ");
			mh_Attributes.Render(as_Output);
			if(InnerText=="")
				as_Output.append("/");
			as_Output.append(">");
		}else{
			if(InnerText=="")
				as_Output.append("<"+Name+"/>");
			else
				as_Output.append("<"+Name+">");
		}
		if(InnerText!=""){
			as_Output.append(InnerText);
			as_Output.append("</"+Name+">");
		}
	}else{
		if(mh_Attributes.Count()>0){
			as_Output.append("<"+Name+" ");
			mh_Attributes.Render(as_Output);
			as_Output.append(">");
		}else
			as_Output.append("<"+Name+">");
		mh_ChildNodes.Render(as_Output);
		as_Output.append("</"+Name+">");
	}
	return;
}
//------------------------------------------
void XmlNode::CreateChildrenFromInnerText(void)
{
	XmlDocument::Parse((BYTE*)InnerText.c_str(),strlen(InnerText.c_str()),&mh_ChildNodes);
	return;
}
//------------------------------------------
//XML DOCUMENT
//------------------------------------------
void XmlDocument::Clear(void)
{
	ChildNodes.Clear();
	me_DocType=DOCTYPE_UNKNOWN;
	return;
}
//------------------------------------------
/*XmlNode* XmlDocument::GetNodeByPath(char* apsz_DocElemPath)
{
	return(0);
}*/
//------------------------------------------
char* XmlDocument::GetString(XmlDocumentType ae_DocType)
{
	switch(ae_DocType){
		case DOCTYPE_UTF8:
			return("UTF-8");
		case DOCTYPE_UTF16:
			return("UTF-16");
	}
	return("");
}
//------------------------------------------
void XmlDocument::Load(char* apsz_FilePath)
{
	char* lp_File;
	long  len;

	lp_File = File::ReadAllAsText(apsz_FilePath);
	//DECODE XML HERE!!!
	len = strlen(lp_File);
	SetDocumentType((BYTE*)lp_File,len);
	Parse((BYTE*)lp_File,len,&ChildNodes);
	free(lp_File);

	return;
}
//------------------------------------------
void XmlDocument::LoadXML(char* apsz_XML,int ai_Length)
{
	Clear();
	SetDocumentType((BYTE*)apsz_XML,ai_Length);
	Parse((BYTE*)apsz_XML,ai_Length,&ChildNodes);
	return;
}
//------------------------------------------
void XmlDocument::Parse(BYTE* ap_Data,int ai_Length,XmlNodeList* ap_NodeList)
{
	long length = ai_Length;
	long pos=0;
	char lsz_Buffer[100000]={0};
	bool inAngle=false;

	while(pos<length){
TOP:
		char ch[2]={0};
		ch[0] = ap_Data[pos++];

		if(ch[0]=='<')inAngle=true;

		//read past <? xml version="1.0" ?>
		if(inAngle&&ch[0]=='?'){
DOCVERSION:
			//document version
			while(1){
				ch[0] = ap_Data[pos];
				pos++;
				if(ch[0]=='>'){
					lsz_Buffer[0]=0;
					inAngle=false;
					goto TOP;
				}
			}
		}
		//read in an element
		if(inAngle){
			XmlNode* node;
			lsz_Buffer[0]=0;

			ch[0] = ap_Data[pos];
			pos++;

			if(ch[0]=='?')goto DOCVERSION;

			node = new XmlNode;
			node->Name="";
			node->InnerText="";

			if(ch[0]!=10&&ch[0]!=13)
				strcat(lsz_Buffer,ch);

			//read element name
			while(1){
				ch[0] = ap_Data[pos++];

				if(ch[0]==' '||ch[0]=='>'||ch[0]=='/'){
					char EndTag[2048];
					char EndTag2[2048];
					bool HasInnerText=true;
					bool InsideQuotes=false;

					if(ch[0]=='/')
						HasInnerText=false;

					//save element name
					node->Name = lsz_Buffer;
					//clear buffer
					lsz_Buffer[0]=0;

					//ATTRIBUTE READING--------------------------------------------
					if(ch[0]==' '){
						XmlAttribute* a = new XmlAttribute;
						a->Name="";
						a->Value="";
						while(1){
							ch[0] = ap_Data[pos];
							pos++;

							if(ch[0]=='='){
								if(!InsideQuotes){
									a->Name = lsz_Buffer;
									lsz_Buffer[0]=0;
								}else{
									strcat(lsz_Buffer,ch);
								}
							}
							else if(ch[0]=='>'){
								if(!InsideQuotes){
									ATTR_DONE:
									//attributes are read
									a->Value = lsz_Buffer;
									node->mh_Attributes.mv_AttributeList.push_back(a);
								}else{
									strcat(lsz_Buffer,ch);
								}
								//break out of reading attributes
								//time to read inner text
								break;
							}else if(ch[0]=='"'){
								InsideQuotes=!InsideQuotes;
							}else if(ch[0]=='/'){
								if(!InsideQuotes){
									HasInnerText = false;
									while(1){
										ch[0] = ap_Data[pos++];
										//add completed attribute
										if(ch[0]=='>'){
											goto ATTR_DONE;
										}
									}
								}else{
									strcat(lsz_Buffer,ch);
								}
							}else if(ch[0]==' ' && !InsideQuotes){
								//add completed attribute
								a->Value = lsz_Buffer;
								node->mh_Attributes.mv_AttributeList.push_back(a);
								a = new XmlAttribute;
								//clear buffers, continue reading attributes
								a->Name="";
								a->Value="";
								lsz_Buffer[0]=0;
							}else{
								//if not a CR or LF, append character to attribute buffer
								if(ch[0]!=10&&ch[0]!=13&&ch[0]!='"')
									strcat(lsz_Buffer,ch);
							}
						}
					}
					//INNER TEXT READING-------------------------------------------
					if(HasInnerText){
						//READ NODE INNER TEXT
						sprintf(EndTag,"</%s>",node->Name.c_str());
						lsz_Buffer[0]=0;
						while(1){
							ch[0] = ap_Data[pos++];

							if(ch[0]!=10&&ch[0]!=13)
								strcat(lsz_Buffer,ch);

							String::Right(lsz_Buffer,(WORD)strlen(EndTag),EndTag2);
							if(!strcmp(EndTag,EndTag2)){
								lsz_Buffer[strlen(lsz_Buffer)-strlen(EndTag)]=0;
								String::Trim(lsz_Buffer);
								node->InnerText = lsz_Buffer;
								node->CreateChildrenFromInnerText();
								break;
							}
						}
					}

					//add node to container
					ap_NodeList->mv_NodeList.push_back(node);
					inAngle=false;
					goto TOP;
				}else{
					if(ch[0]!=10&&ch[0]!=13)
						strcat(lsz_Buffer,ch);
				}
			}
		}
		if(ch[0]!=10&&ch[0]!=13&&ch[0]!=' ')
			strcat(lsz_Buffer,ch);
	}
	return;
}
//------------------------------------------
bool XmlDocument::Save(char* apsz_FilePath)
{
	string ls_Document="";
	FILE*  lp_File;
	
	//render the InnerText for all XML nodes
	ChildNodes.Render(ls_Document);
	//XML ENCODE THE DOCUMENT HERE!!!

	//open file for writing
	lp_File = fopen(apsz_FilePath,"w");
	if(!lp_File)return(false);

	//write data
	fprintf(lp_File,"<?xml version=\"1.0\" ?>\n%s",ls_Document.c_str());
	fclose(lp_File);

	return(true);
}
//------------------------------------------
char* XmlDocument::SaveToMemory(int* ap_Length)
{
	string ls_Document="<?xml version=\"1.0\" ?>";
	FILE*  lp_File;
	
	//render the InnerText for all XML nodes
	ChildNodes.Render(ls_Document);
	//XML ENCODE THE DOCUMENT HERE!!!

	if(ap_Length)*ap_Length = ls_Document.length();

	//return a copy of the xml document in memory
	return(strdup(ls_Document.c_str()));
}
//------------------------------------------
void XmlDocument::SetDocumentType(BYTE* ap_Data,int ai_Length)
{
	me_DocType=DOCTYPE_UNKNOWN;

	if(ai_Length>=2){
		BYTE lc_UTF16[2]={0xFF,0xFE};
		if(memcmp(ap_Data,lc_UTF16,2))
			me_DocType=DOCTYPE_UTF16;
	}
	if(ai_Length>=3){
		BYTE lc_UTF8[3]={0xEF,0xBB,0xBF};
		if(memcmp(ap_Data,lc_UTF8,3))
			me_DocType=DOCTYPE_UTF8;
	}
	return;
}
//------------------------------------------