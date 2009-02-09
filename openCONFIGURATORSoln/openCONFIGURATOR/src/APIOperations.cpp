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

#include "../Include/openCONFIGURATOR.h"
#include "../Include/Internal.h"
#include "../Include/Exception.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <libxml/xmlreader.h>
#include <errno.h>
#include <string.h>
#include <libxml/xmlwriter.h>
#include <libxml/encoding.h>

#define MY_ENCODING "ISO-8859-1"

int lastVarIndex = -1;
bool CDTCompleted = false;

/**************************************************************************************************
	* Function Name: AddIndexAttributes
    * Description: Adds the default attributes to the Index, when addded.
/****************************************************************************************************/
static void AddIndexAttributes(char* IndexID, CIndex* objIndex)
	{
			
				// Setting the Index Value
				objIndex->setIndexValue(IndexID);
				//$S The actual value for all the attributes has to come from ObjDict.xdd
				objIndex->setName("Test_Name");
				char* value = "Test_Values";
				objIndex->setObjectType(atoi((const char*)value));
				objIndex->setLowLimit((char*)value);
				objIndex->setHighLimit((char*)value);
				objIndex->setAccessType((char*)value);
				//objIndex->setPDOMapping((char*)value);				
				objIndex->setDefaultValue((char*)value);
				objIndex->setActualValue((char*)value);
				objIndex->setDataType((char*)value);
	}

/**************************************************************************************************
	* Function Name: AddSubIndexAttributes
    * Description: Adds the default attributes to the Index, when addded.
/****************************************************************************************************/
static void AddSubIndexAttributes(char* SubIndexID, CSubIndex* objSubIndex)
	{
			
				// Setting the Index Value
				objSubIndex->setIndexValue(SubIndexID);
				//$S The actual value for all the attributes has to come from ObjDict.xdd
				objSubIndex->setName("Test_Name");
				char* value = "Test_Values";
				objSubIndex->setObjectType(atoi((const char*)value));
				objSubIndex->setLowLimit((char*)value);
				objSubIndex->setHighLimit((char*)value);
				objSubIndex->setAccessType((char*)value);
				//objIndex->setPDOMapping((char*)value);				
				objSubIndex->setDefaultValue((char*)value);
				//objIndex->setActualValue((char*)value);
				objSubIndex->setDataType((char*)value);
	}

 
/**************************************************************************************************
	* Function Name: CreateNode
    * Description:
/****************************************************************************************************/
ocfmRetCode CreateNode(int NodeID, ENodeType NodeType)
	{
		ocfmRetCode ErrStruct;
		CNode objNode;
		CNodeCollection *objNodeCollection;
		int NodePos;
		
		// TODO:If Check is made when Zero nodes present, Seg Fault is happening
		if(NodeType == CN)
		{
			ErrStruct = IfNodeExists(NodeID, NodeType, &NodePos);
								
			if(ErrStruct.code == OCFM_ERR_SUCCESS)								
			{
				cout << "OCFM_ERR_NODE_ALREADY_EXISTS" << OCFM_ERR_NODE_ALREADY_EXISTS << endl;
				ErrStruct.code = OCFM_ERR_NODE_ALREADY_EXISTS;
				return ErrStruct;
			}
			else
			{
				cout<< "OCFM_ERR_NODEID_NOT_FOUND" << OCFM_ERR_NODEID_NOT_FOUND << endl;;
			}
		}
		objNode.setNodeId(NodeID);
		objNode.setNodeType(NodeType);
		
		objNode.CreateIndexCollection();
		objNode.CreateDataTypeCollection();
		objNode.CreateApplicationProcess();

		objNodeCollection = CNodeCollection::getNodeColObjectPointer();
		objNodeCollection->addNode(objNode);
		ErrStruct.code = OCFM_ERR_SUCCESS;
		return ErrStruct;
	}

/**************************************************************************************************
	* Function Name: DeleteNode
    * Description:
	* Return value: ocfmRetCode
/****************************************************************************************************/
ocfmRetCode DeleteNode(int NodeID, ENodeType NodeType)
	{
		int NodePos;
		ocfmRetCode ErrStruct;
		ErrStruct = IfNodeExists(NodeID, NodeType, &NodePos);
							
		if(ErrStruct.code == OCFM_ERR_SUCCESS)								
		{
			//NodePos = ErrStruct.returnValue;
		}
		else
		{
			cout << "\n\nErrStruct.code:" << ErrStruct.code << "\n\n!!!" << endl;
			// Node Doesn't Exist
			return ErrStruct;
		}
		int count;
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		objNodeCollection= CNodeCollection::getNodeColObjectPointer();	
		cout<< "Inside DeleteNode: \n" <<objNodeCollection->getNumberOfNodes()<<endl;
		objNodeCollection = CNodeCollection::getNodeColObjectPointer();
		objNodeCollection->deleteNode(NodePos);
		ErrStruct.code = OCFM_ERR_SUCCESS;
		return ErrStruct;		
	}
	
/**************************************************************************************************
	* Function Name: DeleteIndex
    * Description: Deletes the Index after performing check for Index Existanse
	* Return value: ocfmRetCode
/****************************************************************************************************/
ocfmRetCode DeleteIndex(int NodeID, ENodeType NodeType, char* IndexID)
	{
		
		int IndexPos;
		
		ocfmRetCode ErrStruct;
		ErrStruct = IfIndexExists(NodeID, NodeType, IndexID, &IndexPos);
		if(ErrStruct.code == OCFM_ERR_SUCCESS)
		{
			//IndexPos = ErrStruct.returnValue;
		}
		else
		{
			 //OCFM_ERR_INDEXID_NOT_FOUND;
			 return ErrStruct;
		}
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		CIndex objIndex;

		objIndex.setNodeID(objNode.getNodeId());
		objNodeCollection= CNodeCollection::getNodeColObjectPointer();
		objNode = objNodeCollection->getNode(NodeType, NodeID);

		objIndexCollection = objNode.getIndexCollection();
		//cout<< "Inside DeleteIndex: \n" << atoi(IndexID) <<endl;
		cout<< "Inside DeleteIndex: \n" << IndexPos <<endl;
		objIndexCollection->deleteIndex(IndexPos);
		ErrStruct.code = OCFM_ERR_SUCCESS;
		return ErrStruct;

	}
	
/**************************************************************************************************
	* Function Name: DeleteSubIndex
    * Description: Deletes the Index after performing check for Index Existanse
	* Return value: ocfmRetCode
/****************************************************************************************************/
ocfmRetCode DeleteSubIndex(int NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID)
	{
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		CIndex objIndex;
		CIndex* objSubIndex;
		int SubIndexPos;
		int IndexPos;
		ocfmRetCode ErrStruct;
		
		ErrStruct = IfSubIndexExists(NodeID, NodeType, IndexID, SubIndexID, &SubIndexPos, &IndexPos);
		if(ErrStruct.code == OCFM_ERR_SUCCESS)
		{
			//SubIndexPos = Tmp_ErrStruct.returnValue;
		}
		else
		{
			//OCFM_ERR_INVALID_SUBINDEXID;
			return ErrStruct;
		}

		objIndex.setNodeID(objNode.getNodeId());
		objNodeCollection= CNodeCollection::getNodeColObjectPointer();
		objNode = objNodeCollection->getNode(NodeType, NodeID);


		objIndexCollection = objNode.getIndexCollection();
		objSubIndex =objIndexCollection->getIndex(IndexPos);
		
		cout << "NumberofSubIndexes:" << objIndex.getNumberofSubIndexes()<< endl;
		objSubIndex->deleteSubIndex(SubIndexPos);
		ErrStruct.code = OCFM_ERR_SUCCESS;
		return ErrStruct;

	}
/**************************************************************************************************
	* Function Name: AddSubIndex
    * Description:
	* Return value: ocfmRetCode
/****************************************************************************************************/
ocfmRetCode AddSubIndex(int NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID)
	{
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		CIndex objIndex;
		int SubIndexPos;
		int IndexPos;
		ocfmRetCode ErrStruct;
				
		ErrStruct = IfSubIndexExists(NodeID, NodeType, IndexID, SubIndexID, &SubIndexPos, &IndexPos);
		if( (ErrStruct.code == OCFM_ERR_NO_SUBINDEXS_FOUND) || (ErrStruct.code == OCFM_ERR_SUBINDEXID_NOT_FOUND))
		{
			objIndex.setNodeID(objNode.getNodeId());
			objNodeCollection= CNodeCollection::getNodeColObjectPointer();
			objNode = objNodeCollection->getNode(NodeType, NodeID);
			objIndexCollection = objNode.getIndexCollection();
			CIndex* objIndexPtr;
			objIndexPtr =objIndexCollection->getIndex(IndexPos);
			CSubIndex objSubIndex;
			
			//Set the NodeID
			objSubIndex.setNodeID(objNode.getNodeId());
			AddSubIndexAttributes(SubIndexID, &objSubIndex);									
			objIndexPtr->addSubIndex(objSubIndex);
			printf("Added SubIndex \n\n");
			ErrStruct.code = OCFM_ERR_SUCCESS;
			return ErrStruct;						
		}
		return ErrStruct;
	}	

