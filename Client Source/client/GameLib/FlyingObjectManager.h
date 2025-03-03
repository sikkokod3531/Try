#pragma once

#include "FlyTarget.h"

#include <set>

class CFlyingInstance;
class CFlyingData;
class CMapManager;
class CActorInstance;

class CFlyingManager : public CSingleton<CFlyingManager>
{
public:
	enum EFlyIndex
	{
		FLY_NONE,
		FLY_EXP,
		FLY_HP_MEDIUM,
		FLY_HP_BIG,
		FLY_SP_SMALL,
		FLY_SP_MEDIUM,
		FLY_SP_BIG,
		FLY_FIREWORK1,
		FLY_FIREWORK2,
		FLY_FIREWORK3,
		FLY_FIREWORK4,
		FLY_FIREWORK5,
		FLY_FIREWORK6,
		FLY_FIREWORK_XMAS,
		FLY_CHAIN_LIGHTNING,
		FLY_HP_SMALL,
		FLY_SKILL_MUYEONG,
#ifdef ENABLE_QUIVER_SYSTEM
		FLY_QUIVER_ATTACK_NORMAL,
#endif
	};

	enum EIndexFlyType
	{
		INDEX_FLY_TYPE_NORMAL,
		INDEX_FLY_TYPE_FIRE_CRACKER,
		INDEX_FLY_TYPE_AUTO_FIRE,
	};

public:
	CFlyingManager();
	virtual ~CFlyingManager();

	void Destroy();

	void DeleteAllInstances();

	bool RegisterFlyingData(const char* c_szFilename);
	bool RegisterFlyingData(const char* c_szFilename, DWORD& r_dwRetCRC);

#ifdef ENABLE_SKILL_COLOR_SYSTEM
	CFlyingInstance* CreateFlyingInstanceFlyTarget(const DWORD dwID, const D3DXVECTOR3& v3StartPosition, const CFlyTarget& cr_FlyTarget, bool canAttack, DWORD* dwSkillColor = NULL);
#else
	CFlyingInstance* CreateFlyingInstanceFlyTarget(const DWORD dwID, const D3DXVECTOR3& v3StartPosition, const CFlyTarget& cr_FlyTarget, bool canAttack);
#endif
#ifdef ENABLE_QUIVER_SYSTEM
#ifdef ENABLE_SKILL_COLOR_SYSTEM
	CFlyingInstance* CreateIndexedFlyingInstanceFlyTarget(const DWORD dwIndex, const D3DXVECTOR3& v3StartPosition, const CFlyTarget& cr_FlyTarget, DWORD* dwSkillColor = NULL);
#else
	CFlyingInstance* CreateIndexedFlyingInstanceFlyTarget(const DWORD dwIndex, const D3DXVECTOR3& v3StartPosition, const CFlyTarget& cr_FlyTarget);
#endif
#endif
	void Update();
	void Render();

	void SetMapManagerPtr(CMapManager* pMapManager) { m_pMapManager = pMapManager; }
	CMapManager* GetMapManagerPtr() { return m_pMapManager; }

public: // Controlled by Server
	bool RegisterIndexedFlyData(DWORD dwIndex, BYTE byType, const char* c_szFileName);
	void CreateIndexedFly(DWORD dwIndex, CActorInstance* pStartActor, CActorInstance* pEndActor);

private:
	void __DestroyFlyingInstanceList();
	void __DestroyFlyingDataMap();

	typedef std::map<DWORD, CFlyingData*> TFlyingDataMap;
	typedef std::list<CFlyingInstance*> TFlyingInstanceList;

	typedef struct SIndexFlyData
	{
		BYTE byType;
		DWORD dwCRC;
	} TIndexFlyData;
	typedef std::map<DWORD, TIndexFlyData> TIndexFlyDataMap;

	TFlyingDataMap			m_kMap_pkFlyData;
	TFlyingInstanceList		m_kLst_pkFlyInst;
	TIndexFlyDataMap		m_kMap_dwIndexFlyData;

	CMapManager* m_pMapManager;

	DWORD m_IDCounter;
};
