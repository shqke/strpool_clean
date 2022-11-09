#ifndef _INCLUDE_SOURCEMOD_EXTENSION_CONFIG_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_CONFIG_H_

#define SMEXT_CONF_NAME			"CGameStringPool cleaner"
#define SMEXT_CONF_DESCRIPTION	"Cleans up stringpool from removed entities"
#define SMEXT_CONF_VERSION		"0.6"
#define SMEXT_CONF_AUTHOR		"Evgeniy \"shqke\" Kazakov"
#define SMEXT_CONF_URL			"https://github.com/shqke/strpool_clean"
#define SMEXT_CONF_LOGTAG		"strpool-clean"
#define SMEXT_CONF_LICENSE		"GPL"
#define SMEXT_CONF_DATESTRING	__DATE__

#define GAMEDATA_FILE			"strpool_clean"
#define PLUGIN_LOG_PREFIX		"[" SMEXT_CONF_LOGTAG "] "

#define SMEXT_LINK(name) SDKExtension *g_pExtensionIface = name;

#define SMEXT_CONF_METAMOD
#define SMEXT_ENABLE_GAMEHELPERS
#define SMEXT_ENABLE_GAMECONF
#define SMEXT_ENABLE_PLAYERHELPERS

#endif // _INCLUDE_SOURCEMOD_EXTENSION_CONFIG_H_
