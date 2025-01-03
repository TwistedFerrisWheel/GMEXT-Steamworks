// https://partner.steamgames.com/doc/api/ISteamNetworkingSockets

#include "pch.h"
#include "steam_glue.h"
#include "steam_api.h"
#include "Extension_Interface.h"
#include "YYRValue.h"
#include "steam_common.h"

// We started working on this and then realized we needed https://partner.steamgames.com/doc/sdk/api#steam_game_servers

YYEXPORT void steam_netsock_init(RValue& Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg)//() 
{
	// You plan to use networking in your app. This should be called on the first frame.
	// https://partner.steamgames.com/doc/api/ISteamNetworkingUtils#InitRelayNetworkAccess
	
	SteamNetworkingUtils()->InitRelayNetworkAccess();

	Result.kind = VALUE_BOOL;
	Result.val = true;
}

YYEXPORT void steam_netsock_create_socket(RValue& Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg)//() 
{
	// https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#CreateListenSocketP2P
	// https://partner.steamgames.com/doc/api/steamnetworkingtypes#SteamNetworkingConfigValue_t

	// I'm unsure how to create a SteamNetworkingConfigValue_t
	SteamNetworkingConfigValue_t listen_socket_config = {};
	SteamNetworkingSockets()->CreateListenSocketP2P(0, 0, &listen_socket_config);

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