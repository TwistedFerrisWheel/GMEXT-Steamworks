
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