#include "StdAfx.h"
#ifdef ENABLE_LOGIN_DLG_RENEWAL
#include "PythonAccountManager.h"

#include "cryptopp/aes.h"
#include "cryptopp/modes.h"
#include "cryptopp/sha.h"
#include "cryptopp/filters.h"
#include "cryptopp/hex.h"
#include "cryptopp/base64.h"

#ifdef ENABLE_HWID_SECURITY_UTILITY
#include "HWIDManager.h"
#endif

CPythonAccountManager::CPythonAccountManager()
{
}

CPythonAccountManager::~CPythonAccountManager()
{
}

void CPythonAccountManager::Initialize(const char* szFileName)
{
	m_stLastAccountName.clear();
	m_vec_AccountInfo.clear();
	m_stFileName = szFileName;

	FILE* pFile;
	if (fopen_s(&pFile, m_stFileName.c_str(), "rb") != 0)
		return;

	static BYTE aesIV[CryptoPP::AES::BLOCKSIZE] = { 0xFF, 0x0, 0xFE, 0x1, 0xFD, 0x2, 0xFC, 0x3, 0xFB, 0x4, 0xFA, 0x5, 0xF9, 0x6, 0xF8, 0x7 };
	CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption aesDecryption((const BYTE*)GetSecurityHWIDHash(), 32, aesIV);

	BYTE bLen;
	char szReadBuf[128];
	char szDecryptBuf[128];

	if (!fread_s(&bLen, sizeof(BYTE), sizeof(BYTE), 1, pFile))
	{
		TraceError("cannot read len of last login name");
		fclose(pFile);
		return;
	}

	if (bLen)
	{
		if (!fread_s(szReadBuf, sizeof(szReadBuf), bLen, 1, pFile))
		{
			TraceError("cannot read last login name");
			fclose(pFile);
			return;
		}
		szReadBuf[bLen] = '\0';

		aesDecryption.ProcessData((BYTE*)szDecryptBuf, (BYTE*)szReadBuf, bLen);
		szDecryptBuf[bLen] = '\0';

		m_stLastAccountName = szDecryptBuf;
	}

	bool bIsAccount;
	while (fread_s(&bIsAccount, sizeof(bool), sizeof(bool), 1, pFile) && bIsAccount)
	{
		TAccountInfo kInfo;
		if (!fread_s(&kInfo.bIndex, sizeof(BYTE), sizeof(BYTE), 1, pFile))
		{
			TraceError("cannot read account index");
			fclose(pFile);
			return;
		}

		if (!fread_s(&bLen, sizeof(BYTE), sizeof(BYTE), 1, pFile))
		{
			TraceError("cannot read length of username of account %u", kInfo.bIndex);
			fclose(pFile);
			return;
		}

		if (!fread_s(szReadBuf, sizeof(szReadBuf), bLen, 1, pFile))
		{
			TraceError("cannot read username of account %u", kInfo.bIndex);
			fclose(pFile);
			return;
		}
		szReadBuf[bLen] = '\0';

		aesDecryption.ProcessData((BYTE*)szDecryptBuf, (BYTE*)szReadBuf, bLen);
		szDecryptBuf[bLen] = '\0';

		strcpy_s(kInfo.szLoginName, szDecryptBuf);

		if (!fread_s(&bLen, sizeof(BYTE), sizeof(BYTE), 1, pFile))
		{
			TraceError("cannot read length of password of account %u", kInfo.bIndex);
			fclose(pFile);
			return;
		}

		if (!fread_s(szReadBuf, sizeof(szReadBuf), bLen, 1, pFile))
		{
			TraceError("cannot read password of account %u", kInfo.bIndex);
			fclose(pFile);
			return;
		}
		szReadBuf[bLen] = '\0';

		aesDecryption.ProcessData((BYTE*)szDecryptBuf, (BYTE*)szReadBuf, bLen);
		szDecryptBuf[bLen] = '\0';

		strcpy_s(kInfo.szPassword, szDecryptBuf);

#ifdef ENABLE_PIN_SYSTEM
		if (!fread_s(&bLen, sizeof(BYTE), sizeof(BYTE), 1, pFile))
		{
			TraceError("cannot read length of pin of account %u", kInfo.bIndex);
			fclose(pFile);
			return;
		}

		if (!fread_s(szReadBuf, sizeof(szReadBuf), bLen, 1, pFile))
		{
			TraceError("cannot read pin of account %u", kInfo.bIndex);
			fclose(pFile);
			return;
		}
		szReadBuf[bLen] = '\0';

		aesDecryption.ProcessData((BYTE*)szDecryptBuf, (BYTE*)szReadBuf, bLen);
		szDecryptBuf[bLen] = '\0';

		strcpy_s(kInfo.szPIN, szDecryptBuf);
#endif

		m_vec_AccountInfo.push_back(kInfo);
	}

	fclose(pFile);
}

