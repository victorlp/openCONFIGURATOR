#include "../Include/ComplexDataType.h"


CComplexDataType::CComplexDataType(void)
	{
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