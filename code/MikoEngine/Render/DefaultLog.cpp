#include "stdafx.h"
#include "DefaultLog.h"
//-----------------------------------------------------------------------------
// TODO: �������� ���������� �� ���������� ����������
//-----------------------------------------------------------------------------
DefaultLog gDefaultLog;
//-----------------------------------------------------------------------------
DefaultLog& GetLog()
{
	return gDefaultLog;
}
//-----------------------------------------------------------------------------