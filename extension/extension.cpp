#include "extension.h"
#include "gamestringpool.h"

#include <sourcehook.h>

CExtension g_Extension;
SMEXT_LINK(&g_Extension);

SH_DECL_HOOK0_void(IGameSystem, LevelShutdownPostEntity, SH_NOATTRIB, 0);

ISDKHooks* sdkhooks = NULL;
IServerGameEnts* gameents = NULL;

extern CGameStringPool* g_pGameStringPool;

// Pointer to a member method of IGameSystem
typedef void (CGameStringPool::* GameSystemFunc_t)();
void (*InvokeMethodReverseOrder)(GameSystemFunc_t f, char const* timed);

inline void RemovePooledString(CBaseEntity* pEntity, const char* pszValue)
{
	datamap_t* pDatamap = gamehelpers->GetDataMap(pEntity);
	if (pDatamap == NULL) {
		return;
	}

	typedescription_t* pTypeDesc = gamehelpers->FindInDataMap(pDatamap, pszValue);
	if (pTypeDesc == NULL) {
		return;
	}

	const string_t& str = *reinterpret_cast<string_t*>(reinterpret_cast<byte*>(pEntity) + pTypeDesc->fieldOffset);
	if (str == NULL_STRING) {
		return;
	}

	g_pGameStringPool->Remove(STRING(str));
}

bool CExtension::SetupFromGameConfig(IGameConfig* gc, char* error, int maxlength)
{
	static const struct {
		const char* key;
		void*& address;
	} s_sigs[] = {
#ifdef PLATFORM_WINDOWS
		{ "InvokeMethodReverseOrder", (void*&)InvokeMethodReverseOrder },
#else
		{ "g_GameStringPool", (void*&)g_pGameStringPool },
#endif
	};

	for (auto&& el : s_sigs) {
		if (!gc->GetMemSig(el.key, &el.address)) {
			snprintf(error, maxlength, "Unable to find signature for \"%s\" from game config (file: \"" GAMEDATA_FILE ".txt\")", el.key);

			return false;
		}

		if (el.address == NULL) {
			snprintf(error, maxlength, "Sigscan for \"%s\" failed (game config file: \"" GAMEDATA_FILE ".txt\")", el.key);

			return false;
		}
	}

#ifdef PLATFORM_WINDOWS
	// Query each IGameSystem and store CGameStringPool pointer if there's one (and there should be)
	InvokeMethodReverseOrder(&CGameStringPool::MaybeSetGameStringPool, NULL);
#endif

	if (g_pGameStringPool == NULL) {
		snprintf(error, maxlength, "g_pGameStringPool is NULL");

		return false;
	}

	return true;
}

void CExtension::Handler_CGameStringPool_LevelShutdownPostEntity()
{
	CGameStringPool::PurgeDeferredDeleteList();
}

void CExtension::OnEntityDestroyed(CBaseEntity* pEntity)
{
	if (pEntity == NULL) {
		return;
	}

	RemovePooledString(pEntity, "m_iName");
	RemovePooledString(pEntity, "m_iszScriptId");
}

bool CExtension::SDK_OnLoad(char* error, size_t maxlength, bool late)
{
	IGameConfig* gc = NULL;
	if (!gameconfs->LoadGameConfigFile(GAMEDATA_FILE, &gc, error, maxlength)) {
		strncpy(error, "Unable to load a gamedata file \"" GAMEDATA_FILE ".txt\"", maxlength);

		return false;
	}

	if (!SetupFromGameConfig(gc, error, maxlength)) {
		gameconfs->CloseGameConfigFile(gc);

		return false;
	}

	gameconfs->CloseGameConfigFile(gc);

	sharesys->AddDependency(myself, "bintools.ext", true, true);
	sharesys->AddDependency(myself, "sdkhooks.ext", true, true);

	return true;
}

void CExtension::SDK_OnUnload()
{
	if (sdkhooks != NULL) {
		sdkhooks->RemoveEntityListener(this);
	}

	SH_REMOVE_HOOK(IGameSystem, LevelShutdownPostEntity, g_pGameStringPool, SH_MEMBER(this, &CExtension::Handler_CGameStringPool_LevelShutdownPostEntity), true);
}

void CExtension::SDK_OnAllLoaded()
{
	SM_GET_LATE_IFACE(SDKHOOKS, sdkhooks);
	if (sdkhooks == NULL) {
		return;
	}

	sdkhooks->AddEntityListener(this);

	SH_ADD_HOOK(IGameSystem, LevelShutdownPostEntity, g_pGameStringPool, SH_MEMBER(this, &CExtension::Handler_CGameStringPool_LevelShutdownPostEntity), true);
}

void CExtension::NotifyInterfaceDrop(SMInterface* pInterface)
{
	SDK_OnUnload();
}

bool CExtension::QueryRunning(char* error, size_t maxlength)
{
	SM_CHECK_IFACE(SDKHOOKS, sdkhooks);

	return true;
}
