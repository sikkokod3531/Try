#pragma once
#include "desc.h"

class CLIENT_DESC : public DESC
{
public:
	CLIENT_DESC();
	virtual ~CLIENT_DESC();

	virtual BYTE	GetType() { return DESC_TYPE_CONNECTOR; }
	virtual void	Destroy();
	virtual void	SetPhase(int phase);

	bool 		Connect(int iPhaseWhenSucceed = 0);
	void		Setup(LPFDWATCH _fdw, const char* _host, WORD _port);

	void		SetRetryWhenClosed(bool);

	void		DBPacketHeader(BYTE bHeader, DWORD dwHandle, DWORD dwSize);
	void		DBPacket(BYTE bHeader, DWORD dwHandle, const void* c_pvData, DWORD dwSize);
	void		Packet(const void* c_pvData, int iSize);
	bool		IsRetryWhenClosed();

	void		Update(DWORD t);
	void		UpdateChannelStatus(DWORD t, bool fForce);

	// Non-destructive close for reuse
	void Reset();

#ifdef ENABLE_FULL_SYSTEM
	bool		ResetForces();

	bool		SetForceNorm(bool f);
	bool		IsForceNorm();

	bool		SetForceBusy(bool f);
	bool		IsForceBusy();

	bool		SetForceFull(bool f);
	bool		IsForceFull();
#endif // ENABLE_FULL_SYSTEM

private:
	void InitializeBuffers();

protected:
	int			m_iPhaseWhenSucceed;
	bool		m_bRetryWhenClosed;
	time_t		m_LastTryToConnectTime;
	time_t		m_tLastChannelStatusUpdateTime;

#ifdef ENABLE_FULL_SYSTEM
	bool		m_bForceNorm;
	bool		m_bForceBusy;
	bool		m_bForceFull;
#endif // ENABLE_FULL_SYSTEM

	CInputDB 	m_inputDB;
	CInputP2P 	m_inputP2P;
};

extern LPCLIENT_DESC db_clientdesc;
extern LPCLIENT_DESC g_pkAuthMasterDesc;