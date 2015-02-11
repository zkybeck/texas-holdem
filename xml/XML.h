/*XML.h                                   */
/*------------------------------------------
By Brian Clifton (bclifton@gmail.com)

Author Notes:
This code is free for use in any projects.
The code is provided "as is" without any 
expressed or implied warranty

Description:
Loads XML Documents and provides an interface
for navigating, manipulating, and saving them

Known Problems
this will only load elements and attributes.
schemas are not supported, and xml documents 
are not validated.  invalid xml documents will 
encounter unknown behaviour

Version History:
2006.03.01 - 2.1 - XmlDocument::LoadXML now requires buffer length
                   Added XmlDocument::SaveToMemory
2005.12.11 - 2.0 - Kahnadex::XML
2005.07.16 - 1.6 - added XMLAttributeList::AddAttribute
2005.03.19 - 1.5 - fixed parsing attributes with spaces
                   fixed parsing last attribute when ending with "/>"
2005.03.16 - 1.4 - added XMLNodeList::AddNode
                   Save now returns a bool
2004.10.16 - 1.3 - added DocumentType,LoadXML
                   beginning unicode support.
				   started adding support for
				   node path lookup.
2004.09.16 - 1.2 - fixed some parsing issues
2004.09.13 - 1.1 - wrote memory clean up
                   partially documented for doxygen
2004.09.11 - 1.0 - initial release
------------------------------------------
TODO List:
------------------------------------------
1) Paths need to be added.  
(ex: 'Config/Node1/ returns a node list of matching nodes)
These can be added by using an STL <map>.  
Code below is in place but is commented out
------------------------------------------
2) Unicode Support
Right now, there is absolutely no unicode support.  Internally,
everything should be stored as a wstring.  When parsing chars,
look for typical UTF-8 or UTF-16 items.  Notes are below.
------------------------------------------
UNICODE NOTES: (pardon the mess, I need to add unicode support)
A SAVE METHOD NEEDS TO BE ADDED WHEN SAVING AS UTF-8
if character is unicode or it's value is ANDable with 128
-AND the 1,2 and 4 out
SHIFT RIGHT by 3
OR the value with jjdfkdsjkfjsldkFINISH ME

To check if a character is unicode or not
after reading a character, check if the 128 bit
is on.  If so, AND against 128, 64, and 32.
Make sure 128 = 128, 64 = 64, and 32 = 0

If they are not enabled, just read the character OTHERWISE
If the byte has those enabled, 
-set IsUnicode = true
-Shift the value 3 places to the LEFT.
-read the next byte.  AND out the 32 and 16 and shift them
RIGHT 4 places.  OR this with the previos value
-shift the second byte to the LEFT 2 places.
-read the last byte. XOR this with 128
-OR the 2nd byte and 3rd byte together

FINALLY
-store the complete 1st and second value in a wchar_t
-add this into the wstring member

ANOTHER NOTE
& and other invalid characters < > etc
need to be encoded for xml properly when saved
and decoded when loaded

<,>,& etc. to &lt;, &gt; and &amp;











############################################
NEEDS TO BE DONE REALLY FUCKING SOON

PUT IN A HASH MAP IN THE ATTRIBUTE CONTAINER CLASS
MAP ATTRIBUTE NAMES TO AN ATTRIBUTE OBJECT!

DO *NOT* ALLOW DUPLICATE ATTRIBUTE ENTRIES!!
############################################
------------------------------------------*/
#ifndef KAHNADEX_XML_H
#define KAHNADEX_XML_H
//------------------------------------------
#include "File.h"
#include "String.h"
#include <vector>
#include <string>
//#include <map>
using namespace std;
using namespace Kahnadex;
using namespace Kahnadex::Files;
namespace Kahnadex{
namespace XML{
//------------------------------------------
//possible XML document types
typedef enum
{
	DOCTYPE_UNKNOWN=0,//not specified/no signature
	DOCTYPE_UTF8,     //UTF-8 signature: EF BB BF
	DOCTYPE_UTF16,    //UTF-16 signature: FFFE
} XmlDocumentType;
//------------------------------------------
//one attribute name/value pair
typedef struct
{
	string Name;
	string Value;
} XmlAttribute;
//------------------------------------------
//a list of attributes associated with an XmlNode
class XmlAttributeList
{
	friend class XmlDocument;
	friend class XmlNode;

	//members
	private:
		vector<XmlAttribute*> mv_AttributeList;

	//methods
	private:
		void          Render(string& as_Output);
	public:
		void          AddAttribute(XmlAttribute* ap_Attribute);
		void          Clear(void);
		int           Count(void);
		XmlAttribute* FirstAttribute(void);
		XmlAttribute* Item(int ai_Item);

		~XmlAttributeList(){Clear();};
};
inline int XmlAttributeList::Count(void){
	return((int)mv_AttributeList.size());
}
//------------------------------------------
//a list of sub-nodes associated with an XMLNode
class XmlNode;
class XmlNodeList
{
	friend class XmlDocument;
	friend class XmlNode;

	//members
	private:
		vector<XmlNode*> mv_NodeList;

	//methods
	private:
		void     Render(string& as_Output);
	public:
		void     AddNode(XmlNode* ap_Node);
		void     Clear(void);
		int      Count(void);
		XmlNode* FirstNode(void);
		XmlNode* Item(int ai_Item);

		~XmlNodeList(){Clear();};
};
inline int XmlNodeList::Count(void){
	return((int)mv_NodeList.size());
}
//------------------------------------------
//the actual XML node
//this has attributes and can contain nodes
class XmlNode
{
	friend class XmlDocument;
	friend class XmlNodeList;
	
	//members
	private:
		XmlAttributeList  mh_Attributes;
		XmlNodeList       mh_ChildNodes;
	public:
		string            InnerText;
		string            Name;

	//methods
	private:
		void              CreateChildrenFromInnerText(void);
		void              Render(string& as_Output);
	public:
		XmlAttributeList& Attributes(void);
		XmlNodeList&      ChildNodes(void);

		~XmlNode();

};
inline XmlAttributeList& XmlNode::Attributes(void){
	return(mh_Attributes);
}
inline XmlNodeList& XmlNode::ChildNodes(void){
	return(mh_ChildNodes);
}
//------------------------------------------
//this will recursively load an xml document into
//memory using XMLNodes
class XmlDocument
{
	friend class XmlNode;

	//members
	private:
		XmlDocumentType me_DocType;
	public:
		XmlNodeList ChildNodes;
	//	map<string,XMLNode*> mo_LookUp;

	//methods
	private:
		static char*    GetString(XmlDocumentType ae_DocType);
		static void     Parse(BYTE* ap_Data,int ai_Length,XmlNodeList* ap_NodeList);
		void            SetDocumentType(BYTE* ap_Data,int ai_Length);
	public:
		void            Clear(void);
		XmlDocumentType DocumentType(void);
		void            Load(char* apsz_FilePath);
		void            LoadXML(char* apsz_XML,int ai_Length);
		bool            Save(char* apsz_FilePath);
		char*           SaveToMemory(int* ap_Length);
	//	XMLNode* GetNodeByPath(char* apsz_DocElemPath);

		XmlDocument(){me_DocType=DOCTYPE_UNKNOWN;}
		~XmlDocument(){Clear();};
};
inline XmlDocumentType XmlDocument::DocumentType(void){
	return(me_DocType);
}
//------------------------------------------
}}
#endif