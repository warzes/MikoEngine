﻿#include "stdafx.h"
#include "DefaultAssert.h"
//-----------------------------------------------------------------------------
// TODO: возможно избавиться от глобальной переменной
//-----------------------------------------------------------------------------
DefaultAssert gDefaultAssert;
//-----------------------------------------------------------------------------
DefaultAssert& GetAssert()
{
	return gDefaultAssert;
}
//-----------------------------------------------------------------------------