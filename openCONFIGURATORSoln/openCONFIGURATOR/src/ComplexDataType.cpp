#include "../Include/ComplexDataType.h"
#include <string.h>
#include <stdlib.h>


CComplexDataType::CComplexDataType(void)
	{
			name_id_attr = new appProcessCommon;
			name_id_attr->dataType = NULL;
			name_id_attr->dataTypeUniqueIDRef = NULL;
			name_id_attr->m_Name = NULL;
			name_id_attr->UniqueId = NULL;
			VarIndex = -1;
			
	}

CComplexDataType::~CComplexDataType(void)
	{
  }
void CComplexDataType::addVarDeclaration(varDeclaration VarDecl)
 {
		int i = varCollection.Add();
		varCollection[i] = VarDecl;
		m_varCount = varCollection.Count();
 }