/**************************************************************************************************
	* Function Name: AddIndex
    * Description:
	* Return value: ocfmRetCode
/****************************************************************************************************/
ocfmRetCode AddIndex(int NodeID, ENodeType NodeType, char* IndexID)
	{
		int IndexPos = 0;
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		CIndex objIndex;
		ocfmRetCode ErrStruct;
		ErrStruct.code = OCFM_ERR_UNKNOWN;
		ErrStruct.errorString = NULL;
		
		cout<< "Inside AddIndex \n"<<endl;
		ErrStruct = IfIndexExists(NodeID, NodeType, IndexID, &IndexPos);
		if( (ErrStruct.code == OCFM_ERR_NO_INDEX_FOUND) || (ErrStruct.code == OCFM_ERR_INDEXID_NOT_FOUND) )
		{				
			objIndex.setNodeID(objNode.getNodeId());
			objNodeCollection= CNodeCollection::getNodeColObjectPointer();
			objNode = objNodeCollection->getNode(NodeType, NodeID);
			objIndexCollection = objNode.getIndexCollection();
			AddIndexAttributes(IndexID, &objIndex);
			objIndexCollection->addIndex(objIndex);	
			printf("Added Index \n\n");
			//return OCFM_ERR_SUCCESS;
			ErrStruct.code = OCFM_ERR_SUCCESS;
			return ErrStruct;
		}	
		
		return ErrStruct;

	}	
/**************************************************************************************************
	* Function Name: EditIndex
    * Description:
	* Return value: ocfmRetCode
/****************************************************************************************************/
ocfmRetCode SetIndexAttributes(int NodeID, ENodeType NodeType, char* IndexID, char* IndexValue, char* IndexName)
{
	int IndexPos;
	ocfmRetCode ErrStruct;		
	ErrStruct = IfIndexExists(NodeID, NodeType, IndexID, &IndexPos);
	if(ErrStruct.code == OCFM_ERR_SUCCESS)
	{
		//IndexPos = ErrStruct.returnValue;
	}
	else
	{
		//OCFM_ERR_INDEXID_NOT_FOUND
		return ErrStruct;
	}
	CIndex* objIndexPtr;

	CNode objNode;		
	CNodeCollection *objNodeCollection;
	CIndexCollection *objIndexCollection;
	CIndex objIndex;
	
	objIndex.setNodeID(objNode.getNodeId());
	objNodeCollection= CNodeCollection::getNodeColObjectPointer();
	objNode = objNodeCollection->getNode(NodeType, NodeID);
	objIndexCollection = objNode.getIndexCollection();
	objIndexPtr =objIndexCollection->getIndex(IndexPos);			
	//cout << "EditIndexValue:Index Actual Value:" << objIndexPtr->getActualValue() << IndexValue << endl;
	objIndexPtr->setActualValue(IndexValue);
	objIndexPtr->setName(IndexName);
	//printf("EditIndexValue:Index Actual Value:%s-%s\n", objIndexPtr->getActualValue(), IndexValue);
	ErrStruct.code = OCFM_ERR_SUCCESS;
	return ErrStruct;
	
}

/**************************************************************************************************
	* Function Name: EditSubIndex
    * Description:
	* Return value: ocfmRetCode
/****************************************************************************************************/
ocfmRetCode SetSubIndexAttributes(int NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID, char* IndexValue, char* IndexName)
{
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		CIndex objIndex;
		CIndex* objSubIndex;
		int IndexPos;
		int SubIndexPos;
		ocfmRetCode ErrStruct;
		//CSubIndex* objSubIndex;
		
		ErrStruct = IfSubIndexExists(NodeID, NodeType, IndexID, SubIndexID, &SubIndexPos, &IndexPos);
		if(ErrStruct.code == OCFM_ERR_SUCCESS)
		{
			//SubIndexPos = Tmp_ErrStruct.returnValue;
		}
		else
		{
			//OCFM_ERR_INVALID_SUBINDEXID
			return ErrStruct;
		}
		
		CSubIndex* objSubIndexPtr;
			
		objIndex.setNodeID(objNode.getNodeId());
		objNodeCollection= CNodeCollection::getNodeColObjectPointer();
		objNode = objNodeCollection->getNode(NodeType, NodeID);

		objIndexCollection = objNode.getIndexCollection();
		objSubIndex =objIndexCollection->getIndex(IndexPos);
		
		objSubIndexPtr = objSubIndex->getSubIndex(SubIndexPos);						
		printf("SubIndexValue:%s-%s\n", objSubIndexPtr->getName(), SubIndexID);
		objSubIndexPtr->setActualValue(IndexValue);
		objSubIndexPtr->setName(IndexName);
		printf("SubIndexValue:%s-%s\n", objSubIndexPtr->getName(), SubIndexID);
		ErrStruct.code = OCFM_ERR_SUCCESS;
		return ErrStruct;
}

/**************************************************************************************************
	* Function Name: DisplayNodeTree
    * Description:
/****************************************************************************************************/
void DisplayNodeTree()
	{
		int count;
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		
		objNodeCollection= CNodeCollection::getNodeColObjectPointer();
		objIndexCollection = objNode.getIndexCollection();
		
		if( objNodeCollection->getNumberOfNodes() > 0)
		{
			for(int count = 0; count < objNodeCollection->getNumberOfNodes(); count++)
			{
					objNode = objNodeCollection->getNodebyCollectionIndex(count);
					if (objNode.getNodeType() == CN)
					{						
						//int tmp_NodeID = objNode.getNodeId();
						//objNode =objNodeCollection->getNode(CN,tmp_NodeID);
						printf("NodePos:%d, NodeID:%d\n", count, objNode.getNodeId());
					}						
			}
		}
		else
		{
				printf("No Nodes found!\n");
				return;
		}
			
	}	
	
/**************************************************************************************************
	* Function Name: GetIndexData
   * Description: 
/****************************************************************************************************/
void GetIndexData(CIndex* objIndex, char* Buffer)
	{
			int len;
			
			//Get the Index Value		
			
			/*if(objIndex->getNumberofSubIndexes()==0 &&(objIndex->getDefaultValue()!= NULL ||
																																													 objIndex->getActualValue()!=NULL))*/
			strcpy(Buffer,"");
			if(objIndex->getNumberofSubIndexes()==0 && objIndex->getActualValue()!=NULL)
			
			{
					//Buffer = (char*)malloc(sizeof(objIndex->getIndexValue()+1));
					strcpy(Buffer,objIndex->getIndexValue());

					//Place a tab			
					len = strlen(Buffer);
					//Buffer =(char*)realloc(Buffer,2);
					strcat(Buffer,"\t");
					printf("%s",Buffer);

					//If subindexes are none, add "00"
					//	Buffer =(char*)realloc(Buffer,4);
						strcat(Buffer,"00");
						strcat(Buffer,"\t");

							//Add datatype
							DataType dt;		
							//Buffer =(char*)realloc(Buffer,6);	
							dt = objIndex->getDataType();
							int padLength=0;
							if(dt.Name != NULL)
							{
								strcat(Buffer ,dt.DataSize);				
								padLength = hex2int(dt.DataSize)*2;
							}
							else strcat(Buffer,"0000");
							
							strcat(Buffer,"\t");
						
							if (objIndex->getActualValue()!=NULL)
							{									
								char* actvalue;
								actvalue = (char*)malloc(50);
								actvalue = strchr((char*)objIndex->getActualValue(),'x');
								if(actvalue!=NULL)
								{
									actvalue = subString(actvalue,1,strlen(actvalue)-1);
									strcat(Buffer,padLeft(actvalue,'0',padLength));
								}
								else	strcat(Buffer,padLeft((char*)objIndex->getActualValue(),'0',padLength));								
							}
					
							//else 
							//{
							//	//Buffer =(char*)realloc(Buffer,sizeof(objIndex->getDefaultValue()+1));									
							//	strcat(Buffer,objIndex->getDefaultValue());
							//	//Buffer =(char*)realloc(Buffer,2);				
							//
							//}
								strcat(Buffer,"\n");
				}	
			
			else
			{
				int noOfSubIndexes = objIndex->getNumberofSubIndexes();
				bool Indexadded  = false;				
				bool resetValueAdded = false;
				int i;
				for(i=0; i<noOfSubIndexes ; i++)
				{
					CSubIndex* objSubIndex;
					objSubIndex = objIndex->getSubIndex(i);
				
					/*if(strcmp(objSubIndex->getIndexValue(),"00")!=0 && objSubIndex->getActualValue() != NULL)*/
					if(objSubIndex->getActualValue() != NULL)
			
					{
						if (Indexadded)
						strcat(Buffer,objIndex->getIndexValue());
						else
						{
							strcpy(Buffer,objIndex->getIndexValue());
							Indexadded = true;
						}
											
						//Place a tab			
						len = strlen(Buffer);
						strcat(Buffer,"\t");
						printf("%s",Buffer);

							/*if(objSubIndex->getActualValue() != NULL || objSubIndex->getDefaultValue()!=NULL)*/
								strcat(Buffer, objSubIndex->getIndexValue());
								strcat(Buffer,"\t");
									//Add datatype
								DataType dt;			
								dt = objSubIndex->getDataType();
								int padLength=0;
								if(dt.Name != NULL)
								{
									strcat(Buffer ,dt.DataSize);				
									padLength = hex2int(dt.DataSize)*2;
								}
								else strcat(Buffer,"0000");
								
								strcat(Buffer,"\t");
							
								if (objSubIndex->getActualValue()!=NULL)
								{
										char* actvalue;
										actvalue = (char*)malloc(50);
								
								// Add the reset value for that Index,SubIndex
									if(strcmp(objSubIndex->getIndexValue(),"00")==0 && CheckIfMappingPDO((char*)objIndex->getIndexValue()) &&
												resetValueAdded==false )
									{
										strcpy(actvalue,"0");
										strcat(Buffer,padLeft(actvalue,'0',padLength));
										resetValueAdded = true;
										i--;
									}
									else
									{
											actvalue = strchr((char*)objSubIndex->getActualValue(),'x');
										if(actvalue!=NULL)
										{
											actvalue = subString(actvalue,1,strlen(actvalue)-1);
											strcat(Buffer,padLeft(actvalue,'0',padLength));
										}
											else	strcat(Buffer,padLeft((char*)objSubIndex->getActualValue(),'0',padLength));
										}
								}
									/*else strcat(Buffer,objSubIndex->getDefaultValue());*/
								strcat(Buffer,"\n");
						}
					}
						
				}
	}
	
		
			
