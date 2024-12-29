// https://partner.steamgames.com/doc/api/ISteamNetworkingSockets

#include "pch.h"
#include "steam_glue.h"
#include "steam_api.h"
#include "Extension_Interface.h"
#include "YYRValue.h"
#include "steam_common.h"

YYEXPORT void steam_netsock_init(RValue& Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg)//() 
{
	// You plan to use networking in your app. This should be called on the first frame.
	
	// Needs to call InitRelayNetworkAccess

	Result.kind = VALUE_BOOL;
	Result.val = true;
}

YYEXPORT void steam_netsock_create_listen_socket(RValue& Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg)//() 
{
	// https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#CreateListenSocketP2P

	// Code Update Needed

	// Results Update Needed
	Result.kind = VALUE_BOOL;
	Result.val = true;
}

YYEXPORT void steam_netsock_connect(RValue& Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg)//(int64 identifier) 
{
	int64 identifier = YYGetInt64(arg, 0);

	// Code Update Needed

	// Results Update Needed
	Result.kind = VALUE_BOOL;
	Result.val = true;
}