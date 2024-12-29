/// amaranth_test.cpp

#include "pch.h"
#include "steam_glue.h"
#include "steam_api.h"
#include "Extension_Interface.h"
#include "YYRValue.h"
#include "steam_common.h"

YYEXPORT void /*double*/ amaranth_add(RValue& Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg)//(double auto_accept)
{
    double firstNum = YYGetReal(arg, 0);
    double secondNum = YYGetReal(arg, 1);

    Result.kind = VALUE_REAL;
    Result.val = (double) firstNum+secondNum;
}