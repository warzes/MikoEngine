#include "stdafx.h"
#include "DefaultAllocator.h"
//-----------------------------------------------------------------------------
// TODO: �������� ���������� �� ���������� ����������
//-----------------------------------------------------------------------------
DefaultAllocator gDefaultAllocator;
//-----------------------------------------------------------------------------
DefaultAllocator& GetAllocator()
{
	return gDefaultAllocator;
}
//-----------------------------------------------------------------------------