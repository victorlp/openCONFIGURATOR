#include "../Include/ComplexDataType.h"
#include <string.h>
#include <stdlib.h>


CComplexDataType::CComplexDataType(void)
	{
			name_id_attr = new appProcessCommon;
	}

CComplexDataType::~CComplexDataType(void)
	{
  }
void CComplexDataType::addVarDeclaration(varDeclaration VarDecl)
 {
		int i = collectionObj.Add();
		collectionObj[i] = VarDecl;
		m_varCount = collectionObj.Count();
 }