void CPythonAccountManager::Destroy()
{
	if (m_stFileName.empty())
		return;

	FILE* pFile;
	if (fopen_s(&pFile, m_stFileName.c_str(), "wb") != 0)
		return;

	static BYTE aesIV[CryptoPP::AES::BLOCKSIZE] = { 0xFF, 0x0, 0xFE, 0x1, 0xFD, 0x2, 0xFC, 0x3, 0xFB, 0x4, 0xFA, 0x5, 0xF9, 0x6, 0xF8, 0x7 };
	CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption aesEncryption((const BYTE*)GetSecurityHWIDHash(), 32, aesIV);

	BYTE bLen;
	char szEncryptBuf[128];

	bLen = static_cast<BYTE>(m_stLastAccountName.length());
	fwrite(&bLen, sizeof(BYTE), 1, pFile);

	if (bLen)
	{
		aesEncryption.ProcessData((BYTE*)szEncryptBuf, (const BYTE*)m_stLastAccountName.c_str(), bLen);
		fwrite(szEncryptBuf, bLen, 1, pFile);
	}

	for (int i = 0; i < m_vec_AccountInfo.size(); ++i)
	{
		bool bIsAccount = true;
		fwrite(&bIsAccount, sizeof(bool), 1, pFile);

		TAccountInfo& kInfo = m_vec_AccountInfo[i];
		fwrite(&kInfo.bIndex, sizeof(BYTE), 1, pFile);

		bLen = static_cast<BYTE>(strlen(kInfo.szLoginName));
		fwrite(&bLen, sizeof(BYTE), 1, pFile);

		aesEncryption.ProcessData((BYTE*)szEncryptBuf, (const BYTE*)kInfo.szLoginName, bLen);
		fwrite(szEncryptBuf, bLen, 1, pFile);

		bLen = static_cast<BYTE>(strlen(kInfo.szPassword));
		fwrite(&bLen, sizeof(BYTE), 1, pFile);

		aesEncryption.ProcessData((BYTE*)szEncryptBuf, (const BYTE*)kInfo.szPassword, bLen);
		fwrite(szEncryptBuf, bLen, 1, pFile);

#ifdef ENABLE_PIN_SYSTEM
		bLen = static_cast<BYTE>(strlen(kInfo.szPIN));
		fwrite(&bLen, sizeof(BYTE), 1, pFile);

		aesEncryption.ProcessData((BYTE*)szEncryptBuf, (const BYTE*)kInfo.szPIN, bLen);
		fwrite(szEncryptBuf, bLen, 1, pFile);
#endif
	}

	bool bIsAccount = false;
	fwrite(&bIsAccount, sizeof(bool), 1, pFile);

	fclose(pFile);

	m_vec_AccountInfo.clear();
	m_stFileName = "";
}

#ifdef ENABLE_PIN_SYSTEM
void CPythonAccountManager::SetAccountInfo(BYTE bIndex, const char* szLoginName, const char* szPassword, const char* szPIN)
#else
void CPythonAccountManager::SetAccountInfo(BYTE bIndex, const char* szLoginName, const char* szPassword)
#endif
{
	for (int i = 0; i < m_vec_AccountInfo.size(); ++i)
	{
		if (bIndex == m_vec_AccountInfo[i].bIndex)
		{
			strcpy_s(m_vec_AccountInfo[i].szLoginName, szLoginName);
			strcpy_s(m_vec_AccountInfo[i].szPassword, szPassword);
#ifdef ENABLE_PIN_SYSTEM
			strcpy_s(m_vec_AccountInfo[i].szPIN, szPIN);
#endif
			return;
		}
	}

	TAccountInfo kInfo;
	kInfo.bIndex = bIndex;
	strcpy_s(kInfo.szLoginName, szLoginName);
	strcpy_s(kInfo.szPassword, szPassword);
#ifdef ENABLE_PIN_SYSTEM
	strcpy_s(kInfo.szPIN, szPIN);
#endif
	m_vec_AccountInfo.push_back(kInfo);
}