/**************************************************************************************************
	* Function Name: GenerateCNOBD
   * Description: 
/****************************************************************************************************/
char* GenerateCNOBD(CNodeCollection* objNodeCol)
	{
		CNode objNode;
		CIndexCollection* objIndexCollection;
		char* Buffer2;

		Buffer2 =  new char;
		for(int count=0; count<objNodeCol->getNumberOfNodes(); count++)
			{
				objNode = objNodeCol->getNode(CN,count+1);
				objIndexCollection = objNode.getIndexCollection();
				char* comment= new char;
				char* c;
				c= new char;
				itoa(count+1,c,10);
				strcpy(comment,"\\\\Configuration Data for CN-");
				comment = strcat(comment,c);
				comment = strcat(comment,"\n");
				char* NodeId;
				//comment = strcat(comment,(char*)count);
				/*strcpy(Buffer2, comment);*/
				int NumberOfIndexes = objIndexCollection->getNumberofIndexes();
				for(int i=0; i<NumberOfIndexes; i++)
					{
						CIndex* objIndex;
						objIndex = objIndexCollection->getIndex(i);
						const char* IndexValue = objIndex->getIndexValue();
						if(strcmp(IndexValue,"1006")==0 ||strcmp(IndexValue,"2000")==0)
							{
								//strcpy(Buffer2,GetIndexData(objIndex));
								GetIndexData(objIndex, Buffer2);
								
							}
					}				
						
			}
		return Buffer2;
	}
	/**************************************************************************************************
	* Function Name: WriteCNsData
 * Description: Writes all CNS data to CDC file
/****************************************************************************************************/

	void WriteCNsData(char* fileName)
{
		char* Buffer2;
		char* Buffer3;
		int len;
		CIndexCollection* objIndexCollection;
	

		//Buffer2 = NULL;
//		strcpy(Buffer3,"");
		char* c = (char*)malloc(4);	
		FILE* fileptr = new FILE();
		CNode objNode;	
		CNodeCollection* objNodeCollection;
		objNodeCollection = CNodeCollection::getNodeColObjectPointer();		
		
		int CNCount=0;
		if (( fileptr = fopen(fileName,"a+")) == NULL)
		{
			cout << "Problem" <<endl;
		}
		for(int count=0; count<objNodeCollection->getNumberOfNodes(); count++)
			{
				objNode = objNodeCollection->getNodebyCollectionIndex(count);
				if (objNode.getNodeType() == CN)
					{
						
							objIndexCollection = objNode.getIndexCollection();
							char* comment= (char*)malloc(30);
							itoa(CNCount+1,c,10);
							CNCount++;
							
							strcpy(comment,"\\\\Configuration Data for CN-");
							comment = strcat(comment,c);
							comment = strcat(comment,"\n");
							len = strlen(comment);
							if((fwrite(comment, sizeof(char),len,fileptr))!=NULL)
							{
								printf("\nComments for CN-%d",count);
								fclose(fileptr);
							}
							char* NodeId;
							//comment = strcat(comment,(char*)count);
							/*strcpy(Buffer2, comment);*/
							int NumberOfIndexes;
							bool firstBuffer = true;																								
							NumberOfIndexes = objIndexCollection->getNumberofIndexes();
							
							CIndex* objIndex;
							char* Buffer4;
									
						/*************WRITE MN'S 1006,1020 Indexes Values *******************************/			
							Buffer4 = (char*)malloc(300);	
							objIndex = getMNIndexValues("1006");
							Buffer2 = (char*)malloc(sizeof(char)*strlen(Buffer4)+1);				
							GetIndexData(objIndex,Buffer4);
							strcpy(Buffer2, Buffer4);
							
							
							objIndex = getMNIndexValues("1020");
							if(objIndex!=NULL)
							{
								GetIndexData(objIndex,Buffer4);
								strcat(Buffer2, Buffer4);
							}
									
							/*************WRITE Other Required CN Indexes in CDC *******************************/
								for(int i=0; i<NumberOfIndexes; i++)
								{
									
								
									
								/*	if(firstBuffer)
									{
										Buffer2 = (char*)malloc(sizeof(char)*strlen(Buffer4)+1);																				
										strcpy(Buffer2, Buffer4);
										firstBuffer = false;											
									}
									else strcat(Buffer2, Buffer4);*/
									
									objIndex = objIndexCollection->getIndex(i);
									const char* IndexValue = objIndex->getIndexValue();
									
									/*if((CheckIfNotPDO((char*)IndexValue)==false)  || CheckIfManufactureSpecificObject((char*)IndexValue))
								*/
								if(CheckAllowedCNIndexes((char*)IndexValue))
								{
																								
											GetIndexData(objIndex, Buffer4);
											strcat(Buffer2, Buffer4);
											/*if(firstBuffer)
											{
												Buffer2 = (char*)malloc(sizeof(char)*strlen(Buffer4)+1);																				
												strcpy(Buffer2, Buffer4);
												firstBuffer = false;											
											}
											else
											{
												printf("length of Buffer1 %d",strlen(Buffer4));
												printf("\nlength of Buffer2 %d",strlen(Buffer2));
												strcat(Buffer2, Buffer4);
											}*/
											//free(Buffer1);
											//strcat(Buffer2, Buffer4);
										}									
								}	
						/**************write module data********************************/
					/*	if(objNode.ProcessImageCollection.Count() !=0)
						{
							int count = objNode.ProcessImageCollection.Count();
							
							for(int i=0; i<count; i++)
							{
										char* Index = (char*)malloc(4);	
										CIndex* objIndex;										
										strcpy(Index,objNode.ProcessImageCollection[i].Index);
										objIndex = objIndexCollection->getIndexbyIndexValue(Index);
										
							}
						}*/
							
							//Convert CN NodeID to Hex
							itoa(count+1,c,16);	
							char CNDataLength[18];
							strcpy(CNDataLength, "1F22\t");
							c = padLeft(c, '0', 2);
							/*if (strlen(c)< 2)
								{
									char clen[2];
									strcpy(clen,c);
									strcpy(c,"0");
									strcat(c,clen);
								}
							*///write CN-n NodeID  in the next to 1F22
							strcat(CNDataLength, c);
							strcat(CNDataLength, "\t");
							
							//write the size of CN-n Buffer
							int len = lenOfCNBuffer(Buffer2);

							//Convert length to Hex
							itoa(len,c,16);
							printf("c%s",c);
							
							c = padLeft(c, '0', 4);
							strcat(CNDataLength, c);				
							
							// First write the IF22 data in a Buffer and then CN-ns Object Dictionary
							Buffer3 = (char*)malloc(strlen(Buffer2)+50);
							strcpy(Buffer3, CNDataLength);
							strcat(Buffer3, "\n");
							strcat(Buffer3, Buffer2);
															
						
							//strcpy(Buffer2,GenerateCNOBD(objNodeCollection));
						
							//write all CNs data in the file
							len = strlen(Buffer3);
							if (( fileptr = fopen(fileName,"a+")) == NULL)
							{
								cout << "Problem" <<endl;
							}
							if((fwrite(Buffer3, sizeof(char),len,fileptr))!=NULL)
								{
									printf("success");				
									fclose(fileptr);	
								}
					}
			}
	}
