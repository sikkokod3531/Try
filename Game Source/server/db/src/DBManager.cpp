#include "stdafx.h"
#include "DBManager.h"
#include "ClientManager.h"

extern std::string g_stLocale;

CDBManager::CDBManager()
{
	Initialize();
}

CDBManager::~CDBManager()
{
	Destroy();
}

void CDBManager::Initialize()
{
	for (int i = 0; i < SQL_MAX_NUM; ++i)
	{
		m_mainSQL[i] = NULL;
		m_directSQL[i] = NULL;
		m_asyncSQL[i] = NULL;
	}
}

void CDBManager::Destroy()
{
	Clear();
}

void CDBManager::Clear()
{
	for (int i = 0; i < SQL_MAX_NUM; ++i)
	{
		if (m_mainSQL[i])
		{
			delete m_mainSQL[i];
			m_mainSQL[i] = NULL;
		}

		if (m_directSQL[i])
		{
			delete m_directSQL[i];
			m_directSQL[i] = NULL;
		}

		if (m_asyncSQL[i])
		{
			delete m_asyncSQL[i];
			m_asyncSQL[i] = NULL;
		}
	}

	Initialize();
}

void CDBManager::Quit()
{
	for (int i = 0; i < SQL_MAX_NUM; ++i)
	{
		if (m_mainSQL[i])
			m_mainSQL[i]->Quit();

		if (m_asyncSQL[i])
			m_asyncSQL[i]->Quit();

		if (m_directSQL[i])
			m_directSQL[i]->Quit();
	}
}

SQLMsg* CDBManager::PopResult()
{
	SQLMsg* p;

	for (int i = 0; i < SQL_MAX_NUM; ++i)
		if (m_mainSQL[i] && m_mainSQL[i]->PopResult(&p))
			return p;

	return NULL;
}

SQLMsg* CDBManager::PopResult(eSQL_SLOT slot)
{
	SQLMsg* p;

	if (m_mainSQL[slot] && m_mainSQL[slot]->PopResult(&p))
		return p;

	return NULL;
}
int CDBManager::Connect(int iSlot, const char* db_address, const int db_port, const char* db_name, const char* user, const char* pwd)
{
	if (db_address == NULL || db_name == NULL)
		return false;

	if (iSlot < 0 || iSlot >= SQL_MAX_NUM)
		return false;

	if (m_directSQL[iSlot] != nullptr)
	{
		sys_log(0, "ALREADY CONNECTED! RECONNECT DENIED! (slot %d)", iSlot);
		return true;
		// return m_directSQL[iSlot]->IsConnected();
	}

	sys_log(0, "CREATING DIRECT_SQL");

	m_directSQL[iSlot] = new CAsyncSQL2;
	if (!m_directSQL[iSlot]->Setup(db_address, user, pwd, db_name, g_stLocale.c_str(), true, db_port))
	{
		Clear();
		return false;
	}

	sys_log(0, "CREATING MAIN_SQL");
	m_mainSQL[iSlot] = new CAsyncSQL2;
	if (!m_mainSQL[iSlot]->Setup(db_address, user, pwd, db_name, g_stLocale.c_str(), false, db_port))
	{
		Clear();
		return false;
	}

	sys_log(0, "CREATING ASYNC_SQL");
	m_asyncSQL[iSlot] = new CAsyncSQL2;
	if (!m_asyncSQL[iSlot]->Setup(db_address, user, pwd, db_name, g_stLocale.c_str(), false, db_port))
	{
		Clear();
		return false;
	}

	return true;
}

SQLMsg* CDBManager::DirectQuery(const char* c_pszQuery, int iSlot)
{
	return m_directSQL[iSlot]->DirectQuery(c_pszQuery);
}

void CDBManager::ReturnQuery(const char* c_pszQuery, int iType, IDENT dwIdent, void* udata, int iSlot)
{
	assert(iSlot < SQL_MAX_NUM);
	//sys_log(0, "ReturnQuery %s", c_pszQuery);
	CQueryInfo* p = new CQueryInfo;

	p->iType = iType;
	p->dwIdent = dwIdent;
	p->pvData = udata;

	m_mainSQL[iSlot]->ReturnQuery(c_pszQuery, p);

}

void CDBManager::AsyncQuery(const char* c_pszQuery, int iSlot)
{
	assert(iSlot < SQL_MAX_NUM);
	m_asyncSQL[iSlot]->AsyncQuery(c_pszQuery);
}

unsigned long CDBManager::EscapeString(void* to, const void* from, unsigned long length, int iSlot)
{
	assert(iSlot < SQL_MAX_NUM);
	return mysql_real_escape_string(m_directSQL[iSlot]->GetSQLHandle(), (char*)to, (const char*)from, length);
}

void CDBManager::SetLocale(const char* szLocale)
{
	const std::string stLocale(szLocale);
	sys_log(0, "SetLocale start %s", szLocale);
	for (int n = 0; n < SQL_MAX_NUM; ++n)
	{
		m_mainSQL[n]->SetLocale(stLocale);
		m_directSQL[n]->SetLocale(stLocale);
		m_asyncSQL[n]->SetLocale(stLocale);
	}
	sys_log(0, "SetLocale end %s", szLocale);
}

void CDBManager::QueryLocaleSet()
{
	for (int n = 0; n < SQL_MAX_NUM; ++n)
	{
		m_mainSQL[n]->QueryLocaleSet();
		m_directSQL[n]->QueryLocaleSet();
		m_asyncSQL[n]->QueryLocaleSet();
	}
}