void CPythonAccountManager::RemoveAccountInfo(BYTE bIndex)
{
	for (int i = 0; i < m_vec_AccountInfo.size(); ++i)
	{
		if (bIndex == m_vec_AccountInfo[i].bIndex)
		{
			m_vec_AccountInfo.erase(m_vec_AccountInfo.begin() + i);
			return;
		}
	}
}

const CPythonAccountManager::TAccountInfo* CPythonAccountManager::GetAccountInfo(BYTE bIndex)
{
	for (int i = 0; i < m_vec_AccountInfo.size(); ++i)
	{
		if (bIndex == m_vec_AccountInfo[i].bIndex)
			return &m_vec_AccountInfo[i];
	}

	return NULL;
}

PyObject* accmgrInitialize(PyObject* poSelf, PyObject* poArgs)
{
	char* szFileName;
	if (!PyTuple_GetString(poArgs, 0, &szFileName))
		return Py_BadArgument();

	CPythonAccountManager::Instance().Initialize(szFileName);
	return Py_BuildNone();
}

PyObject* accmgrDestroy(PyObject* poSelf, PyObject* poArgs)
{
	CPythonAccountManager::Instance().Destroy();
	return Py_BuildNone();
}

PyObject* accmgrSetLastAccountName(PyObject* poSelf, PyObject* poArgs)
{
	char* szLastAccountName;
	if (!PyTuple_GetString(poArgs, 0, &szLastAccountName))
		return Py_BadArgument();

	CPythonAccountManager::Instance().SetLastAccountName(szLastAccountName);
	return Py_BuildNone();
}

PyObject* accmgrGetLastAccountName(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("s", CPythonAccountManager::Instance().GetLastAccountName().c_str());
}

PyObject* accmgrSetAccountInfo(PyObject* poSelf, PyObject* poArgs)
{
	BYTE bIndex;
	if (!PyTuple_GetByte(poArgs, 0, &bIndex))
		return Py_BadArgument();
	char* szLoginName;
	if (!PyTuple_GetString(poArgs, 1, &szLoginName))
		return Py_BadArgument();
	char* szPassword;
	if (!PyTuple_GetString(poArgs, 2, &szPassword))
		return Py_BadArgument();
#ifdef ENABLE_PIN_SYSTEM
	char* szPIN;
	if (!PyTuple_GetString(poArgs, 3, &szPIN))
		return Py_BadArgument();

	CPythonAccountManager::Instance().SetAccountInfo(bIndex, szLoginName, szPassword, szPIN);
#else
	CPythonAccountManager::Instance().SetAccountInfo(bIndex, szLoginName, szPassword);
#endif
	return Py_BuildNone();
}

PyObject* accmgrRemoveAccountInfo(PyObject* poSelf, PyObject* poArgs)
{
	BYTE bIndex;
	if (!PyTuple_GetByte(poArgs, 0, &bIndex))
		return Py_BadArgument();

	CPythonAccountManager::Instance().RemoveAccountInfo(bIndex);
	return Py_BuildNone();
}

PyObject* accmgrGetAccountInfo(PyObject* poSelf, PyObject* poArgs)
{
	BYTE bIndex;
	if (!PyTuple_GetByte(poArgs, 0, &bIndex))
		return Py_BadArgument();

	const CPythonAccountManager::TAccountInfo* pAccInfo = CPythonAccountManager::Instance().GetAccountInfo(bIndex);
	if (!pAccInfo)
#ifdef ENABLE_PIN_SYSTEM
		return Py_BuildValue("sss", "", "", "");

	return Py_BuildValue("sss", pAccInfo->szLoginName, pAccInfo->szPassword, pAccInfo->szPIN);
#else
		return Py_BuildValue("ss", "", "");

	return Py_BuildValue("ss", pAccInfo->szLoginName, pAccInfo->szPassword);
#endif
}

void initAccountManager()
{
	static PyMethodDef s_methods[] =
	{
		{ "Initialize",			accmgrInitialize,			METH_VARARGS },
		{ "Destroy",			accmgrDestroy,				METH_VARARGS },

		{ "SetLastAccountName",	accmgrSetLastAccountName,	METH_VARARGS },
		{ "GetLastAccountName",	accmgrGetLastAccountName,	METH_VARARGS },

		{ "SetAccountInfo",		accmgrSetAccountInfo,		METH_VARARGS },
		{ "RemoveAccountInfo",	accmgrRemoveAccountInfo,	METH_VARARGS },
		{ "GetAccountInfo",		accmgrGetAccountInfo,		METH_VARARGS },

		{ NULL, NULL, NULL },
	};

	PyObject* poModule = Py_InitModule("accmgr", s_methods);
}
#endif