/**************************************************************************************************
	* Function Name: GenerateCDC
    * Description: Generates the CDC file
/****************************************************************************************************/
void GenerateCDC(char* fileName)
	{
		CNode objNode;	
		CIndexCollection* objIndexCollection;
		//ofstream file;
		char *Buffer1;
		int len;
		const char tempFileName[9] = "temp.txt";
		char* CNData;

		printf("Inside GenerateCDC");
				
		Buffer1 = NULL;		

		//get the MN Node object from the NodeCollection
		CNodeCollection* objNodeCollection;
		objNodeCollection = CNodeCollection::getNodeColObjectPointer();		
		
	/******************************* Write MN's Indexes ******************************************************************/
		objNode = objNodeCollection->getMNNode();

		if(objNode.isNull())
		{
			printf("No MN Node exists");
			exit(1);
		}
		printf("Node id %d",objNode.getNodeId());

		FILE* fileptr = new FILE();
		if (( fileptr = fopen(tempFileName,"w+")) == NULL)
			{
				printf ( "Cannot open file you have named...!\n" );
			
			}
		else
			{
			}

		objIndexCollection = objNode.getIndexCollection();
		

		//Get all the MN's Default Data in Buffer1
		for(int i=0;i < objIndexCollection->getNumberofIndexes ();i++)
			{
				CIndex* objIndex;
				objIndex = objIndexCollection->getIndex(i);
			/*	if(CheckIfNotPDO((char*)objIndex->getIndexValue()))
					{	*/					
						printf("Index%s",objIndex->getIndexValue());						
						Buffer1 = (char*)malloc(200);
						len = strlen(Buffer1);
						
						GetIndexData(objIndex,Buffer1);
						len = strlen(Buffer1);
						if((fwrite(Buffer1, sizeof(char),len,fileptr))!=NULL)
						{
							printf("Buffer1 written");
						
						}
						delete[] Buffer1;
				/*	}*/
						
			}
			fclose(fileptr);
			
		/*************************Write CN's Data in Buffer2***************************************************/
		WriteCNsData((char*)tempFileName);
		
		//Get all the IF81 ENTRY in Buffer1
		if(objNodeCollection->getNumberOfNodes()!=0)
		{
			if (( fileptr = fopen(tempFileName,"a+")) == NULL)
			{
				printf ( "Cannot open file you have named...!\n" );
			
			}
		else
			{
			}
			for(int i=0;i < objNodeCollection->getNumberOfNodes();i++)
			{
					CNode objNode;
					objNode = objNodeCollection->getNodebyCollectionIndex(i);
					if(objNode.getNodeType() ==CN)
					{
							printf("\nInside 1F81");
							Buffer1 = (char*)malloc(100);
							len = strlen(Buffer1);
							strcpy(Buffer1, "//// NodeId Reassignment\n");
							strcat(Buffer1, "1F81");
							strcat(Buffer1, "\t");
							int NodeID = objNode.getNodeId();		
							char* hex = (char*)malloc(3);					
							hex = itoa(NodeID,hex,16);
							hex = padLeft(hex,'0',2);
							strcat(Buffer1, hex);
							strcat(Buffer1, "\t0004\t80000007\n");
							len = strlen(Buffer1);
							if((fwrite(Buffer1, sizeof(char),len,fileptr))!=NULL)
							{
								fclose(fileptr);
								printf("Buffer1 written");
							
							}
							delete[] Buffer1;
					}
				}
				/*	}*/
						
			}
		
		printf("\nText cdc generated");
		// Convert CDC txt file to Binary
		ConvertCdcToBinary(fileName,(char*) tempFileName);
		
	}

/**************************************************************************************************
	* Function Name: GenerateMNOBD
   * Description: Generates the MN Object Dictionary
/****************************************************************************************************/
//void GenerateMNOBD()
//	{
//		CNodeCollection *objNodeCollection;
//		objNodeCollection= CNodeCollection::getNodeColObject();
//
//		//get the MN Node
//		CNode objMNNode;
//		
//		objMNNode=objNodeCollection->getNode(MN,249);
//
//		//get the Index collection of MN Object
//		CIndexCollection *objIndexCollection;
//		objIndexCollection = objMNNode->getIndexCollection();
//
//		//loop through the Indexes of IndexCollection
//		int count=0;
//		for (count=0;count < objIndexCollection->getNumberofIndexes(); count++)
//			{
//				CIndex objIndex;
//				objIndex = objIndexCollection->getIndex(count);
//				objIndex.getIndex();
//				
//			}
//
//
//
//
//	}
void  ProcessCDT(CComplexDataType* objCDT,CApplicationProcess* objAppProc, CNode* objNode, Parameter* para)
{

	ocfmException objex;
		
	{
		if(objCDT ==NULL)
		{
			objex.ocfm_Excpetion(OCFM_ERR_STRUCT_DATATYPE_NOT_FOUND);
			throw objex;		
		}

		for(int i=0 ; i<objCDT->varCollection.Count(); i++)
		{
	
			varDeclaration vd;
			vd.Initialize();
			vd = objCDT->varCollection[i];
			if(vd.nam_id_dt_attr->getDtUniqueRefId() != NULL)
			{
				objCDT = objAppProc->getCDTbyUniqueID(vd.nam_id_dt_attr->getDtUniqueRefId());	
				objAppProc->updatePreviousCDT_UId(vd.StructUniqueId, objCDT->Index);		
				/*objCDT->previousCDT_UId = (char*)malloc(strlen(vd.StructUniqueId)+1);
				strcpy(objCDT->previousCDT_UId, vd.StructUniqueId);*/
				lastVarIndex = i;
				printf("\n previousCDT_UId : %s",objCDT->previousCDT_UId);
				printf("\n DataRefID : %s",vd.nam_id_dt_attr->getDtUniqueRefId());
				ProcessCDT(objCDT, objAppProc, objNode, para);
			}
		if(!CDTCompleted)
		{	
			// add rest of the contents
			ProcessImage pi;
			if(para->access !=NULL)
			strcpy(pi.Direction,getParameterAccess(para->access));
				
				if(vd.size != NULL)			
				{
				/*	pi.DataSize = (char*)malloc(5);*/
					strcpy(pi.DataSize, vd.size);
				}
			else
			{
			/*		pi.DataSize = (char*)malloc(5);*/
					strcpy(pi.DataSize,"000x");
			}
				if(vd.nam_id_dt_attr->getName()!=NULL)
				{
					pi.Name = (char*)malloc(strlen(vd.nam_id_dt_attr->getName()) +4);
					strcpy(pi.Name,getPIName(objNode->getNodeId()));
					strcat(pi.Name,vd.nam_id_dt_attr->getName());
					printf("\n PI Name: %s",pi.Name);
				}
					
				if(vd.nam_id_dt_attr->getDataType()!=NULL)
				{
					pi.DataType = (char*)malloc(strlen(vd.nam_id_dt_attr->getDataType()) +1);
					strcpy(pi.DataType,(const char*)vd.nam_id_dt_attr->getDataType());
				}
		

			objNode->addProcessImage(pi);
		}
		
		}

		/*if(objCDT->VarIndex != -1)
		{
			varDeclaration vd;
			vd = objCDT->
			objCDT
		}*/
		objCDT = objAppProc->getCDTbyUniqueID(objCDT->previousCDT_UId);
		
			for(int i=(lastVarIndex + 1) ; i<objCDT->varCollection.Count(); i++)
			{
			if(!CDTCompleted)
			{
				varDeclaration vd;
				printf("\n i : %d",i);
				vd = objCDT->varCollection[i];
				printf("\nVar Count: %d",objCDT->varCollection.Count());
				if(vd.nam_id_dt_attr->getDtUniqueRefId() != NULL)
				{
					/*objCDT->previousCDT_UId = (char*)malloc(strlen(vd.StructUniqueId)+1);
					strcpy(objCDT->previousCDT_UId, vd.StructUniqueId);	*/	
					objCDT = objAppProc->getCDTbyUniqueID(vd.nam_id_dt_attr->getDtUniqueRefId());
					objAppProc->updatePreviousCDT_UId(vd.StructUniqueId, objCDT->Index);
					printf("\n previousCDT_UId : %s",objCDT->previousCDT_UId);
					printf("\n DataRefID : %s",vd.nam_id_dt_attr->getDtUniqueRefId());
			
					lastVarIndex = i;
					ProcessCDT(objCDT, objAppProc, objNode, para);
				
				}						
			}
		}
		CDTCompleted = true;
	}
	printf("exiting");
}

