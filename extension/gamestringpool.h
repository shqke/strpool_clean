#ifndef _INCLUDE_GAMESTRINGPOOL_H_
#define _INCLUDE_GAMESTRINGPOOL_H_

#include <tier1/stringpool.h>

class CBasePlayer;
class CUserCmd;
#include <igamesystem.h>

class CGameStringPool :
	public CStringPool,
	public CBaseGameSystem
{
public:
	void Remove(const char* pszValue);
	void MaybeSetGameStringPool();

	static void PurgeDeferredDeleteList();
};

#endif // _INCLUDE_GAMESTRINGPOOL_H_
