#include "../Include/openCONFIGURATOR.h"
#include "../Include/Internal.h"
#include "../Include/Exception.h"
#include <iostream>
#include <fstream>
#include <exception>
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
	//int IndexPos;
	char* IndexID;
	char* SubIndexID;
	char* IndexValue;
	char* IndexName;
	EAttributeType AttributeType;
	int intAttributeType;
	int NodePos;
	int IndexPos;
	int SubIndexPos;
		
	IndexValue = new char;
	IndexName = new char;
	IndexID = new char;
	SubIndexID = new char;
	char filePath[100];	
	char* errorstring;
	errorstring = new char;	
	int temp_ret = 0;
						
	CNodeCollection *objNodeCollection;							

	CNode objNode;
	CIndexCollection *objIndexCollection;		
	ocfmRetCode retCode;		
	
	while(ch!='E')
	{
			system("cls");

			cout <<"\nPlease Choose the option"<<endl;
			cout <<"0.ImportObjDictXML"<<endl;
			cout <<"1.Create the Node"<<endl;
			cout <<"2.Parse the File"<<endl;
			cout <<"3.GenerateCDC"<<endl;
			cout <<"4.Total Number of Nodes"<<endl;
			cout<<"5:Total Indexes in a collection"<<endl;
			cout<<"6:Index Properties"<<endl;
			cout<<"7:SubIndex Properties"<<endl;
			cout<<"8:Index Properties"<<endl;						
			cout<<"9:Delete a Node"<<endl;
			cout<<"10:Add Index"<<endl;			
			cout<<"11:Add SubIndex"<<endl;
			cout<<"12: DisplayNodeTree"<<endl;
			cout<<"13: IfNodeExists"<<endl;
			cout<<"14: IfIndexExists"<<endl;
			cout<<"15: Delete Index"<<endl;
			cout<<"16: Delete SubIndex"<<endl;
			cout<<"17: ReImportXML"<<endl;
			cout<<"18: EditIndexValue"<<endl;
			cout<<"19: GetIndexAttributes"<<endl;
			cout<<"20: Generate XAP"<<endl;
			cout<<"21: GetSubIndexAttributes"<<endl;
			cout<<"22: GetCounts:Node, Index, SubIndex"<<endl;
			cout<<"23: GetNodeIDbyNodePos"<<endl;
			cout<<"24: GetIndexIDbyIndexPos"<<endl;
			cout<<"25: DeleteMNObjDict"<<endl;
			cout<<"26: GetIndexAttributesbyPositions"<<endl;
			
			cout <<"Press 'E' for Exit"<<endl;
								
			cin>>opt;
	
			switch (opt)
		{
				case 0:
								/*ImportObjDictXML("/home/selva/Desktop/mn.xdc");*/
								cout << "Enter the ObjectDicitonary file path "<< endl;
								cin>>filePath;
								LoadObjectDictionary(filePath);
								break;
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
									retCode = ImportXML(filePath, NodeID, MN);
									else if (NodeType==1)
									retCode = ImportXML(filePath, NodeID, CN);			
									if(retCode.code !=0)												
									cout <<"\n Error:"<< retCode.errorString;
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


							ocfmRetCode ret;

							if (NodeType ==0 )							
							ret = ImportXML(filePath,NodeID, MN);
							else if (NodeType==1)
							ret = ImportXML(filePath, NodeID, CN);		
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
							
				case 9:
							system("clear");
							cout<<"Enter the NodeID:"<<endl;								
							cin >> NodeID;												
							cout<<"Enter the Node Type(MN=0/CN=1)"<<endl;								
							cin>>NodeType;
							//objNodeCollection= CNodeCollection::getNodeColObjectPointer();	
							//cout<<"Total Number of Nodes are:"<<objNodeCollection->getNumberOfNodes();
							//ocfmRetValError retPos;
							//temp_ret 
							//retPos = IfNodeExists(NodeID, CN, errorstring);
							////if(temp_ret == -1)								
							//if(retPos.errCode.code == OCFM_ERR_SUCCESS)								
							//{	
							if (NodeType == 1)
								DeleteNode(NodeID, CN);
							else if (NodeType == 0)
								DeleteNode(NodeID, MN);
								//cout <<"Node Deleted"<<endl;
							//}
							//else
							//{
							//	//DeleteNode(temp_ret);
							//	printf("\n\n\nErr:%s!!\n\n\n", errorstring);								
							//}
							break;
				case 10:
							system("clear");
							cout<<"Enter the NodeID:"<<endl;								
							cin >> NodeID;												
							cout<<"Enter the Node Type(MN=0/CN=1)"<<endl;								
							cin>>NodeType;
							cout<<"Enter the IndexID:"<<endl;								
							cin >> IndexID;
							if (NodeType == 0 )
							//$SAddIndex_SubIndex(NodeID, MN, IndexID, NULL);
							AddIndex(NodeID, MN, IndexID);
							else if (NodeType == 1)
							//$SAddIndex_SubIndex(NodeID, CN, IndexID, NULL);
							AddIndex(NodeID, CN, IndexID);
							break;
				case 11:
							system("clear");
							cout<<"Enter the NodeID:"<<endl;								
							cin >> NodeID;												
							cout<<"Enter the Node Type(MN=0/CN=1)"<<endl;								
							cin>>NodeType;
							cout<<"Enter the IndexID:"<<endl;
							cin >> IndexID;
							cout<<"Enter the SubIndexID:"<<endl;
							cin >> SubIndexID;
							if (NodeType == 0 )
							AddSubIndex(NodeID, MN, IndexID, SubIndexID);
							if (NodeType == 1 )
							AddSubIndex(NodeID, CN, IndexID, SubIndexID);

							break;
							
				case 12:
							system("clear");
							DisplayNodeTree();							
							break;
							
				case 13:
							cout << "ErrStr-Case13:" << errorstring << endl;
							system("clear");							
							cout<<"Enter the NodeID:"<<endl;								
							cin >> NodeID;												
							cout<<"Enter the Node Type(MN=0/CN=1)"<<endl;								
							cin>>NodeType;
							ocfmRetValError retPos;
							try
							{
							if(NodeType == 1)
							{
							/*	retPos = IfNodeExists(NodeID, CN, &NodePos);
							*/}	
							else if(NodeType == 0)
								/*retPos = IfNodeExists(NodeID, MN, &NodePos);
							*/if(retPos.errCode.code == 0)
							{
								printf("\n\nIfNodeExists ret:%d\n\n", retPos.returnValue);
								
							}
					/*		else
							{
								printf("\n\n\nGot Err String:%s\n\n\n", retPos.errCode.errorString);
								ocfmException objException;
								objException.ocfm_Excpetion(OCFM_ERR_NODE_ALREADY_EXISTS);
								throw objException;
							}*/
						
						 }
						 catch (ocfmException ex)
							{
							/*	return ex.objRetCode;	*/
								//cout << "IfNodeExists" <<e.what()<< endl;
								cout << "IfNodeExists" << endl;
							} 
								break;

						   
				case 14:
							system("clear");							
							cout<<"Enter the NodeID:"<<endl;								
							cin >> NodeID;												
							cout<<"Enter the Node Type(MN=0/CN=1)"<<endl;								
							cin>>NodeType;
							cout<<"Enter the IndexID:"<<endl;
							cin >> IndexID;
							//try
							//{
							ocfmRetCode Tmp_ErrStruct;
							if(NodeType == 1)
								Tmp_ErrStruct = IfIndexExists(NodeID, CN, IndexID, &IndexPos);
							else if(NodeType == 0)
								Tmp_ErrStruct = IfIndexExists(NodeID, MN, IndexID, &IndexPos);
							if(Tmp_ErrStruct.code == OCFM_ERR_SUCCESS)
							{
								//IndexPos = ErrStruct.returnValue;
								cout << "OCFM_ERR_INDEX_ALREADY_EXISTS" << endl;
							}
							else if(Tmp_ErrStruct.code == OCFM_ERR_INVALID_INDEXID)
							{
								cout << "OCFM_ERR_INVALID_INDEXID - Can be Added" << endl;
								//throw ErrStruct.errCode.code;
							}								
							break;
							//}
							//catch (exception& e)
							//catch (int e)
							//{	
								//cout << e.what() <<  endl;
								//cout << "Error Message:" << ErrStruct.errCode.code << endl;
								//cout << "Error Message:" << e << endl;
								//cout <<  IfIndexExists << endl;
								//break;
							//} 
				case 15:
							system("clear");							
							cout<<"Enter the NodeID:"<<endl;								
							cin >> NodeID;												
							cout<<"Enter the Node Type(MN=0/CN=1)"<<endl;								
							cin>>NodeType;
							cout<<"Enter the IndexID to be deleted:"<<endl;
							cin >> IndexID;
							// IfIndexExists returns the Index position if the Index exists
							if(NodeType == 1)
								DeleteIndex(NodeID, CN, IndexID);					
							else if(NodeType == 0)
								DeleteIndex(NodeID, MN, IndexID);
							
							break;
				case 16:
							system("clear");							
							cout<<"Enter the NodeID:"<<endl;								
							cin >> NodeID;												
							cout<<"Enter the Node Type(MN=0/CN=1)"<<endl;								
							cin>>NodeType;
							cout<<"Enter the IndexID:"<<endl;
							cin >> IndexID;
							cout<<"Enter the SubIndexID to be deleted:"<<endl;
							cin >> SubIndexID;
							if(NodeType == 1)
								DeleteSubIndex(NodeID, CN, IndexID, SubIndexID);
							else if(NodeType == 0)
								DeleteSubIndex(NodeID, MN, IndexID, SubIndexID);
							break;
				
				case 17:							
							cout<<"Enter the Node ID"<<endl;
							cin>>NodeID;
							cout<<"Enter the Node Type(MN=0/CN=1)"<<endl;								
							cin>>NodeType;
							cout<<"Enter the filepath"<<endl;

							cin>>filePath;				
							if (NodeType == 0)							
							/*ret = ReImportXML(filePath, NodeID, MN);
							cin>>filePath;							
							if (NodeType == 0)
							{
								ocfmRetCode ret;
								ret = ReImportXML(filePath, NodeID, MN);
							}
							else if (NodeType == 1)
							ret = ReImportXML(filePath, NodeID, CN);*/						

							{
								ocfmRetCode ret;
								ret = ReImportXML(filePath, NodeID, CN);						
							}

							break;
				case 18:							
							system("clear");							
							cout<<"Enter the NodeID:"<<endl;								
							cin >> NodeID;												
							cout<<"Enter the Node Type(MN=0/CN=1)"<<endl;								
							cin>>NodeType;
							cout<<"Enter the IndexID:"<<endl;
							cin >> IndexID;
							cout<<"Enter the SubIndexID:"<<endl;
							cin >> SubIndexID;
							cout<<"Enter the IndexValue:"<<endl;
							cin >> IndexValue;
							cout<<"Enter the IndexName:"<<endl;
							cin >> IndexName;
							if (NodeType == 0)					
							SetSubIndexAttributes(NodeID, MN, IndexID, SubIndexID,IndexValue, IndexName);
							//SetIndexAttributes(NodeID, MN, IndexID, IndexValue, IndexName);
							//(int NodeID, ENodeType NodeType, char* IndexID, char* IndexValue, char* IndexName);
							else if (NodeType == 1)
							SetSubIndexAttributes(NodeID, CN, IndexID, SubIndexID,IndexValue, IndexName);
							//SetIndexAttributes(NodeID, CN, IndexID, IndexValue, IndexName);
							break;

				case 20:
							cout<<"Enter the filepath"<<endl;
							cin>>filePath;
							ret = GenerateXAP(filePath);  
							if(ret.code ==0)
							printf("\n Success");
							else
							printf("%s",ret.errorString);
							cin.get();
							cin.get();
							break;
				case 19:	
							system("clear");							
							cout<<"Enter the NodeID:"<<endl;								
							cin >> NodeID;												
							cout<<"Enter the Node Type(MN=0/CN=1)"<<endl;								
							cin>>NodeType;
							cout<<"Enter the IndexID:"<<endl;
							cin >> IndexID;
							cout<<"Enter the AttributeType:"<<endl;
							cout <<" Enter 0 for NAME" << endl;
							cout <<" Enter 1 for OBJECTTYPE" << endl;
							cout <<" Enter 2 for DATATYPE" << endl;
							cout <<" Enter 3 for ACCESSTYPE" << endl;
							cout <<" Enter 4 for DEFAULTVALUE" << endl;
							cout <<" Enter 5 for ACTUALVALUE" << endl;
							cout <<" Enter 6 for PDOMAPPING" << endl;
							cout <<" Enter 7 for LOWLIMIT" << endl;
							cout <<" Enter 8 for HIGHLIMIT" << endl;
							
							cin >> intAttributeType;
							//cout<<"Enter the SubIndexID:"<<endl;
							//cin >> SubIndexID;
							switch(intAttributeType)
							{
								case 0:
										AttributeType = NAME;
										break;
								case 1:
										AttributeType = OBJECTTYPE;
										break;
								case 2:
										AttributeType = DATATYPE;
										break;		
								case 3:
										AttributeType = ACCESSTYPE;
										break;
								case 4:
										AttributeType = DEFAULTVALUE;
										break;
								case 5:
										AttributeType = ACTUALVALUE;
										break;
								case 6:
										AttributeType = PDOMAPPING;
										break;
								case 7:
										AttributeType = LOWLIMIT;
										break;
								case 8:
										AttributeType = HIGHLIMIT;
										break;
								default:
										cout << "invalid Attribute Type" << endl;
										return 1;
							}							
							if (NodeType == 0)
							{
								char AttributeValue[20] = {0};
								GetIndexAttributes(NodeID, MN, IndexID, AttributeType, AttributeValue);
								if(AttributeValue == NULL)
									cout << "Value is NULL" << endl;
								else
									cout << "\n\n\n\n\nAttributeValue:" << AttributeValue << endl;
							}
							else if (NodeType == 1)
							{
								char AttributeValue[20] = {0};
								GetIndexAttributes(NodeID, CN, IndexID, AttributeType, AttributeValue);
								if(AttributeValue == NULL)
									cout << "Value is NULL" << endl;
								else
									cout << "\n\n\n\n\nAttributeValue:" << AttributeValue << endl;
							}
							cin.get();
							cin.get();
							break;
							
				case 21:	
							system("clear");							
							cout<<"Enter the NodeID:"<<endl;								
							cin >> NodeID;												
							cout<<"Enter the Node Type(MN=0/CN=1)"<<endl;								
							cin>>NodeType;
							cout<<"Enter the IndexID:"<<endl;
							cin >> IndexID;
							cout<<"Enter the SubIndexID:"<<endl;
							cin >> SubIndexID;
							cout<<"Enter the AttributeType: [NAME=0, OBJECTTYPE=1, DATATYPE=2, ACCESSTYPE=3, \n\
							DEFAULTVALUE=4, ACTUALVALUE=5, PDOMAPPING=6, LOWLIMIT=7, HIGHLIMIT=8]"<<endl;
							cin >> intAttributeType;

							switch(intAttributeType)
							{
								case 0:
										AttributeType = NAME;
										break;
								case 1:
										AttributeType = OBJECTTYPE;
										break;
								case 2:
										AttributeType = DATATYPE;
										break;		
								case 3:
										AttributeType = ACCESSTYPE;
										break;
								case 4:
										AttributeType = DEFAULTVALUE;
										break;
								case 5:
										AttributeType = ACTUALVALUE;
										break;
								case 6:
										AttributeType = PDOMAPPING;
										break;
								case 7:
										AttributeType = LOWLIMIT;
										break;
								case 8:
										AttributeType = HIGHLIMIT;
										break;
								default:
										cout << "invalid Attribute Type" << endl;
										return 1;
							}
							
							if (NodeType == 0)
								{
									char AttributeValue[20] = {0};
									GetSubIndexAttributes(NodeID, MN, IndexID, SubIndexID, AttributeType, AttributeValue);
									if(AttributeValue == NULL)
										cout << "Value is NULL" << endl;
									else
										cout << "\n\n\n\n\nAttributeValue:" << AttributeValue << endl;
								}
							else if (NodeType == 1)
								{
									char AttributeValue[20] = {0};
									GetSubIndexAttributes(NodeID, CN, IndexID, SubIndexID, AttributeType, AttributeValue);
									if(AttributeValue == NULL)
										cout << "Value is NULL" << endl;
									else
										cout << "\n\n\n\n\nAttributeValue:" << AttributeValue << endl;
								}
								cin.get();
							cin.get();
							break;
				case 22:	
							cout<<"Enter the NodeID: [240]"<<endl;
							cin >> NodeID;
							cout<<"Enter the Node Type(MN=0/CN=1)"<<endl;								
							cin>>NodeType;
							cout<<"Enter the IndexID:"<<endl;
							cin >> IndexID;

							int tmp_NodeCount;							
							GetNodeCount(NodeID, &tmp_NodeCount);
							cout << "\tNodeCount:" << tmp_NodeCount << endl;
							if(NodeType == 0)
							{
								int tmp_IndexCount;
								int tmp_SubIndexCount;
								GetIndexCount(NodeID, MN, &tmp_IndexCount);
								GetSubIndexCount(NodeID, MN, IndexID, &tmp_SubIndexCount);
								cout << "\tIndexCount:" << tmp_IndexCount << endl;
								cout << "\tSubIndexCount:" << tmp_SubIndexCount << endl;
							}
							else if(NodeType == 1)
							{
								int tmp_IndexCount;
								int tmp_SubIndexCount;
								GetIndexCount(NodeID, CN, &tmp_IndexCount);
								GetSubIndexCount(NodeID, CN, IndexID, &tmp_SubIndexCount);
								cout << "\tIndexCount:" << tmp_IndexCount << endl;
								cout << "\tSubIndexCount:" << tmp_SubIndexCount << endl;
							}
																		
							break;
				case 23:
							cout<<"Enter the Node Position:"<<endl;
							cin >> NodePos;
							ENodeType tmp_NodeType;
							//ENodeType tmp_NodeType;
							int tmp_NodeID;
							ocfmRetCode ErrStruct;
							ErrStruct = GetNodeIDbyNodePos(NodePos, &tmp_NodeType, &tmp_NodeID);
							if(ErrStruct.code == 0)
							{
								cout << "tmp_NodeType:" << tmp_NodeType << endl;
								cout << "tmp_NodeID:" << tmp_NodeID << endl;
							}
							else
								cout << "\n\nErrCode:" << ErrStruct.code << endl;
							break;
				case 24:
							cout<<"Enter the NodeID:"<<endl;
							cin >> NodeID;
							cout<<"Enter the NodePos:"<<endl;
							cin >> NodePos;
							cout<<"Enter the Node Type(MN=0/CN=1)"<<endl;								
							cin>>NodeType;
							cout<<"Enter the IndexPos:"<<endl;
							cin >> IndexPos;
							//cin >> IndexID;
							//cout<<"Enter the SubIndex Pos:"<<endl;
							//cin >> SubIndexPos;
							if(NodeType == 1)
							{
								char tmp_IndexID[20] = {0};
								char tmp_SubIndexID[20] = {0};
								GetIndexIDbyIndexPos(NodeID, CN, IndexPos, tmp_IndexID);
								//GetIndexIDbyIndexPos(NodePos, IndexPos, tmp_IndexID);
								cout << "tmp_IndexID:" << tmp_IndexID << endl;
								//GetSubIndexIDbySubIndexPos(NodeID, CN, IndexID, SubIndexPos, tmp_SubIndexID);
							}
							else if(NodeType == 0)
							{
								char tmp_IndexID[20] = {0};
								char tmp_SubIndexID[20] = {0};
								GetIndexIDbyIndexPos(NodeID, MN, IndexPos, tmp_IndexID);
								//GetSubIndexIDbySubIndexPos(NodeID, MN, IndexID, SubIndexPos, tmp_SubIndexID);
							}
							break;
				case 25:
							DeleteNodeObjDict(240, MN);
							break;
				case 26:
							cout<<"Enter the NodePos:"<<endl;
							cin >> NodePos;
							cout<<"Enter the IndexPos:"<<endl;
							cin >> IndexPos;
							cout<<"Enter the SubIndexPos:"<<endl;
							cin >> SubIndexPos;							
							cout<<"Enter the AttributeType: [NAME=0, OBJECTTYPE=1, DATATYPE=2, ACCESSTYPE=3, \n\
							DEFAULTVALUE=4, ACTUALVALUE=5, PDOMAPPING=6, LOWLIMIT=7, HIGHLIMIT=8]"<<endl;
							cin >> intAttributeType;

							switch(intAttributeType)
							{
								case 0:
										AttributeType = NAME;
										break;
								case 1:
										AttributeType = OBJECTTYPE;
										break;
								case 2:
										AttributeType = DATATYPE;
										break;		
								case 3:
										AttributeType = ACCESSTYPE;
										break;
								case 4:
										AttributeType = DEFAULTVALUE;
										break;
								case 5:
										AttributeType = ACTUALVALUE;
										break;
								case 6:
										AttributeType = PDOMAPPING;
										break;
								case 7:
										AttributeType = LOWLIMIT;
										break;
								case 8:
										AttributeType = HIGHLIMIT;
										break;
								default:
										cout << "invalid Attribute Type" << endl;
										break;
							}
							
							NodeType = 1;
							if(NodeType == 1)
							{
								char tmp_AttributeValue[20] = {0};
								//GetIndexAttributesbyPositions(NodePos, IndexPos, AttributeType, tmp_AttributeValue);
								GetSubIndexAttributesbyPositions(NodePos, IndexPos, SubIndexPos, AttributeType, tmp_AttributeValue);
							}
							break;
				default :
							break;
		}
	
	};
}
