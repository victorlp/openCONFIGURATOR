///////////////////////////////////////////////////////////////////////////////////////////////
//
//  $Header: $
//
// NAME:  ClassNAME
//
// BASE  CLASSES: none
//  
// PURPOSE:  purpose description
//
// AUTHOR:  
//
//  COPYRIGHT NOTICE:
//
//	****************************************************************************

// (c) Kalycito Infotech Private Limited
//
//  Project:      openCONFIGURATOR 
//
//  Description:  
//
//
//  License:
//
//    Redistribution and use in source and binary forms, with or without
//    modification, are permitted provided that the following conditions
//    are met:
//
//    1. Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//    2. Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//
//    3. Neither the name of Kalycito Infotech Private Limited nor the names of 
//       its contributors may be used to endorse or promote products derived
//       from this software without prior written permission. For written
//       permission, please contact info@kalycito.com.
//
//    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
//    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
//    COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
//    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//    ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//    POSSIBILITY OF SUCH DAMAGE.
//
//    Severability Clause:
//
//        If a provision of this License is or becomes illegal, invalid or
//        unenforceable in any jurisdiction, that shall not affect:
//        1. the validity or enforceability in that jurisdiction of any other
//           provision of this License; or
//        2. the validity or enforceability in other jurisdictions of that or
//           any other provision of this License.
//
//****************************************************************************/
//
//  REVISION HISTORY:
// $Log:      $
///////////////////////////////////////////////////////////////////////////////////////////////

#include "../Include/ObjectDictionary.h"
#include "../Include/Declarations.h"
#include "../Include/NodeCollection.h"
#include "../Include/Index.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <libxml\xmlreader.h>
#include <errno.h>

string strerr;

void LoadObjectDictionary()
	{
	
		//Parse the ObjectDictionary xml and creates the objectDictionary object
		
	}
/**************************************************************************************************
	* Function Name: ImportXML
    * Description: Imports the XML file
/****************************************************************************************************/
static void ImportXML(string fileName,string filePath,string errorstring,int NodeID,int NodeType)
	{
		string fileLocation;
		fileLocation= fileName + filePath;
		fileLocation ="E:\\Kalycito\\checkout\\Source\\SharedLibrary\\openCONFIGURATORSoln\\XMLParser\\12345678_MyName.xml";
		parseFile(fileLocation,NodeType,NodeID);


		//Cleanup function for the XML library.
   
		 xmlCleanupParser();
    /*
     * this is to debug memory for regression tests
     */
    xmlMemoryDump();
    /*return(0);*/


	}
/**************************************************************************************************
	* Function Name: processNode
    * Description: Process the Node value,Name and its attributes
/****************************************************************************************************/
void processNode(xmlTextReaderPtr reader)
{
    const xmlChar *name, *value;
	CIndex objIndex;
	string AttributeName;
	CNodeCollection *objNodeCollection;
	
	
    name = xmlTextReaderConstName(reader);
    if (name == NULL)
	name = BAD_CAST "--";
	
    value = xmlTextReaderConstValue(reader);
	//If the NodeTYPE is ELEMENT
	if( xmlTextReaderNodeType(reader)==1)
		{		
			
			char* UpperCaseElementName;
			UpperCaseElementName=(char*)name;
			if(strcmp(UpperCaseElementName,"Object")==0)
				{
					if (xmlTextReaderHasAttributes(reader)==1)
						{
							objNodeCollection= CNodeCollection::getNodeColObject();
							while(xmlTextReaderMoveToNextAttribute(reader))
								{
									
									value = xmlTextReaderConstValue(reader);
									name =xmlTextReaderConstName(reader);							
									AttributeName =string(strupr((char*)name));
									if(AttributeName == "INDEX")							
									objIndex.setIndex(string((char*)value));
											
									}
							//objNodeCollection->getNode(NodeType,NodeID);
						}

				}
		}
					
}
/**************************************************************************************************
	* Function Name: parseFile
    * Description: Parses the XML file
/****************************************************************************************************/
void parseFile(string filename,int NodeType,int NodeID) 
{
    xmlTextReaderPtr reader;
    int ret;

	strerr="";
    reader = xmlReaderForFile(filename.c_str(), NULL, 0);
    if (reader != NULL)
	{
        ret = xmlTextReaderRead(reader);
        while (ret == 1)
		{
            processNode(reader);
            ret = xmlTextReaderRead(reader);
        }
        xmlFreeTextReader(reader);
        if (ret != 0)
		{
            fprintf(stderr, "%s : failed to parse\n", filename);
			/*strerr= strerror(errno);
			strerr =" Error:" +strerr;*/
        }
    }
	else 
		{
			strerr=strerror(errno);
			strerr =" Error:" +strerr;
       /* fprintf(stderr, "Unable to open %s\n", filename);*/
		}
 }
/**************************************************************************************************
	* Function Name: CreateTree
    * Description:
/****************************************************************************************************/
void CreateTree()
	{
		//objNodeCollection= CNodeCollection::getNodeColObject();
		
	}
/**************************************************************************************************
	* Function Name: GenerateCDC
    * Description: Generates the CDC file
/****************************************************************************************************/
void GenerateCDC(string fileName, string filePath)
	{
		CNode objMNNode;		
		string fullFileName;

		//get the MN Node object from the NodeCollection
		fullFileName = fileName + filePath;
		ofstream file;
		//file.open(fullFileName);


		//char *WriteBuffer;		

	}
/**************************************************************************************************
	* Function Name: GenerateMNOBD
    * Description: Generates the MN Object Dictionary
/****************************************************************************************************/
void GenerateMNOBD()
	{
		CNodeCollection *objNodeCollection;
		objNodeCollection= CNodeCollection::getNodeColObject();

		//get the MN Node
		CNode *objMNNode;
		
		objMNNode=objNodeCollection->getNode(MN,249);

		//get the Index collection of MN Object
		CIndexCollection *objIndexCollection;
		objIndexCollection = objMNNode->getIndexCollection();

		//loop through the Indexes of IndexCollection
		int count=0;
		for (count=0;count < objIndexCollection->getNumberofIndexes(); count++)
			{
				CIndex objIndex;
				objIndex = objIndexCollection->getIndex(count);
				objIndex.getIndex();
				
			}




	}
int main(int argc, char **argv) {
   /* if (argc != 2)
        return(1);*/

    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
   
   // parseFile("E:\\Kalycito\\checkout\\Source\\SharedLibrary\\openCONFIGURATORSoln\\XMLParser\\12345678_MyName.xml",NodeType,NodeID);

    /*
     * Cleanup function for the XML library.
     */
    xmlCleanupParser();
    /*
     * this is to debug memory for regression tests
     */
    xmlMemoryDump();
    return(0);
}
