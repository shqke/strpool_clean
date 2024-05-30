Description
------
Aims to aggressively patch a game fatal error "CUtlRBTree overflow" with `CGameStringPool` (`CStringPool::Allocate`) being a culprit.

Currently extension targets entity names (`targetname`/`m_iName`) and unique vscript script ids (`m_iszScriptId`).

Requirements
------
- [MM:Source (1.10+)](https://www.sourcemm.net/)
- [SourceMod (1.9+)](https://www.sourcemod.net/)

Supported Games
------
- [Left 4 Dead 2](https://store.steampowered.com/app/550/Left_4_Dead_2/)