void DecodeUniqiueIDRef(char* uniquedIdref, CNode* objNode)
{
	ocfmException objex;
	Parameter* para;
	CApplicationProcess* objAppProc;
	ProcessImage procImage;
	CComplexDataType* objCDT;
	
	try
	{
		if(objNode->getApplicationProcess()!=NULL)
		{
			
			objAppProc = objNode->getApplicationProcess();
			if(objAppProc->ParameterCollection.Count()!=0)
			{
				
				para = objAppProc->get_Parameterby_UniqueIDRef(uniquedIdref);
				if(para == NULL)
				{
					objex.ocfm_Excpetion(OCFM_ERR_UNIQUE_ID_REF_NOT_FOUND);
					throw objex;
				}
				
				//if(para->access != NULL)
				//{
				//	
				//	//procImage.Direction = getParameterAccess(para->access);
				//	//strcpy(procImage.Direction, getParameterAccess(para->access));
				//	strcpy(procImage.Direction, "rw");
				//	procImage.ParametrIndex = para->ParaIndex;

				//}
				// Check if DataTypeUniqueIDref exists
				if(para->name_id_dt_attr.dataTypeUniqueIDRef !=NULL)
				{
							
						objCDT = objAppProc->getCDTbyUniqueID(para->name_id_dt_attr.dataTypeUniqueIDRef);
						if(objCDT ==NULL)
						{
							objex.ocfm_Excpetion(OCFM_ERR_STRUCT_DATATYPE_NOT_FOUND);
							throw objex;
						}
						ProcessCDT(objCDT, objAppProc, objNode, para); 
						lastVarIndex = -1;
						CDTCompleted = false;
				}
				
				else
				{
					
				}
			}
			
		}
		
	}
	catch(ocfmException& ex)
	{
		throw ex;
	}
}
/**************************************************************************************************
	* Function Name: ProcessPDONodes
    * Description: Processes the Node
/****************************************************************************************************/
void ProcessPDONodes(int NodeID)
{
		CNodeCollection* objNodeCol;
		ocfmException* objex = NULL;
		objNodeCol = CNodeCollection::getNodeColObjectPointer();
		CNode* objNode;
		objNode = objNodeCol->getNodePtr(CN,NodeID);
		CIndexCollection* objPDOCollection;
		CIndexCollection* objIndexCollection;
		/* Check RPDO Mapped objects*/



		objPDOCollection = objNode->getPDOIndexCollection();
		if(objPDOCollection!= NULL)
		{
		

			objIndexCollection = objNode->getIndexCollection();
			for(int count = 0; count<objPDOCollection->getNumberofIndexes(); count++)
			{
					CIndex* objIndex;
					objIndex = objPDOCollection->getIndex(count);
					if(!(CheckIfMappingPDO((char*)objIndex->getIndexValue())))
					continue;
					if(objIndex->getNumberofSubIndexes() > 0)
					{
						int sicount = 1;
						int siTotal = objIndex->getNumberofSubIndexes ();
						while(sicount< siTotal)
						{
							CSubIndex* objSI;
							objSI = objIndex->getSubIndex(sicount);
							if (objSI->getActualValue()!=NULL)
							{
									const char* value = objSI->getActualValue();
									int len = strlen(value);
									char* reverseValue = (char*)malloc(len);
									/* Reverse the actual value to get Index / subindex*/
							/*		reverseValue = reverse((char*)value);*/
									
									/* Get the Index*/
									char* strModuleIndex = (char*)malloc(5);
									strModuleIndex = subString((char*)value, len-4,4);
									strModuleIndex[5] ='\0';
							/*		strModuleIndex = strncpy(strModuleIndex,reverseValue,4);
							*/			/* Get the SubIndex*/
									char* strSubIndex = (char*)malloc(3);
									//strSubIndex = subString(reverseValue,2,2);
									strSubIndex = subString((char*)value, len-6,2);
									strSubIndex[3] ='\0';
									ProcessImage objProcessImage;
														
									/* Index of the Process Image variable*/						
									objProcessImage.Index = new char[4];
									strcpy(objProcessImage.Index, strModuleIndex);
									
										/* SubIndex of the Process Image variable*/		
									objProcessImage.subindex = new char[2];
									strcpy(objProcessImage.subindex, (const char*)strSubIndex);
									
									CIndex* objModuleIndex;
									CSubIndex* objSIndex;
																
										objModuleIndex = objIndexCollection->getIndexbyIndexValue(strModuleIndex);
										if(objModuleIndex==NULL)
										{								
											objex->ocfm_Excpetion(OCFM_ERR_MODULE_INDEX_NOT_FOUND);
											throw objex;
										}
										objSIndex = objModuleIndex->getSubIndexbyIndexValue(strSubIndex);
										if(objSIndex==NULL)
										{								
											objex->ocfm_Excpetion(OCFM_ERR_MODULE_SUBINDEX_NOT_FOUND);
											throw objex;
										}
										
									
										if(objSIndex->getUniqueIDRef()!=NULL)
										{
											DecodeUniqiueIDRef(objSIndex->getUniqueIDRef(), objNode);
											
										}
										else
										{
										
											/* Name of the Process Image variable*/
											objProcessImage.Name = (char*)malloc(strlen(objSIndex->getName())+1);
											strcpy(objProcessImage.Name, objSIndex->getName());
											/* Access of the Process Image variable*/
											strcpy(objProcessImage.Direction, objSIndex->getAccessType());
											DataType dt;
											dt = objSIndex->getDataType();
											/* Data Size in hex of the Process Image variable*/
										/*	objProcessImage.DataSize = (char*)malloc(strlen(dt.DataSize+1));*/
											strcpy(objProcessImage.DataSize, dt.DataSize);
											/* Datatype in hex of the Process Image variable*/
											objProcessImage.DataType = (char*)malloc(strlen(dt.Name+1));
											strcpy(objProcessImage.DataType, dt.Name);
											
											objNode->addProcessImage(objProcessImage);
										}
								}
								sicount++;
							}
						}
				}
			}
	}

void ConvertCdcToBinary(char* fileName,char* tempFile)
{
	FILE *fin, *fout;
	char* filePath;
	int count=0;
	int num=0;
	char ch=0;
	int iCtr;
	int iLength;
	unsigned char ca_cn1obd[10000];
	unsigned char tempCn1Obd[10000];

	fin = fopen(tempFile, "r");
	//filePath = (char*)malloc(80);
	//filePath = strchr(reverse(fileName),'\\');
	//filePath = subString(fileName, 0, strlen(fileName) - strlen(filePath) +1);
	//strcat(filePath,"config_data.cdc");
	////fout = fopen("config_data.cdc", "wb");
	fout = fopen(fileName, "wb");
	
	
		while(ch != EOF)
		{
			ch = fgetc(fin);
			if(ch == '/') 
		    {
				while(fgetc(fin) != '\n');
		    }
			else if( ch != '\t' || ch != '\n' || ch != ' ')
			{
				// Convert to Upper case
				ch = toupper(ch);
				if((ch >= 65 && ch <= 70) || (ch >= 97 && ch <=102))
				{
					ca_cn1obd[num] = ch - 55;
					num++;
				}
				else if (( ch >=48 && ch <= 57))
				{
					ca_cn1obd[num] = ch - 48;
					num++;
				}
		}
		//ca_cn1obd[num] = '\0';
		// For testing purpose.
		/*for ( count = 0; count<num ; count++)
		{
			printf("%x",ca_cn1obd[count]);

		}*/
		iLength = num;
		//printf( "\niLength = %d\n",iLength);
		//For Byte Packing
		for (iCtr = 0 , count = 0; iCtr < iLength; iCtr++, count++ )
		{
			tempCn1Obd[count] = (unsigned char)( ( ca_cn1obd[ iCtr ] << 4 ) | ca_cn1obd[ ++iCtr ] );
			//printf("0x%2x\t",tempCn1Obd[count]);
		}
		//printf("Size : %d\n", count);
		count = count -1;
		// Write to Binary file
		fwrite(&tempCn1Obd,1,count,fout);
	}
	
	fclose(fin);
	fclose(fout);	
}
 int lenOfCNBuffer(char* Buffer)
 {
		char tempchar;
		int len = strlen(Buffer);
		int counter =0;
		int actualLength=0;
		while(counter<len)
		{
			tempchar = *(Buffer+counter);
			if(tempchar!='\n' && tempchar != '\t')
			actualLength++;
			counter++;
		}
		return actualLength;

 }
CIndex* getMNIndexValues(char* Index)
{
		CNodeCollection* objNodeCollection;
		objNodeCollection = CNodeCollection::getNodeColObjectPointer();		
		CIndexCollection* objIndexCol;
		CNode objNode;
		objNode = objNodeCollection->getMNNode();
		objIndexCol = objNode.getIndexCollection();
		CIndex* objIndex = NULL;
		objIndex = objIndexCol->getIndexbyIndexValue(Index);
		return objIndex;
}

