#include "smsdk_config.h"
#include "gamestringpool.h"

#include <vector>

// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

CGameStringPool* g_pGameStringPool = NULL;
static std::vector<const char*> s_DeferredDeleteList;

void CGameStringPool::Remove(const char* pszValue)
{
	int i = m_Strings.Find(pszValue);
	if (i != m_Strings.InvalidIndex())
	{
		s_DeferredDeleteList.push_back(m_Strings[i]);
		m_Strings.RemoveAt(i);
	}
}

void CGameStringPool::MaybeSetGameStringPool()
{
	if (strcmp(Name(), "CGameStringPool") == 0) {
		g_pGameStringPool = this;
	}
}

void CGameStringPool::PurgeDeferredDeleteList()
{
	DevMsg(PLUGIN_LOG_PREFIX "Removing %d strings\n", s_DeferredDeleteList.size());

	for (auto&& str : s_DeferredDeleteList) {
		free((void*)str);
	}

	s_DeferredDeleteList.clear();
}
