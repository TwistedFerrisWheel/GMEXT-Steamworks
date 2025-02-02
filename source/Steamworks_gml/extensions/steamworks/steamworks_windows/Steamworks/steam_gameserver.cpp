
#include "pch.h"
#include "steam_api.h"
#include "Extension_Interface.h"
#include "YYRValue.h"
#include "steam_gameserver.h"
#include "steam_glue.h"
#include "steam_callbacks.h"

#include <filesystem>
#include <string>
#include <vector>
#include <sstream>

#include "DesktopExtensionTools.h"

// https://partner.steamgames.com/doc/sdk/api#steam_game_servers

bool steam_gameserver_is_initialised = false;

YYEXPORT void steam_gameserver_update(RValue& Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg)
{
	if (!steam_gameserver_is_initialised)
	{
		Result.kind = VALUE_REAL;
		Result.val = 0;
		return;
	}

	SteamGameServer_RunCallbacks();

	Result.kind = VALUE_REAL;
	Result.val = 1;
}

YYEXPORT void steam_gameserver_init(RValue& Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg)//(int appid)
{
	uint16 game_port = static_cast<uint16>(YYGetReal(arg, 0));
	uint16 query_port = static_cast<uint16>(YYGetReal(arg, 1));
	EServerMode server_mode = static_cast<EServerMode>(YYGetReal(arg, 2));
	const char* server_version = YYGetString(arg, 3);

	//Can not use eServerModeInvalid as a server mode
	if (server_mode == EServerMode::eServerModeInvalid) {
		Result.kind = VALUE_REAL;
		Result.val = 0;
		return;
	}

	// https://partner.steamgames.com/doc/api/steam_gameserver
	//0x00000000 binds to any avaiable IPV4 Address

	Result.kind = VALUE_BOOL;
	Result.val = SteamGameServer_Init(0x00000000, game_port, query_port, server_mode, server_version);

	steam_gameserver_is_initialised = Result.val;
}

YYEXPORT void steam_gameserver_logon(RValue& Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg)
{
	const char* account_id = YYGetString(arg, 0);
	SteamGameServer()->LogOn(account_id);
	Result.kind = VALUE_BOOL;
	Result.val = true;
}

YYEXPORT void steam_gameserver_logon_anonymous(RValue& Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg)
{
	SteamGameServer()->LogOnAnonymous();
	Result.kind = VALUE_BOOL;
	Result.val = true;
}

YYEXPORT void steam_gameserver_initialised(RValue& Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg)
{
	Result.kind = VALUE_BOOL;
	Result.val = steam_gameserver_is_initialised;
}

extern "C" void __cdecl SteamGameServerDebugTextHook(int nSeverity, const char* pchDebugText)
{
	DebugConsoleOutput(pchDebugText);
	DebugConsoleOutput("\n");
}

YYEXPORT void steam_gameserver_set_warning_message_hook(RValue& Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg)
{
	if (!steam_gameserver_is_initialised)
	{
		Result.kind = VALUE_REAL;
		Result.val = 0;
		return;
	}

	SteamGameServerClient()->SetWarningMessageHook(&SteamGameServerDebugTextHook);
}


YYEXPORT void steam_gameserver_shutdown(RValue& Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg)
{
	if (!steam_gameserver_is_initialised)
	{
		Result.kind = VALUE_REAL;
		Result.val = 0;
		return;
	}

	if (SteamGameServer()->BLoggedOn())
		SteamGameServer()->LogOff();

	SteamGameServer_Shutdown();
}

YYEXPORT void steam_gameserver_is_loggedOn(RValue& Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg)
{
	Result.kind = VALUE_BOOL;
	Result.val = SteamGameServer()->BLoggedOn();
}

YYEXPORT void steam_gameserver_get_IP(RValue& Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg) {

	SteamIPAddress_t IPAddr = SteamGameServer()->GetPublicIP();
	if (IPAddr.IsSet()) {
		Result.kind = VALUE_INT32;
		Result.val = IPAddr.m_unIPv4;
	}
	else {
		Result.kind = VALUE_INT32;
		Result.val = 0;
	}
}

YYEXPORT void steam_gameserver_get_ID(RValue& Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg) {
	Result.kind = VALUE_INT64;
	Result.val = SteamGameServer()->GetSteamID().ConvertToUint64();
}

//Setting Server Data
YYEXPORT void steam_gameserver_set_dedicated(RValue& Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg) {
	bool dedicated = YYGetBool(arg, 0);
	SteamGameServer()->SetDedicatedServer(dedicated);
	Result.kind = VALUE_BOOL;
	Result.val = dedicated;
}