/**************************************************************************************************
	* Function Name: ImportObjDictXML
    * Description: Imports the Object Dictionary XML file. Objects from this Object dictionary file 
	will be used when Adding Index and/or Adding SubIndex
	* Return value: ocfmRetCode
/****************************************************************************************************/
//ocfmRetCode ImportObjDictXML(char* fileName)
//{
//	xmlTextReaderPtr reader;
//	int ret;
//	ocfmRetCode ErrStruct;
//	CIndex objIndex;
//
//	cout << "ImportObjDictXML:" << fileName << endl;
//	
//	CObjectDictionary* ObjDictObject;
//	ObjDictObject = CObjectDictionary::getObjDictObjectPointer();
//	//char* IndexID = "1006";
//	
//	//objIndex = ObjDictObject->CreateIndexCollection();
//	
//	//objNodeCollection= CNodeCollection::getNodeColObjectPointer();
//	//objNode = objNodeCollection->getNode(NodeType, NodeID);
//	//objIndexCollection = objNode.getIndexCollection();
//			
//	//AddIndexAttributes(IndexID, &objIndex);
//	//ObjDictObject->addIndex(objIndex);
//
//	//reader = xmlReaderForFile(fileName, NULL, 0);
//	//if (reader != NULL)
//	//{
//	//	ret = xmlTextReaderRead(reader);
//	//	while (ret == 1)
//	//	{		
//	//		processObjDict(reader);
//	//		ret = xmlTextReaderRead(reader);
//	//	}
//	//}
//	//else
//	//{
//	//	fprintf(stderr, "Unable to open %s\n", filename);
//	//}
//
//	//printf("\n\n\nCalling xmlFreeTextReader\n\n\n\n");	
//	
//}
ocfmRetCode GenerateXAP(char* fileName)
{
		CNodeCollection* objNodeCollection;
		ocfmException* objException;
		objNodeCollection = CNodeCollection::getNodeColObjectPointer();		
		try
		{
			if(objNodeCollection->getNumberOfNodes()==0)
			{
				objException->ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
				throw objException;
			}
			CNode objNode;
			xmlTextWriterPtr writer = NULL;
			xmlDocPtr doc = NULL;
			int picount = 0;
			int i=10;
			StartXAPxml(writer, doc);
			for(int count=0; count<objNodeCollection->getNumberOfNodes(); count++)
			{
				objNode = objNodeCollection->getNodebyCollectionIndex(count);
				if (objNode.getNodeType() == CN)
				{
						if(objNode.ProcessImageCollection.Count() !=0)
						{
							picount = 0;
							while(picount<objNode.ProcessImageCollection.Count())
							{
									ProcessImage pi;
									pi = objNode.ProcessImageCollection[picount];
									WriteXAPElements(pi, writer);			
									picount++;						
							}
												
						}
					}
				
			}
				EndWrtitingXAP(writer, fileName, doc);
		}
		catch(ocfmException* ex)
		{
			return *(ex->_ocfmRetCode);
		}
}
void WriteXAPElements(ProcessImage pi, xmlTextWriterPtr& writer)
{
	int rc;
	 /* Start an element named "Channel". Since thist is the first
     * element, this will be the root element of the document. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "Channel");
    if (rc < 0)
    {
        printf("testXmlwriterDoc: Error at xmlTextWriterStartElement\n");
        return;
    }



    

    /* Add an attribute with name "Name" and value  to channel. */
     printf("\n%s",pi.Name);
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "Name",      
                                   BAD_CAST pi.Name);
                                  
    if (rc < 0)
    {
        printf("testXmlwriterDoc: Error at xmlTextWriterWriteAttribute\n");
        return;
    }
    
    

    /* Add an attribute with name "direction" and value to channel */
      printf("\n%s",pi.Direction);
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "Direction",
                                     BAD_CAST pi.Direction );
    if (rc < 0)
    {
        printf("testXmlwriterDoc: Error at xmlTextWriterWriteAttribute\n");
        return;
    }

				/* Add an attribute with name "DataType" and value to channel */
				 printf("\n%s",pi.DataType);
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "dataType",
                                     BAD_CAST pi.DataType );
    if (rc < 0)
    {
        printf("testXmlwriterDoc: Error at xmlTextWriterWriteAttribute\n");
        return;
    }
    
    /* Add an attribute with name "dataSize" and value to channel */
    printf("\n%s",pi.DataSize);
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "dataSize",
                                     BAD_CAST pi.DataSize );
    if (rc < 0)
    {
        printf("testXmlwriterDoc: Error at xmlTextWriterWriteAttribute\n");
        return;
    }
			
    

    
    ///* Close the element named Channel. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0)
    {
        printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
        return;
    }

   
}
void StartXAPxml(xmlTextWriterPtr& writer,  xmlDocPtr& doc)

{
	 int rc;
		//xmlTextWriterPtr writer =NULL;


    /* Create a new XmlWriter for DOM, with no compression. */
    writer = xmlNewTextWriterDoc(&doc, 0);
    if (writer == NULL) {
        printf("testXmlwriterDoc: Error creating the xml writer\n");
        return;
    }

    /* Start the document with the xml default for the version,
     * encoding ISO 8859-1 and the default for the standalone
     * declaration. */
    rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
    if (rc < 0) {
        printf("testXmlwriterDoc: Error at xmlTextWriterStartDocument\n");
        return;
    }

			/* Write a comment as child of ORDER */
		 rc = xmlTextWriterWriteComment(writer,BAD_CAST "This file was autogenerated by openCONFIGURATOR");
    if (rc < 0)
     {
        printf
            ("testXmlwriterDoc: Error at xmlTextWriterWriteFormatComment\n");
        return;
    }
    rc = xmlTextWriterStartElement(writer, BAD_CAST "ProcessImage");
    if (rc < 0) {
        printf("testXmlwriterDoc: Error at xmlTextWriterStartElement\n");
        return;
    }
}
 
void EndWrtitingXAP( xmlTextWriterPtr& writer, char* fileName, xmlDocPtr& doc)
{ 
	int rc;
    /* Here we could close the elements ORDER and EXAMPLE using the
     * function xmlTextWriterEndElement, but since we do not want to
     * write any other elements, we simply call xmlTextWriterEndDocument,
     * which will do all the work. */
    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0) {
        printf("testXmlwriterDoc: Error at xmlTextWriterEndDocument\n");
        return;
    }

    xmlFreeTextWriter(writer);

				xmlSaveFormatFile ((const char*)fileName, doc, 1);

    xmlFreeDoc(doc);
}
/**************************************************************************************************
	* Function Name: GetIndexAttributes
    * Description:
	* Return value: ocfmRetCode
/****************************************************************************************************/

ocfmRetCode GetIndexAttributes(
	int				NodeID, 
	ENodeType		NodeType, 
	char* 			IndexID, 
	EAttributeType 	AttributeType, 
	char* 			Out_AttributeValue)
{
	int IndexPos;
	ocfmRetCode ErrStruct;
	
	ErrStruct = IfIndexExists(NodeID, NodeType, IndexID, &IndexPos);
		if(ErrStruct.code == OCFM_ERR_SUCCESS)
		{
			cout << "IndexPos:" << IndexPos << endl;
		}
		else
		{
			 //OCFM_ERR_INDEXID_NOT_FOUND;
			 return ErrStruct;
		}
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		CIndex objIndex;
		CIndex* objIndexPtr;
		
		objIndex.setNodeID(objNode.getNodeId());
		objNodeCollection= CNodeCollection::getNodeColObjectPointer();
		objNode = objNodeCollection->getNode(NodeType, NodeID);

		objIndexCollection = objNode.getIndexCollection();
		objIndexPtr = objIndexCollection->getIndex(IndexPos);
		
		switch(AttributeType)
		{
			case 0:									
					if(objIndexPtr->getName() != NULL)
						strcpy(Out_AttributeValue, (char *)objIndexPtr->getName());
					break;
			case 1:
					if(objIndexPtr->getObjectType() != NULL)
						strcpy(Out_AttributeValue, (char *)objIndexPtr->getObjectType());
					break;
			case 2:
					DataType tmpDataType;
					tmpDataType = objIndexPtr->getDataType();
					strcpy(Out_AttributeValue, tmpDataType.Name);
					break;		
			case 3:
					if(objIndexPtr->getAccessType() != NULL)
						strcpy(Out_AttributeValue, (char *)objIndexPtr->getAccessType());
					break;
			case 4:
					if(objIndexPtr->getDefaultValue() != NULL)
						strcpy(Out_AttributeValue, (char *) objIndexPtr->getDefaultValue());
					break;
			case 5:
					if(objIndexPtr->getActualValue() != NULL)
						strcpy(Out_AttributeValue, (char *) objIndexPtr->getActualValue());
					break;
			case 6:
					if(objIndexPtr->getPDOMapping() != NULL)
						strcpy(Out_AttributeValue, (char *) objIndexPtr->getPDOMapping());
					break;
			case 7:
					if(objIndexPtr->getLowLimit() != NULL)
						strcpy(Out_AttributeValue, (char *) objIndexPtr->getLowLimit());
					break;
			case 8:
					if(objIndexPtr->getHighLimit() != NULL)
					strcpy(Out_AttributeValue, (char *) objIndexPtr->getHighLimit());
					break;									
			default:
					cout << "invalid Attribute Type" << endl;
					ErrStruct.code = OCFM_ERR_INVALID_ATTRIBUTETYPE;
					return ErrStruct;
		}
		
	ErrStruct.code = OCFM_ERR_SUCCESS;
	return ErrStruct;
}
/**************************************************************************************************
	* Function Name: GetSubIndexAttributes
    * Description:
	* Return value: ocfmRetCode
/****************************************************************************************************/
ocfmRetCode GetSubIndexAttributes(
	int 			NodeID, 
	ENodeType 		NodeType, 
	char* 			IndexID, 
	char* 			SubIndexID, 
	EAttributeType 	AttributeType, 
	char* 			Out_AttributeValue)
{
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		CIndex objIndex;
		CIndex* objSubIndex;
		int SubIndexPos;
		int IndexPos;
		ocfmRetCode ErrStruct;
		
		ErrStruct = IfSubIndexExists(NodeID, NodeType, IndexID, SubIndexID, &SubIndexPos, &IndexPos);
		if(ErrStruct.code == OCFM_ERR_SUCCESS)
		{
			//SubIndexPos = Tmp_ErrStruct.returnValue;
		}
		else
		{
			//OCFM_ERR_INVALID_SUBINDEXID;
			return ErrStruct;
		}

		objIndex.setNodeID(objNode.getNodeId());
		objNodeCollection= CNodeCollection::getNodeColObjectPointer();
		objNode = objNodeCollection->getNode(NodeType, NodeID);

		objIndexCollection = objNode.getIndexCollection();
		objSubIndex =objIndexCollection->getIndex(IndexPos);
		
		cout << "NumberofSubIndexes:" << objIndex.getNumberofSubIndexes()<< endl;

		CSubIndex* objSubIndexPtr;
		objSubIndexPtr = objSubIndex->getSubIndex(SubIndexPos);	
		
		switch(AttributeType)
		{
			case 0:						
					if(objSubIndexPtr->getName() != NULL)
						strcpy(Out_AttributeValue, (char *)objSubIndexPtr->getName());
					break;
			case 1:
					if(objSubIndexPtr->getObjectType() != NULL)
						strcpy(Out_AttributeValue, (char *)objSubIndexPtr->getObjectType());
					break;
			case 2:
					DataType tmpDataType;
					tmpDataType = objSubIndexPtr->getDataType();					
					strcpy(Out_AttributeValue, tmpDataType.Name);
					break;		
			case 3:
					if(objSubIndexPtr->getAccessType() != NULL)
						strcpy(Out_AttributeValue, (char *)objSubIndexPtr->getAccessType());
					break;
			case 4:
					if(objSubIndexPtr->getDefaultValue() != NULL)
						strcpy(Out_AttributeValue, (char *) objSubIndexPtr->getDefaultValue());
					break;
			case 5:
					if(objSubIndexPtr->getActualValue() != NULL)
						strcpy(Out_AttributeValue, (char *) objSubIndexPtr->getActualValue());
					break;
			case 6:
					if(objSubIndexPtr->getPDOMapping() != NULL)
						strcpy(Out_AttributeValue, (char *) objSubIndexPtr->getPDOMapping());
					break;
			case 7:
					if(objSubIndexPtr->getLowLimit() != NULL)
						strcpy(Out_AttributeValue, (char *) objSubIndexPtr->getLowLimit());
					break;
			case 8:
					if(objSubIndexPtr->getHighLimit() != NULL)
						strcpy(Out_AttributeValue, (char *) objSubIndexPtr->getHighLimit());
					break;
			default:
					cout << "invalid Attribute Type" << endl;
					ErrStruct.code = OCFM_ERR_INVALID_ATTRIBUTETYPE;
					return ErrStruct;
		}		
		
		ErrStruct.code = OCFM_ERR_SUCCESS;
		return ErrStruct;
}

