#ifndef _INCLUDE_EXTENSION_PROPER_H_
#define _INCLUDE_EXTENSION_PROPER_H_

#include "smsdk_ext.h"

#include <extensions/ISDKHooks.h>

class CExtension :
	public SDKExtension,
	public ISMEntityListener
{
protected:
	bool SetupFromGameConfig(IGameConfig* gc, char* error, int maxlength);

public: // SourceHook callbacks
	void Handler_CGameStringPool_LevelShutdownPostEntity();

public:
	/**
	 * @brief	When an entity is destroyed
	 *
	 * @param	pEntity		CBaseEntity entity.
	 */
	void OnEntityDestroyed(CBaseEntity* pEntity) override;

public:
	/**
	 * @brief This is called after the initial loading sequence has been processed.
	 *
	 * @param error		Error message buffer.
	 * @param maxlength	Size of error message buffer.
	 * @param late		Whether or not the module was loaded after map load.
	 * @return			True to succeed loading, false to fail.
	 */
	bool SDK_OnLoad(char* error, size_t maxlength, bool late) override;

	/**
	 * @brief This is called once the extension unloading process begins.
	 */
	void SDK_OnUnload() override;

	/**
	 * @brief This is called once all known extensions have been loaded.
	 */
	void SDK_OnAllLoaded() override;

	/**
	 * @brief Notifies the extension that an external interface it uses is being removed.
	 *
	 * @param pInterface		Pointer to interface being dropped.  This
	 * 							pointer may be opaque, and it should not
	 *							be queried using SMInterface functions unless
	 *							it can be verified to match an existing
	 */
	void NotifyInterfaceDrop(SMInterface* pInterface) override;

	/**
	 * @brief Return false to tell Core that your extension should be considered unusable.
	 *
	 * @param error				Error buffer.
	 * @param maxlength			Size of error buffer.
	 * @return					True on success, false otherwise.
	 */
	bool QueryRunning(char* error, size_t maxlength) override;
};

#endif // _INCLUDE_EXTENSION_PROPER_H_