YYEXPORT void steam_gameserver_set_advertise(RValue& Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg) {
	bool active = YYGetBool(arg, 0);
	SteamGameServer()->SetAdvertiseServerActive(active);
	Result.kind = VALUE_REAL;
	Result.val = 0;
}

YYEXPORT void steam_gameserver_set_name(RValue& Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg) {
	const char* server_name = YYGetString(arg, 0);

	int len = strlen(server_name);

	if (server_name == NULL or server_name == "" or len > k_cbMaxGameServerName) {
		Result.kind = VALUE_REAL;
		Result.val = 0;
		return;
	}

	SteamGameServer()->SetServerName(server_name);
	Result.kind = VALUE_REAL;
	Result.val = 1;
}

YYEXPORT void steam_gameserver_set_map(RValue& Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg) {
	const char* map_name = YYGetString(arg, 0);

	int len = strlen(map_name);

	if (map_name == NULL or map_name == "" or len > k_cbMaxGameServerMapName) {
		Result.kind = VALUE_REAL;
		Result.val = 0;
		return;
	}

	SteamGameServer()->SetMapName(map_name);
	Result.kind = VALUE_REAL;
	Result.val = 1;
}

YYEXPORT void steam_gameserver_set_description(RValue& Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg) {
	const char* description = YYGetString(arg, 0);

	if (description == NULL or description == "") {
		Result.kind = VALUE_REAL;
		Result.val = 0;
		return;
	}

	SteamGameServer()->SetGameDescription(description);
	Result.kind = VALUE_REAL;
	Result.val = 1;
}


//Usually seperated with semi-colons: https://partner.steamgames.com/doc/api/ISteamGameServer#SetGameTags 
YYEXPORT void steam_gameserver_set_tags(RValue& Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg) {
	const char* tags = YYGetString(arg, 0);

	if (tags == NULL or tags == "") {
		Result.kind = VALUE_REAL;
		Result.val = 0;
		return;
	}

	SteamGameServer()->SetGameTags(tags);
	Result.kind = VALUE_REAL;
	Result.val = 1;
}

//Don't set this unless it actually changes, its only uploaded to the master once; when acknowledged.
//Usually seperated with semi-colons: https://partner.steamgames.com/doc/api/ISteamGameServer#SetGameTags 
YYEXPORT void steam_gameserver_set_game_data(RValue& Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg) {
	const char* data = YYGetString(arg, 0);

	if (data == NULL or data == "") {
		Result.kind = VALUE_REAL;
		Result.val = 0;
		return;
	}

	SteamGameServer()->SetGameData(data);
	Result.kind = VALUE_REAL;
	Result.val = 1;
}


//Converting Game ID to String is recommended for this: https://partner.steamgames.com/doc/api/ISteamGameServer#SetProduct
YYEXPORT void steam_gameserver_set_product(RValue& Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg) {
	const char* product = YYGetString(arg, 0);

	if (product == NULL or product == "") {
		Result.kind = VALUE_REAL;
		Result.val = 0;
		return;
	}

	SteamGameServer()->SetProduct(product);
	Result.kind = VALUE_REAL;
	Result.val = 1;
}

YYEXPORT void steam_gameserver_set_region(RValue& Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg) {
	const char* region = YYGetString(arg, 0);
	SteamGameServer()->SetRegion(region);
	Result.kind = VALUE_REAL;
	Result.val = 1;
}

YYEXPORT void steam_gameserver_set_max_players(RValue& Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg) {
	int max_players = YYGetReal(arg, 0);

	if (max_players < 0) {
		Result.kind = VALUE_REAL;
		Result.val = 0;
		return;
	}

	SteamGameServer()->SetMaxPlayerCount(max_players);
	Result.kind = VALUE_REAL;
	Result.val = 1;
}

YYEXPORT void steam_gameserver_set_password_protected(RValue& Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg) {
	bool pswd_locked = YYGetBool(arg, 0);
	SteamGameServer()->SetPasswordProtected(pswd_locked);
	Result.kind = VALUE_BOOL;
	Result.val = pswd_locked;
}

//Callbacks
void steam_net_callbacks_t::steam_server_connected(SteamServersConnected_t* s) {
	steam_net_event ev = steam_net_event((char*)"server_connected");
	ev.dispatch();
}

void steam_net_callbacks_t::steam_server_connect_failed(SteamServerConnectFailure_t* s) {
	steam_net_event ev = steam_net_event((char*)"server_failed_connection");
	ev.set((char*)"trying_for_connection", s->m_bStillRetrying);
	ev.set_result(s->m_eResult);
	ev.dispatch();
}

void steam_net_callbacks_t::steam_server_disconnected(SteamServersDisconnected_t* s) {
	steam_net_event ev = steam_net_event((char*)"server_failed_connection");
	ev.set_result(s->m_eResult);
	ev.dispatch();
}