/**************************************************************************************************
	* Function Name: GetNodeCount
    * Description:
	* Return value: ocfmRetCode
/****************************************************************************************************/
ocfmRetCode GetNodeCount(
	int 		MNID, 
	int* 		Out_NodeCount)
{
	ocfmRetCode ErrStruct;
	CNode objNode;
	CNodeCollection *objNodeCollection;
	objNodeCollection= CNodeCollection::getNodeColObjectPointer();
	
	*Out_NodeCount = objNodeCollection->getNumberOfNodes();
	
	ErrStruct.code = OCFM_ERR_SUCCESS;
	return ErrStruct;
}

char* getPIName(int NodeID)
{
	char* str;
	char* id = new char[2];
	str =(char*)malloc(5);
	strcpy(str, "CN");
	id =  itoa(NodeID,id, 10);
	strcat(str, id);
	strcat(str, ".");
	strcat(str,"\0");
	return str;
}

char* getParameterAccess(char* access)
{
	if (!strcmp(ConvertToUpper(access), "READ"))
	return "ro\0";
	else if(!strcmp(ConvertToUpper(access),"READWRITE"))
	return "rw\0";
	else return "xx";

}

/**************************************************************************************************
	* Function Name: GetIndexCount
    * Description:
	* Return value: ocfmRetCode
/****************************************************************************************************/
ocfmRetCode GetIndexCount(
	int 		NodeID, 
	ENodeType 	NodeType, 
	int* 		Out_IndexCount)
{
		ocfmRetCode ErrStruct;
		
		
	CNode objNode;		
	CNodeCollection *objNodeCollection;
	CIndexCollection *objIndexCollection;
	CIndex objIndex;
	
	int NodePos;
	ErrStruct = IfNodeExists(NodeID, NodeType, &NodePos);
	if (ErrStruct.code == 0) 
	{		
		//retPos = ErrStruct.returnValue;
	}
	else
	{	
		cout << "\n\nErrStruct.errCode.code:" << ErrStruct.code << "\n\n!!!" << endl;
		// Node Doesn't Exist
		ErrStruct.code = OCFM_ERR_INVALID_NODEID;
		return ErrStruct;
	}

	objNodeCollection = CNodeCollection::getNodeColObjectPointer();
	objNode = objNodeCollection->getNode(NodeType, NodeID);
	objIndexCollection = objNode.getIndexCollection();
	
	*Out_IndexCount = objIndexCollection->getNumberofIndexes();

	ErrStruct.code = OCFM_ERR_SUCCESS;
	return ErrStruct;
}

/**************************************************************************************************
	* Function Name: GetSubIndexCount
    * Description:
	* Return value: ocfmRetCode
/****************************************************************************************************/
ocfmRetCode GetSubIndexCount(
	int 		NodeID,
	ENodeType 	NodeType,
	char* 		IndexID,
	int* 		Out_SubIndexCount) 
{
	CNode objNode;		
	CNodeCollection *objNodeCollection;
	CIndexCollection *objIndexCollection;
	CIndex objIndex;
	CIndex* objSubIndex;
	ocfmRetCode ErrStruct;
	
	int IndexPos;
	
	ErrStruct = IfIndexExists(NodeID, NodeType, IndexID, &IndexPos);
	if (ErrStruct.code == 0)
	{				
		//IndexPos = ErrStruct.returnValue;
	}
	else
	{	
		ErrStruct.code = OCFM_ERR_INDEXID_NOT_FOUND;
		return ErrStruct;
	}
	
	objIndex.setNodeID(objNode.getNodeId());
	objNodeCollection= CNodeCollection::getNodeColObjectPointer();
	objNode = objNodeCollection->getNode(NodeType, NodeID);

	objIndexCollection = objNode.getIndexCollection();
	objSubIndex =objIndexCollection->getIndex(IndexPos);
	
	*Out_SubIndexCount = objSubIndex->getNumberofSubIndexes();
	
	ErrStruct.code = OCFM_ERR_SUCCESS;
	return ErrStruct;
}

/**************************************************************************************************
	* Function Name: GetNodeIDbyNodePos
    * Description: Fills the NodeID and NodeType for the NodeCount
	* Return value: ocfmRetCode
/****************************************************************************************************/

ocfmRetCode GetNodeIDbyNodePos(
	int NodePos, 
	ENodeType* NodeType, 
	int* Out_NodeID)
{
	cout<< "Inside GetNodeIDbyNodeCount" << endl;
	ocfmRetCode ErrStruct;
	
	int tmp_NodeCount;
	try
	{
		// We do not have support for multiple MNs in this version.
		GetNodeCount( 240, &tmp_NodeCount);
		
		if(NodePos > tmp_NodeCount)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_INVALID_NODEPOS);		
			throw objException;
		}
		else if(tmp_NodeCount == 0)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);		
			throw objException;
		}
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;

		objNodeCollection= CNodeCollection::getNodeColObjectPointer();
		
		objIndexCollection = objNode.getIndexCollection();

		objNode = objNodeCollection->getNodebyCollectionIndex(NodePos);
		
		if(objNode.getNodeType() == NULL)
		{
			cout << "OCFM_ERR_INVALID_NODETYPE" << endl;
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_INVALID_NODETYPE);
			throw objException;
		}
			*NodeType = objNode.getNodeType();
			*Out_NodeID = objNode.getNodeId();
				
		ErrStruct.code = OCFM_ERR_SUCCESS;
		return ErrStruct;
	}
	catch(ocfmException* ex)
	{
		return *(ex->_ocfmRetCode);
	}
}

/**************************************************************************************************
	* Function Name: GetIndexIDbyIndexPos
    * Description: Fills the IndexID for the IndexCount, given NodeID and the NodeType
	* Return value: ocfmRetCode
/****************************************************************************************************/

