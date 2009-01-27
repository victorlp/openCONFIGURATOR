#include "../Include/openCONFIGURATOR.h"
#include "../Include/Internal.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <libxml/xmlreader.h>
#include <errno.h>
#include <string.h>
int main(int argc, char **argv) 
{
   /* if (argc != 2)
        return(1);*/

    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
   
    //parseFile("E:\\Kalycito\\checkout\\Source\\SharedLibrary\\openCONFIGURATORSoln\\XMLParser\\12345678_MyName.xml",NodeType,NodeID);

	
	char ch='N';
	int opt = 0;;
	
	int NodeID;
	int NodeType;
	char filePath[100];	
	char* errorstring;
	errorstring = new char;		
						
	CNodeCollection *objNodeCollection;							

	CNode objNode;
	CIndexCollection *objIndexCollection;							
	
	while(ch!='E')
	{
			system("cls");

			cout <<"\nPlease Choose the option"<<endl;
			cout <<"1.Create the Node"<<endl;
			cout <<"2.Parse the File"<<endl;
			cout <<"3.GenerateCDC"<<endl;
			cout <<"4.Total Number of Nodes"<<endl;
			cout<<"5:Total Indexes in a collection"<<endl;
			cout<<"6:Index Properties"<<endl;
			cout<<"7:SubIndex Properties"<<endl;
			cout<<"6:Index Properties"<<endl;						
			cout <<"Press 'E' for Exit"<<endl;
								
			cin>>opt;
	
			switch (opt)
		{
				case 1:
								system("cls");
								cout<<"Enter the NodeID"<<endl;								
								cin >> NodeID;
								cout<<"Enter the Node Type(MN=0/CN=1)"<<endl;								
								cin>>NodeType;
								if (NodeType ==0 )
								CreateNode(NodeID,MN);								
								else if (NodeType==1)
								CreateNode(NodeID,CN);
								cout <<"Node Created"<<endl;
								cout <<"Do you want to Parse the file(y/n)?"<<endl;
								char c;
								cin >> c;
								if(c=='y')
								{
									cout<<"Enter the filepath"<<endl;
									cin>>filePath;
									char* errorstring;
									errorstring = new char;
									if (NodeType ==0 )							
									ImportXML(filePath, errorstring, NodeID, MN);
									else if (NodeType==1)
									ImportXML(filePath, errorstring, NodeID, CN);															
									cin.get();
									cin.get();
							}
							/*	opt=2;*/
								break;
				case 2:							
							cout<<"Enter the Node ID"<<endl;
							cin>>NodeID;
							cout<<"Enter the Node Type(MN=0/CN=1)"<<endl;								
							cin>>NodeType;
							cout<<"Enter the filepath"<<endl;
							cin>>filePath;
							int ret;					
							if (NodeType ==0 )							
							ret = ImportXML(filePath, errorstring, NodeID, MN);
							else if (NodeType==1)
							ret = ImportXML(filePath, errorstring, NodeID, CN);		
							cin.get();
							cin.get();							
							break;

				case 3:							
							cout<<"Enter the filepath"<<endl;
							cin>>filePath;
							GenerateCDC(filePath);  
							break;
				case 4:
							objNodeCollection= CNodeCollection::getNodeColObjectPointer();	
							cout<<"Total Number of Nodes are:"<<objNodeCollection->getNumberOfNodes();
							cin.get();
							cin.get();
							break;
				case 5:
							cout<<"Enter the Node ID"<<endl;
							cin>>NodeID;
							cout<<"Enter the Node Type(MN=0/CN=1)"<<endl;								
							cin>>NodeType;
							objNodeCollection= CNodeCollection::getNodeColObjectPointer();
	
							if (NodeType ==0 )
							objNode =objNodeCollection->getNode(MN,NodeID);
							else if (NodeType==1)
							objNode =objNodeCollection->getNode(CN,NodeID);
							objIndexCollection = objNode.getIndexCollection();
							cout<<"Total Number of Indexes in a collection"<<objIndexCollection->getNumberofIndexes()<<endl;
							cin.get();
							cin.get();
							break;
				default :
								break;
		}
	
	};
}