ocfmRetCode GetIndexIDbyIndexPos(
	int NodeID, 
	ENodeType NodeType, 
	int IndexPos, 
	char* Out_IndexID)
{	
	ocfmRetCode ErrStruct;
		
	CNode objNode;		
	CNodeCollection *objNodeCollection;
	CIndexCollection *objIndexCollection;
	CIndex* objIndex;
	
	int NodePos;
	try
	{
		ErrStruct = IfNodeExists(NodeID, NodeType, &NodePos);
		if (ErrStruct.code == 0) 
		{		
			//retPos = ErrStruct.returnValue;
		}
		else
		{	
			cout << "$S\n\nErrStruct.errCode.code:" << ErrStruct.code << "\n\n!!!" << endl;
			// Node Doesn't Exist
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_INVALID_NODEID);
			throw objException;
		}

		objNodeCollection = CNodeCollection::getNodeColObjectPointer();
		//objNode = objNodeCollection->getNode(NodeType, NodeID);
		objNode = objNodeCollection->getNodebyCollectionIndex(NodePos);
		objIndexCollection = objNode.getIndexCollection();
		
		int tmp_IndexCount = objIndexCollection->getNumberofIndexes();
		
		if(tmp_IndexCount == 0)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}
		else if(tmp_IndexCount < IndexPos)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_INVALID_INDEXPOS);
			throw objException;
		}
		
		objIndex = objIndexCollection->getIndex(IndexPos);
		
		if(objIndex->getIndexValue() != NULL)
			strcpy(Out_IndexID, (char *) objIndex->getIndexValue());
		
		cout << "$SOut_IndexID:" << Out_IndexID << endl;

		ErrStruct.code = OCFM_ERR_SUCCESS;
		return ErrStruct;
	}
	catch(ocfmException* ex)
	{
		return *(ex->_ocfmRetCode);
	}
}

/**************************************************************************************************
	* Function Name: GetIndexIDbyPositions
    * Description: Fills the IndexID for the IndexCount,
	* Return value: ocfmRetCode
/****************************************************************************************************/

ocfmRetCode GetIndexIDbyPositions(
	int NodePos, 
	int IndexPos, 
	char* Out_IndexID)
{
	ocfmRetCode ErrStruct;
	CNode objNode;		
	CNodeCollection *objNodeCollection;
	CIndexCollection *objIndexCollection;
	CIndex* objIndex;
		
	try
	{
		objNodeCollection= CNodeCollection::getNodeColObjectPointer();

		int tmp_NodeCount = objNodeCollection->getNumberOfNodes();
		
		if(tmp_NodeCount == 0)
		{
			ocfmException* objException = new ocfmException;			
			objException->ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		else if(tmp_NodeCount < NodePos)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_INVALID_NODEPOS);
			throw objException;
		}
		
		objNode = objNodeCollection->getNodebyCollectionIndex(NodePos);
		objIndexCollection = objNode.getIndexCollection();
		
		int tmp_IndexCount = objIndexCollection->getNumberofIndexes();
		
		if(tmp_IndexCount == 0)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}
		else if(tmp_IndexCount < IndexPos)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_INVALID_INDEXPOS);
			throw objException;
		}
		
		objIndex = objIndexCollection->getIndex(IndexPos);
		
		if(objIndex->getIndexValue() != NULL)
			strcpy(Out_IndexID, (char *) objIndex->getIndexValue());
		
		cout << "Out_IndexID:" << Out_IndexID << endl;

		ErrStruct.code = OCFM_ERR_SUCCESS;
		return ErrStruct;
	}
	catch(ocfmException* ex)
	{
		return *(ex->_ocfmRetCode);
	}
	
}

/**************************************************************************************************
	* Function Name: GetSubIndexIDbySubIndexPos
    * Description: Fills the SubIndexID for the SubIndexPos, given NodeID, NodeType and the IndexID
	* Return value: ocfmRetCode
/****************************************************************************************************/

ocfmRetCode GetSubIndexIDbySubIndexPos(
	int NodeID, 
	ENodeType NodeType, 
	char* IndexID, 
	int SubIndexPos, 
	char* Out_SubIndexID)
{
	CNode objNode;		
	CNodeCollection *objNodeCollection;
	CIndexCollection *objIndexCollection;
	CIndex objIndex;
	CIndex* objSubIndex;
	ocfmRetCode ErrStruct;
	
	int IndexPos;
	try
	{
		
		ErrStruct = IfIndexExists(NodeID, NodeType, IndexID, &IndexPos);
		if (ErrStruct.code == 0)
		{				
			//IndexPos = ErrStruct.returnValue;
		}
		else
		{	
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}
		
		//objIndex.setNodeID(objNode.getNodeId());
		objNodeCollection= CNodeCollection::getNodeColObjectPointer();
		objNode = objNodeCollection->getNode(NodeType, NodeID);

		objIndexCollection = objNode.getIndexCollection();
		objSubIndex = objIndexCollection->getIndex(IndexPos);
			
		int tmp_SubIndexCount = objSubIndex->getNumberofSubIndexes();
		if(tmp_SubIndexCount == 0)
		{		
			ocfmException* objException = new ocfmException;			
			objException->ocfm_Excpetion(OCFM_ERR_NO_SUBINDEXS_FOUND);
			throw objException;			
		}
		else if(tmp_SubIndexCount < SubIndexPos)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXPOS);
			throw objException;	
		}
	
		CSubIndex* objSubIndexPtr;
		objSubIndexPtr = objSubIndex->getSubIndex(SubIndexPos);
		
		if(objSubIndexPtr->getIndexValue() != NULL)
			strcpy(Out_SubIndexID, (char *) objSubIndexPtr->getIndexValue());	
		
		ErrStruct.code = OCFM_ERR_SUCCESS;
		return ErrStruct;
	}
	catch(ocfmException* ex)
	{
		return *(ex->_ocfmRetCode);
	}
}

/**************************************************************************************************
	* Function Name: GetSubIndexIDbyPositions
    * Description: Fills the SubIndexID for the SubIndexPos, given NodePos and the IndexPos
	* Return value: ocfmRetCode
/****************************************************************************************************/

ocfmRetCode GetSubIndexIDbyPositions(
	int NodePos, 
	int IndexPos, 
	int SubIndexPos, 
	char* Out_SubIndexID)

{
	CNode objNode;		
	CNodeCollection *objNodeCollection;
	CIndexCollection *objIndexCollection;
	CIndex objIndex;
	CIndex* objSubIndex;
	ocfmRetCode ErrStruct;

	try
	{		
		objNodeCollection= CNodeCollection::getNodeColObjectPointer();

		int tmp_NodeCount = objNodeCollection->getNumberOfNodes();
		
		if(tmp_NodeCount == 0)
		{
			ocfmException* objException = new ocfmException;			
			objException->ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		else if(tmp_NodeCount < NodePos)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_INVALID_NODEPOS);
			throw objException;
		}
		
		objNode = objNodeCollection->getNodebyCollectionIndex(NodePos);		
		objIndexCollection = objNode.getIndexCollection();
		
		int tmp_IndexCount = objIndexCollection->getNumberofIndexes();		
		if(tmp_IndexCount == 0)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}
		else if(tmp_IndexCount < IndexPos)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_INVALID_INDEXPOS);
			throw objException;
		}
		
		objSubIndex = objIndexCollection->getIndex(IndexPos);
			
		int tmp_SubIndexCount = objSubIndex->getNumberofSubIndexes();
		if(tmp_SubIndexCount == 0)
		{		
			ocfmException* objException = new ocfmException;			
			objException->ocfm_Excpetion(OCFM_ERR_NO_SUBINDEXS_FOUND);
			throw objException;			
		}
		else if(tmp_SubIndexCount < SubIndexPos)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXPOS);
			throw objException;	
		}
	
		CSubIndex* objSubIndexPtr;
		objSubIndexPtr = objSubIndex->getSubIndex(SubIndexPos);
		
		if(objSubIndexPtr->getIndexValue() != NULL)
			strcpy(Out_SubIndexID, (char *) objSubIndexPtr->getIndexValue());	
		
		ErrStruct.code = OCFM_ERR_SUCCESS;
		return ErrStruct;
	}
	catch(ocfmException* ex)
	{
		return *(ex->_ocfmRetCode);
	}
}

/**************************************************************************************************
	* Function Name: DeleteMNObjDict
    * Description: Deletes the MN's Object Dictinary.
	* Return value: ocfmRetCode
/****************************************************************************************************/

ocfmRetCode DeleteMNObjDict(
	int NodeID)
{
	ocfmRetCode ErrStruct;
	int NodePos;
	cout << "\n$SInside DeleteMNObjDict..\n" << endl;
	try
	{	
		ErrStruct = IfNodeExists(NodeID, MN, &NodePos);
		
		if(ErrStruct.code == 0)
		{
		}
		else
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_INVALID_NODEID);
			throw objException;
		}
		
		CNode objNode;
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		CDataTypeCollection *objDataTypeCollection;
		CIndex objIndex;

		objIndex.setNodeID(objNode.getNodeId());
		objNodeCollection= CNodeCollection::getNodeColObjectPointer();
		objNode = objNodeCollection->getNode(MN, NodeID);
		
		objDataTypeCollection = objNode.getDataTypeCollection();

		objIndexCollection = objNode.getIndexCollection();

		// Delete IndexCollection
		objIndexCollection->DeleteIndexCollection();
		// Delete DataTypeCollection
		objDataTypeCollection->DeleteDataTypeCollection();
		cout << "\n\n$SDelete MN OBD Success" << endl;
		ErrStruct.code = OCFM_ERR_SUCCESS;
		return ErrStruct;
	}
	catch(ocfmException* ex)
	{
		return *(ex->_ocfmRetCode);
	}
}
