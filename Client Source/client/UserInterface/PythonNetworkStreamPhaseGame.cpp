#include "StdAfx.h"
#include "PythonNetworkStream.h"
#include "Packet.h"

#include "PythonGuild.h"
#include "PythonCharacterManager.h"
#include "PythonPlayer.h"
#include "PythonBackground.h"
#include "PythonMiniMap.h"
#include "PythonTextTail.h"
#include "PythonItem.h"
#include "PythonChat.h"
#include "PythonShop.h"
#include "PythonExchange.h"
#include "PythonQuest.h"
#include "PythonEventManager.h"
#include "PythonMessenger.h"
#include "PythonApplication.h"

#include "../EterPack/EterPackManager.h"
#include "../gamelib/ItemManager.h"

#include "AbstractApplication.h"
#include "AbstractCharacterManager.h"
#include "InstanceBase.h"

#ifdef ENABLE_EVENT_SYSTEM
#include "PythonGameEvents.h"
#endif

#ifdef ENABLE_CUBE_RENEWAL
#include "PythonCubeRenewal.h"
#endif

#ifdef USE_DISCORD_RPC_MODULE
#include "DiscordManager.h"
#endif

#include <intrin.h>
#ifdef ENABLE_ANTICHEAT
#include <rascal.h>
#endif
#ifdef ENABLE_CHAT_DISCREDIT
bool CPythonNetworkStream::LoadDiscreditFile(const char* szFileName)
{
	CMappedFile File;
	LPCVOID pData;

	if (!CEterPackManager::Instance().Get(File, szFileName, &pData))
		return false;

	CMemoryTextFileLoader textFileLoader;
	textFileLoader.Bind(File.Size(), pData);

	CTokenVector TokenVector;

	if (!textFileLoader.SplitLine(0, &TokenVector, "\t"))
		return false;

	if (TokenVector.size() != 1)
		return false;

	/**
	 * 0: closed
	 * 1: chage with txt file
	 * 2: chage with *
	 * 3: chage with space
	 * 4: dont send
	 */
	m_discreditMode = atoi(TokenVector[0].c_str());

	for (DWORD i = 1; i < textFileLoader.GetLineCount(); ++i)
	{
		if (!textFileLoader.SplitLine(i, &TokenVector, "\t"))
			continue;

		if (TokenVector.size() != 2)
		{
			TraceError("LoadDiscreditFile %s - line %d - wrong size(excepted %d gived %d)", szFileName, i, 2, TokenVector.size());
			continue;
		}

		const std::string& c_rstrSrc = TokenVector[0];
		const std::string& c_rstrTarget = TokenVector[1];

		m_DiscreditMap[c_rstrSrc] = c_rstrTarget;
	}

	// TraceError("LoadDiscreditFile %s - completed - size %d", szFileName, m_DiscreditMap.size()); // todo::

	return true;
}

#include <regex>
#include <boost/range/adaptor/reversed.hpp>
void CPythonNetworkStream::ControlDiscredit(std::string& strchat)
{
	// if (CPythonCharacterManager::Instance().GetMainInstancePtr()->IsGameMaster())
	// 	return;

	switch (m_discreditMode)
	{
		case 1:
		{
			for (const auto& kv : boost::adaptors::reverse(m_DiscreditMap))
			{
				size_t start_pos = strchat.find(kv.first);
				if(start_pos == std::string::npos)
					continue;

				if(start_pos + kv.first.length() == strchat.length() || strchat.at(start_pos + kv.first.length()) == ' ')
				{
					strchat.replace(start_pos, kv.first.length(), kv.second);
					break;
				}
			}

			break;
		}
		case 2:
		{
			for (const auto& kv : boost::adaptors::reverse(m_DiscreditMap))
				strchat = std::regex_replace(strchat, std::regex(kv.first), "*");
			break;
		}
		case 3:
		{
			for (const auto& kv : boost::adaptors::reverse(m_DiscreditMap))
				strchat = std::regex_replace(strchat, std::regex(kv.first), " ");
			break;
		}
		case 4:
		{
		}

		case 0:
		default:
			break;
	}
}
#endif

void CPythonNetworkStream::__RefreshAlignmentWindow()
{
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshAlignment", Py_BuildValue("()"));
}

void CPythonNetworkStream::__RefreshTargetBoardByVID(DWORD dwVID)
{
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshTargetBoardByVID", Py_BuildValue("(i)", dwVID));
}

void CPythonNetworkStream::__RefreshTargetBoardByName(const char* c_szName)
{
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshTargetBoardByName", Py_BuildValue("(s)", c_szName));
}

void CPythonNetworkStream::__RefreshTargetBoard()
{
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshTargetBoard", Py_BuildValue("()"));
}

void CPythonNetworkStream::__RefreshCostumeWindow()
{
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshCostumeWindow", Py_BuildValue("()"));
}

void CPythonNetworkStream::__RefreshGameOptionDlg()
{
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshGameOptionDlg", Py_BuildValue("()"));
}

void CPythonNetworkStream::__RefreshGuildWindowGradePage()
{
	m_isRefreshGuildWndGradePage = true;
}

void CPythonNetworkStream::__RefreshGuildWindowSkillPage()
{
	m_isRefreshGuildWndSkillPage = true;
}

void CPythonNetworkStream::__RefreshGuildWindowMemberPageGradeComboBox()
{
	m_isRefreshGuildWndMemberPageGradeComboBox = true;
}

void CPythonNetworkStream::__RefreshGuildWindowMemberPage()
{
	m_isRefreshGuildWndMemberPage = true;
}

void CPythonNetworkStream::__RefreshGuildWindowBoardPage()
{
	m_isRefreshGuildWndBoardPage = true;
}

void CPythonNetworkStream::__RefreshGuildWindowInfoPage()
{
	m_isRefreshGuildWndInfoPage = true;
}

void CPythonNetworkStream::__RefreshMessengerWindow()
{
	m_isRefreshMessengerWnd = true;
}

void CPythonNetworkStream::__RefreshSafeboxWindow()
{
	m_isRefreshSafeboxWnd = true;
}

void CPythonNetworkStream::__RefreshMallWindow()
{
	m_isRefreshMallWnd = true;
}

void CPythonNetworkStream::__RefreshSkillWindow()
{
	m_isRefreshSkillWnd = true;
}

void CPythonNetworkStream::__RefreshExchangeWindow()
{
	m_isRefreshExchangeWnd = true;
}

void CPythonNetworkStream::__RefreshStatus()
{
	m_isRefreshStatus = true;
}

void CPythonNetworkStream::__RefreshCharacterWindow()
{
	m_isRefreshCharacterWnd = true;
}

#ifdef ENABLE_ADDITIONAL_INVENTORY
void CPythonNetworkStream::__RefreshInventoryWindow(BYTE bWindowType)
{
	if (bWindowType > 0)
	{
		if (bWindowType == INVENTORY)
			m_isRefreshInventoryWnd = true;
		else if (bWindowType >= UPGRADE_INVENTORY && bWindowType <= CHEST_INVENTORY)
			m_isRefreshAdditionalInventoryWnd = true;
		else
			m_isRefreshOtherWnd = true;
	}
	else
	{
		m_isRefreshInventoryWnd = true;
	}
}
#else
void CPythonNetworkStream::__RefreshInventoryWindow()
{
	m_isRefreshInventoryWnd = true;
}
#endif

void CPythonNetworkStream::__RefreshEquipmentWindow()
{
	m_isRefreshEquipmentWnd = true;
}

void CPythonNetworkStream::__SetGuildID(DWORD id)
{
	if (m_dwGuildID != id)
	{
		m_dwGuildID = id;
		IAbstractPlayer& rkPlayer = IAbstractPlayer::GetSingleton();

		for (int i = 0; i < PLAYER_PER_ACCOUNT4; ++i)
			if (!strncmp(m_akSimplePlayerInfo[i].szName, rkPlayer.GetName(), CHARACTER_NAME_MAX_LEN))
			{
				m_adwGuildID[i] = id;

				std::string  guildName;
				if (CPythonGuild::Instance().GetGuildName(id, &guildName))
				{
					m_astrGuildName[i] = guildName;
				}
				else
				{
					m_astrGuildName[i] = "";
				}
			}
	}
}

struct PERF_PacketInfo
{
	DWORD dwCount;
	DWORD dwTime;

	PERF_PacketInfo()
	{
		dwCount = 0;
		dwTime = 0;
	}
};

// Game Phase ---------------------------------------------------------------------------
void CPythonNetworkStream::GamePhase()
{
	if (!m_kQue_stHack.empty())
	{
		__SendHack(m_kQue_stHack.front().c_str());
		m_kQue_stHack.pop_front();
	}

	TPacketHeader header = 0;
	bool ret = true;

	const DWORD MAX_RECV_COUNT = 32;
	const DWORD SAFE_RECV_BUFSIZE = 8192;
	DWORD dwRecvCount = 0;

	while (ret)
	{
		if (dwRecvCount++ >= MAX_RECV_COUNT - 1 && GetRecvBufferSize() < SAFE_RECV_BUFSIZE && m_strPhase == "Game")
			break;


		if (!CheckPacket(&header))
			break;

#if defined(ENABLE_PRINT_RECV_PACKET_DEBUG)
		TraceError("RECV HEADER : %u , phase %s ", header, m_strPhase.c_str());
#endif

		switch (header)
		{
		case HEADER_GC_WARP:
			ret = RecvWarpPacket();
			break;

		case HEADER_GC_PHASE:
			ret = RecvPhasePacket();
			return;
			break;

		case HEADER_GC_PVP:
			ret = RecvPVPPacket();
			break;

		case HEADER_GC_CHARACTER_ADD:
			ret = RecvCharacterAppendPacket();
			break;

		case HEADER_GC_CHAR_ADDITIONAL_INFO:
			ret = RecvCharacterAdditionalInfo();
			break;

		case HEADER_GC_CHARACTER_UPDATE:
			ret = RecvCharacterUpdatePacket();
			break;

		case HEADER_GC_CHARACTER_DEL:
			ret = RecvCharacterDeletePacket();
			break;

		case HEADER_GC_CHAT:
			ret = RecvChatPacket();
			break;

		case HEADER_GC_SYNC_POSITION:
			ret = RecvSyncPositionPacket();
			break;

		case HEADER_GC_OWNERSHIP:
			ret = RecvOwnerShipPacket();
			break;

		case HEADER_GC_WHISPER:
			ret = RecvWhisperPacket();
			break;

		case HEADER_GC_CHARACTER_MOVE:
			ret = RecvCharacterMovePacket();
			break;

			// Position
		case HEADER_GC_CHARACTER_POSITION:
			ret = RecvCharacterPositionPacket();
			break;

			// Battle Packet
		case HEADER_GC_STUN:
			ret = RecvStunPacket();
			break;

		case HEADER_GC_DEAD:
			ret = RecvDeadPacket();
			break;

		case HEADER_GC_PLAYER_POINT_CHANGE:
			ret = RecvPointChange();
			break;

			// item packet.
		case HEADER_GC_ITEM_DEL:
			ret = RecvItemDelPacket();
			break;

		case HEADER_GC_ITEM_SET:
			ret = RecvItemSetPacket();
			break;

		case HEADER_GC_ITEM_USE:
			ret = RecvItemUsePacket();
			break;

		case HEADER_GC_ITEM_UPDATE:
			ret = RecvItemUpdatePacket();
			break;

		case HEADER_GC_ITEM_GROUND_ADD:
			ret = RecvItemGroundAddPacket();
			break;

		case HEADER_GC_ITEM_GROUND_DEL:
			ret = RecvItemGroundDelPacket();
			break;

		case HEADER_GC_ITEM_OWNERSHIP:
			ret = RecvItemOwnership();
			break;

		case HEADER_GC_QUICKSLOT_ADD:
			ret = RecvQuickSlotAddPacket();
			break;

		case HEADER_GC_QUICKSLOT_DEL:
			ret = RecvQuickSlotDelPacket();
			break;

		case HEADER_GC_QUICKSLOT_SWAP:
			ret = RecvQuickSlotMovePacket();
			break;

		case HEADER_GC_MOTION:
			ret = RecvMotionPacket();
			break;

		case HEADER_GC_SHOP:
			ret = RecvShopPacket();
			break;

		case HEADER_GC_SHOP_SIGN:
			ret = RecvShopSignPacket();
			break;

		case HEADER_GC_EXCHANGE:
			ret = RecvExchangePacket();
			break;

		case HEADER_GC_QUEST_INFO:
			ret = RecvQuestInfoPacket();
			break;

		case HEADER_GC_REQUEST_MAKE_GUILD:
			ret = RecvRequestMakeGuild();
			break;

		case HEADER_GC_PING:
			ret = RecvPingPacket();
			break;

		case HEADER_GC_SCRIPT:
			ret = RecvScriptPacket();
			break;

		case HEADER_GC_QUEST_CONFIRM:
			ret = RecvQuestConfirmPacket();
			break;

		case HEADER_GC_TARGET:
			ret = RecvTargetPacket();
			break;

		case HEADER_GC_DAMAGE_INFO:
			ret = RecvDamageInfoPacket();
			break;

		case HEADER_GC_PLAYER_POINTS:
			ret = __RecvPlayerPoints();
			break;

		case HEADER_GC_CREATE_FLY:
			ret = RecvCreateFlyPacket();
			break;

		case HEADER_GC_FLY_TARGETING:
			ret = RecvFlyTargetingPacket();
			break;

		case HEADER_GC_ADD_FLY_TARGETING:
			ret = RecvAddFlyTargetingPacket();
			break;

		case HEADER_GC_SKILL_LEVEL_NEW:
			ret = RecvSkillLevelNew();
			break;

		case HEADER_GC_MESSENGER:
			ret = RecvMessenger();
			break;

		case HEADER_GC_GUILD:
			ret = RecvGuild();
			break;

		case HEADER_GC_PARTY_INVITE:
			ret = RecvPartyInvite();
			break;

		case HEADER_GC_PARTY_ADD:
			ret = RecvPartyAdd();
			break;

		case HEADER_GC_PARTY_UPDATE:
			ret = RecvPartyUpdate();
			break;

		case HEADER_GC_PARTY_REMOVE:
			ret = RecvPartyRemove();
			break;

		case HEADER_GC_PARTY_LINK:
			ret = RecvPartyLink();
			break;

		case HEADER_GC_PARTY_UNLINK:
			ret = RecvPartyUnlink();
			break;

		case HEADER_GC_PARTY_PARAMETER:
			ret = RecvPartyParameter();
			break;

		case HEADER_GC_SAFEBOX_SET:
			ret = RecvSafeBoxSetPacket();
			break;

		case HEADER_GC_SAFEBOX_DEL:
			ret = RecvSafeBoxDelPacket();
			break;

		case HEADER_GC_SAFEBOX_WRONG_PASSWORD:
			ret = RecvSafeBoxWrongPasswordPacket();
			break;

		case HEADER_GC_SAFEBOX_SIZE:
			ret = RecvSafeBoxSizePacket();
			break;

		case HEADER_GC_FISHING:
			ret = RecvFishing();
			break;

		case HEADER_GC_DUNGEON:
			ret = RecvDungeon();
			break;

		case HEADER_GC_TIME:
			ret = RecvTimePacket();
			break;

		case HEADER_GC_WALK_MODE:
			ret = RecvWalkModePacket();
			break;

		case HEADER_GC_CHANGE_SKILL_GROUP:
			ret = RecvChangeSkillGroupPacket();
			break;

		case HEADER_GC_REFINE_INFORMATION_NEW:
			ret = RecvRefineInformationPacketNew();
			break;

		case HEADER_GC_SEPCIAL_EFFECT:
			ret = RecvSpecialEffect();
			break;

		case HEADER_GC_CHANNEL:
			ret = RecvChannelPacket();
			break;

		case HEADER_GC_TARGET_CREATE_NEW:
			ret = RecvTargetCreatePacketNew();
			break;

		case HEADER_GC_TARGET_UPDATE:
			ret = RecvTargetUpdatePacket();
			break;

		case HEADER_GC_TARGET_DELETE:
			ret = RecvTargetDeletePacket();
			break;

		case HEADER_GC_AFFECT_ADD:
			ret = RecvAffectAddPacket();
			break;

		case HEADER_GC_AFFECT_REMOVE:
			ret = RecvAffectRemovePacket();
			break;

		case HEADER_GC_MALL_OPEN:
			ret = RecvMallOpenPacket();
			break;

		case HEADER_GC_MALL_SET:
			ret = RecvMallItemSetPacket();
			break;

		case HEADER_GC_MALL_DEL:
			ret = RecvMallItemDelPacket();
			break;

		case HEADER_GC_DIG_MOTION:
			ret = RecvDigMotionPacket();
			break;

		case HEADER_GC_HANDSHAKE:
			RecvHandshakePacket();
			return;
			break;

		case HEADER_GC_HANDSHAKE_OK:
			RecvHandshakeOKPacket();
			return;
			break;

#ifdef ENABLE_IMPROVED_PACKET_ENCRYPTION
		case HEADER_GC_KEY_AGREEMENT:
			RecvKeyAgreementPacket();
			return;
			break;

		case HEADER_GC_KEY_AGREEMENT_COMPLETED:
			RecvKeyAgreementCompletedPacket();
			return;
			break;
#endif

		case HEADER_GC_SPECIFIC_EFFECT:
			ret = RecvSpecificEffect();
			break;

#ifdef ENABLE_OFFLINE_SHOP
		case HEADER_GC_NEW_OFFLINESHOP:
			ret = RecvOfflineshopPacket();
			break;
#endif

		case HEADER_GC_DRAGON_SOUL_REFINE:
			ret = RecvDragonSoulRefine();
			break;

#ifdef ENABLE_ACCE_SYSTEM
		case HEADER_GC_ACCE:
			ret = RecvAccePacket();
			break;
#endif
#ifdef ENABLE_CHEST_INFO_SYSTEM
		case HEADER_GC_CHEST_DROP_INFO:
			ret = RecvChestDropInfo();
			break;
#endif
#ifdef ENABLE_GLOBAL_MESSAGE_UTILITY
		case HEADER_GC_BULK_WHISPER:
			ret = RecvBulkWhisperPacket();
			break;
#endif
#ifdef ENABLE_EVENT_SYSTEM
		case HEADER_GC_EVENT_INFO:
			ret = RecvEventInfo();
			break;
#endif
#ifdef ENABLE_SKILL_CHOOSE_SYSTEM
		case HEADER_GC_SKILLCHOOSE:
			ret = RecvSkillChoose();
			break;
#endif
#ifdef ENABLE_CHANGELOOK_SYSTEM
		case HEADER_GC_CL:
			ret = RecvChangeLookPacket();
			break;
#endif
#ifdef ENABLE_SWITCHBOT_SYSTEM
		case HEADER_GC_SWITCHBOT:
			ret = RecvSwitchbotPacket();
			break;
#endif
#ifdef ENABLE_CUBE_RENEWAL
		case HEADER_GC_CUBE_RENEWAL:
			ret = RecvCubeRenewalPacket();
			break;
#endif
#ifdef ENABLE_PLAYER_STATISTICS
		case HEADER_GC_PLAYER_STATISTICS:
			ret = RecvPlayerStatistics();
			break;
#endif
#ifdef ENABLE_INGAME_MALL_SYSTEM
		case HEADER_GC_ITEM_SHOP:
			ret = RecvItemShopData();
			break;
#endif // ENABLE_INGAME_MALL_SYSTEM
#ifdef ENABLE_TARGET_BOARD_RENEWAL
		case HEADER_GC_MOB_INFO:
			ret = RecvMobInfo();
			break;
#endif

#ifdef ENABLE_BATTLE_PASS_SYSTEM
		case HEADER_GC_BATTLE_PASS_OPEN:
			ret = RecvBattlePassPacket();
			break;

		case HEADER_GC_BATTLE_PASS_UPDATE:
			ret = RecvBattlePassUpdatePacket();
			break;

		case HEADER_GC_BATTLE_PASS_RANKING:
			ret = RecvBattlePassRankingPacket();
			break;
#endif
		case HEADER_GC_UNK_213: // @fixme007
			ret = RecvUnk213();
			break;

		default:
			ret = RecvDefaultPacket(header);
			break;
		}
	}

	if (!ret)
		RecvErrorPacket(header);

	static DWORD s_nextRefreshTime = ELTimer_GetMSec();

	DWORD curTime = ELTimer_GetMSec();
	if (s_nextRefreshTime > curTime)
		return;

	if (m_isRefreshCharacterWnd)
	{
		m_isRefreshCharacterWnd = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshCharacter", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshEquipmentWnd)
	{
		m_isRefreshEquipmentWnd = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshEquipment", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshInventoryWnd)
	{
		m_isRefreshInventoryWnd = false;
#ifdef ENABLE_ADDITIONAL_INVENTORY
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshInventory", Py_BuildValue("(s)", "ALL"));
#else
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshInventory", Py_BuildValue("()"));
#endif
		s_nextRefreshTime = curTime + 300;
	}

#ifdef ENABLE_ADDITIONAL_INVENTORY
	if (m_isRefreshAdditionalInventoryWnd)
	{
		m_isRefreshAdditionalInventoryWnd = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshInventory", Py_BuildValue("(s)", "ONLY_ADDITIONAL"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshOtherWnd)
	{
		m_isRefreshOtherWnd = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshInventory", Py_BuildValue("(s)", "OTHER"));
		s_nextRefreshTime = curTime + 300;
	}
#endif

	if (m_isRefreshExchangeWnd)
	{
		m_isRefreshExchangeWnd = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshExchange", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshSkillWnd)
	{
		m_isRefreshSkillWnd = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshSkill", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshSafeboxWnd)
	{
		m_isRefreshSafeboxWnd = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshSafebox", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshMallWnd)
	{
		m_isRefreshMallWnd = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshMall", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshStatus)
	{
		m_isRefreshStatus = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshStatus", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshMessengerWnd)
	{
		m_isRefreshMessengerWnd = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshMessenger", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshGuildWndInfoPage)
	{
		m_isRefreshGuildWndInfoPage = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshGuildInfoPage", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshGuildWndBoardPage)
	{
		m_isRefreshGuildWndBoardPage = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshGuildBoardPage", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshGuildWndMemberPage)
	{
		m_isRefreshGuildWndMemberPage = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshGuildMemberPage", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshGuildWndMemberPageGradeComboBox)
	{
		m_isRefreshGuildWndMemberPageGradeComboBox = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshGuildMemberPageGradeComboBox", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshGuildWndSkillPage)
	{
		m_isRefreshGuildWndSkillPage = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshGuildSkillPage", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshGuildWndGradePage)
	{
		m_isRefreshGuildWndGradePage = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshGuildGradePage", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}
}

void CPythonNetworkStream::__InitializeGamePhase()
{
	__ServerTimeSync_Initialize();

	m_isRefreshStatus = false;
	m_isRefreshCharacterWnd = false;
	m_isRefreshEquipmentWnd = false;
	m_isRefreshInventoryWnd = false;
	m_isRefreshExchangeWnd = false;
	m_isRefreshSkillWnd = false;
	m_isRefreshSafeboxWnd = false;
	m_isRefreshMallWnd = false;
	m_isRefreshMessengerWnd = false;
	m_isRefreshGuildWndInfoPage = false;
	m_isRefreshGuildWndBoardPage = false;
	m_isRefreshGuildWndMemberPage = false;
	m_isRefreshGuildWndMemberPageGradeComboBox = false;
	m_isRefreshGuildWndSkillPage = false;
	m_isRefreshGuildWndGradePage = false;

	m_EmoticonStringVector.clear();

	m_pInstTarget = NULL;
}

void CPythonNetworkStream::Warp(LONG lGlobalX, LONG lGlobalY)
{
	CPythonBackground& rkBgMgr = CPythonBackground::Instance();
	rkBgMgr.Destroy();
	rkBgMgr.Create();
	rkBgMgr.Warp(lGlobalX, lGlobalY);
	//rkBgMgr.SetShadowLevel(CPythonBackground::SHADOW_ALL);
	rkBgMgr.RefreshShadowLevel();

	LONG lLocalX = lGlobalX;
	LONG lLocalY = lGlobalY;
	__GlobalPositionToLocalPosition(lLocalX, lLocalY);
	float fHeight = CPythonBackground::Instance().GetHeight(float(lLocalX), float(lLocalY));

	IAbstractApplication& rkApp = IAbstractApplication::GetSingleton();
	rkApp.SetCenterPosition(float(lLocalX), float(lLocalY), fHeight);

	__ShowMapName(lLocalX, lLocalY);
}

void CPythonNetworkStream::__ShowMapName(LONG lLocalX, LONG lLocalY)
{
	const std::string& c_rstrMapFileName = CPythonBackground::Instance().GetWarpMapName();
#ifdef ENABLE_MAP_ALGORITHM_RENEWAL
	const std::string & c_rstrRealMapName = CPythonBackground::Instance().GetRealMapName(c_rstrMapFileName);
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ShowMapName", Py_BuildValue("(siis)", c_rstrMapFileName.c_str(), lLocalX, lLocalY, c_rstrRealMapName.c_str()));
#else
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ShowMapName", Py_BuildValue("(sii)", c_rstrMapFileName.c_str(), lLocalX, lLocalY));
#endif
}

void CPythonNetworkStream::__LeaveGamePhase()
{
	CInstanceBase::ClearPVPKeySystem();

	__ClearNetworkActorManager();

	m_bComboSkillFlag = FALSE;

	IAbstractCharacterManager& rkChrMgr = IAbstractCharacterManager::GetSingleton();
	rkChrMgr.Destroy();

	CPythonItem& rkItemMgr = CPythonItem::Instance();
	rkItemMgr.Destroy();

#ifdef ENABLE_OFFLINE_SHOP
#ifdef ENABLE_OFFLINE_SHOP_CITIES
	CPythonOfflineshop::instance().DeleteEntities();
#endif
#endif

#ifdef USE_DISCORD_RPC_MODULE
	CDiscordManager::Instance().UpdatePresence(CDiscordManager::EDiscordPresenceStatus::Discord_Status_Closed, 0);
#endif
}

void CPythonNetworkStream::SetGamePhase()
{
	if ("Game" != m_strPhase)
		m_phaseLeaveFunc.Run();

	Tracen("");
	Tracen("## Network - Game Phase ##");
	Tracen("");

	m_strPhase = "Game";


	m_dwChangingPhaseTime = ELTimer_GetMSec();
	m_phaseProcessFunc.Set(this, &CPythonNetworkStream::GamePhase);
	m_phaseLeaveFunc.Set(this, &CPythonNetworkStream::__LeaveGamePhase);

	IAbstractPlayer& rkPlayer = IAbstractPlayer::GetSingleton();
	rkPlayer.SetMainCharacterIndex(GetMainActorVID());
	__RefreshStatus();

#ifdef USE_DISCORD_RPC_MODULE
	CDiscordManager::Instance().UpdatePresence(CDiscordManager::EDiscordPresenceStatus::Discord_Status_PVM, GetMainActorVID());
#endif
}

bool CPythonNetworkStream::RecvUnk213() // @fixme007
{
	TPacketGCUnk213 kUnk213Packet;
	if (!Recv(sizeof(TPacketGCUnk213)), &kUnk213Packet)
		return false;
	return true;
}

bool CPythonNetworkStream::RecvWarpPacket()
{
	TPacketGCWarp kWarpPacket;

	if (!Recv(sizeof(kWarpPacket), &kWarpPacket))
		return false;
	__DirectEnterMode_Set(m_dwSelectedCharacterIndex);

	CNetworkStream::Connect((DWORD)kWarpPacket.lAddr, kWarpPacket.wPort);

	return true;
}

bool CPythonNetworkStream::RecvPVPPacket()
{
	TPacketGCPVP kPVPPacket;
	if (!Recv(sizeof(kPVPPacket), &kPVPPacket))
		return false;

	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();

	switch (kPVPPacket.bMode)
	{
	case PVP_MODE_AGREE:
		rkChrMgr.RemovePVPKey(kPVPPacket.dwVIDSrc, kPVPPacket.dwVIDDst);

		if (rkPlayer.IsMainCharacterIndex(kPVPPacket.dwVIDDst))
			rkPlayer.RememberChallengeInstance(kPVPPacket.dwVIDSrc);

		if (rkPlayer.IsMainCharacterIndex(kPVPPacket.dwVIDSrc))
			rkPlayer.RememberCantFightInstance(kPVPPacket.dwVIDDst);
		break;
	case PVP_MODE_REVENGE:
	{
		rkChrMgr.RemovePVPKey(kPVPPacket.dwVIDSrc, kPVPPacket.dwVIDDst);

		DWORD dwKiller = kPVPPacket.dwVIDSrc;
		DWORD dwVictim = kPVPPacket.dwVIDDst;

		if (rkPlayer.IsMainCharacterIndex(dwVictim))
			rkPlayer.RememberRevengeInstance(dwKiller);

		if (rkPlayer.IsMainCharacterIndex(dwKiller))
			rkPlayer.RememberCantFightInstance(dwVictim);
		break;
	}

	case PVP_MODE_FIGHT:
		rkChrMgr.InsertPVPKey(kPVPPacket.dwVIDSrc, kPVPPacket.dwVIDDst);
		rkPlayer.ForgetInstance(kPVPPacket.dwVIDSrc);
		rkPlayer.ForgetInstance(kPVPPacket.dwVIDDst);
		break;
	case PVP_MODE_NONE:
		rkChrMgr.RemovePVPKey(kPVPPacket.dwVIDSrc, kPVPPacket.dwVIDDst);
		rkPlayer.ForgetInstance(kPVPPacket.dwVIDSrc);
		rkPlayer.ForgetInstance(kPVPPacket.dwVIDDst);
		break;
	}

	__RefreshTargetBoardByVID(kPVPPacket.dwVIDSrc);
	__RefreshTargetBoardByVID(kPVPPacket.dwVIDDst);

	return true;
}

// DELETEME

void CPythonNetworkStream::NotifyHack(const char* c_szMsg)
{
	if (!m_kQue_stHack.empty())
		if (c_szMsg == m_kQue_stHack.back())
			return;

	m_kQue_stHack.push_back(c_szMsg);
}

bool CPythonNetworkStream::__SendHack(const char* c_szMsg)
{
	Tracen(c_szMsg);

	TPacketCGHack kPacketHack;
	kPacketHack.bHeader = HEADER_CG_HACK;
	strncpy(kPacketHack.szBuf, c_szMsg, sizeof(kPacketHack.szBuf) - 1);

	if (!Send(sizeof(kPacketHack), &kPacketHack))
		return false;

	return true;
}

bool CPythonNetworkStream::SendMessengerAddByVIDPacket(DWORD vid)
{
	TPacketCGMessenger packet;
	packet.header = HEADER_CG_MESSENGER;
	packet.subheader = MESSENGER_SUBHEADER_CG_ADD_BY_VID;
	if (!Send(sizeof(packet), &packet))
		return false;
	if (!Send(sizeof(vid), &vid))
		return false;
	return true;
}

#ifdef ENABLE_GROWTH_PET_SYSTEM
bool CPythonNetworkStream::PetSetNamePacket(const char* petname)
{
	TPacketCGRequestPetName PetSetName;
	PetSetName.byHeader = HEADER_CG_PetSetName;
	strncpy(PetSetName.petname, petname, 12);
	PetSetName.petname[12] = '\0';

	if (!Send(sizeof(PetSetName), &PetSetName))
		return false;

	return true;
}
#endif

#ifdef ENABLE_GROWTH_MOUNT_SYSTEM
bool CPythonNetworkStream::MountSetNamePacket(const char* mountname)
{
	TPacketCGRequestPetName MountSetName;
	MountSetName.byHeader = HEADER_CG_MountSetName;
	strncpy(MountSetName.petname, mountname, 12);
	MountSetName.petname[12] = '\0';

	if (!Send(sizeof(MountSetName), &MountSetName))
		return false;

	return true;
}
#endif

bool CPythonNetworkStream::SendMessengerAddByNamePacket(const char* c_szName)
{
	TPacketCGMessenger packet;
	packet.header = HEADER_CG_MESSENGER;
	packet.subheader = MESSENGER_SUBHEADER_CG_ADD_BY_NAME;
	if (!Send(sizeof(packet), &packet))
		return false;
	char szName[CHARACTER_NAME_MAX_LEN];
	strncpy(szName, c_szName, CHARACTER_NAME_MAX_LEN - 1);
	szName[CHARACTER_NAME_MAX_LEN - 1] = '\0';

	if (!Send(sizeof(szName), &szName))
		return false;
	Tracef(" SendMessengerAddByNamePacket : %s\n", c_szName);
	return true;
}

bool CPythonNetworkStream::SendMessengerRemovePacket(const char* c_szKey, const char* c_szName)
{
	TPacketCGMessenger packet;
	packet.header = HEADER_CG_MESSENGER;
	packet.subheader = MESSENGER_SUBHEADER_CG_REMOVE;
	if (!Send(sizeof(packet), &packet))
		return false;
	char szKey[CHARACTER_NAME_MAX_LEN];
	strncpy(szKey, c_szKey, CHARACTER_NAME_MAX_LEN - 1);
	if (!Send(sizeof(szKey), &szKey))
		return false;
	__RefreshTargetBoardByName(c_szName);
	return true;
}

bool CPythonNetworkStream::SendCharacterStatePacket(const TPixelPosition& c_rkPPosDst, float fDstRot, UINT eFunc, UINT uArg)
{
	if (!__CanActMainInstance())
		return true;

	if (fDstRot < 0.0f)
		fDstRot = 360 + fDstRot;
	else if (fDstRot > 360.0f)
		fDstRot = fmodf(fDstRot, 360.0f);

	TPacketCGMove kStatePacket;
	kStatePacket.bHeader = HEADER_CG_CHARACTER_MOVE;
	kStatePacket.bFunc = eFunc;
	kStatePacket.bArg = uArg;
	kStatePacket.bRot = fDstRot / 5.0f;
	kStatePacket.lX = long(c_rkPPosDst.x);
	
    kStatePacket.lY = long(c_rkPPosDst.y);
	{
		CInstanceBase * pkInst = CPythonCharacterManager::Instance().GetMainInstancePtr();
		if (eFunc == 3 && uArg > 21){ return true; }
		 
		static UINT c_packet = 0;
		static DWORD t_packet = 0;
		DWORD t = timeGetTime();
		if (t_packet + 500 < t) {
			c_packet = 0;
			t_packet = t;
		}
		c_packet++;
		if (c_packet > 8){ return true; }
		 
		static UINT l_func = 0;
		static DWORD t_f = 0;
		 
		static float f_x = 0.0f;
		static float f_y = 0.0f;
		const D3DXVECTOR3 & cpos = pkInst->GetGraphicThingInstancePtr()->GetPosition();
		if (f_x == cpos.x && f_y == cpos.y && t - t_f < 1000 && l_func == eFunc && (eFunc < 3)) {
			return true;
		}
		f_x = cpos.x;
		f_y = cpos.y;
		 
		if (l_func == 0 && eFunc == 0 && t_f + 10 > t){ return true; }
		l_func = eFunc;
		t_f = t;
		if (cpos.x == 0.0f && cpos.y == -0.0f) {
			kStatePacket.lX = long(c_rkPPosDst.x);
			kStatePacket.lY = long(c_rkPPosDst.y);
		}else{
			kStatePacket.lX = long(cpos.x);
			kStatePacket.lY = long(cpos.y < 0.0f ? cpos.y * -1.0f : cpos.y);
		}
	}

	kStatePacket.dwTime = ELTimer_GetServerMSec();

	assert(kStatePacket.lX >= 0 && kStatePacket.lX < 204800);

	__LocalPositionToGlobalPosition(kStatePacket.lX, kStatePacket.lY);

	if (!Send(sizeof(kStatePacket), &kStatePacket))
	{
		Tracenf("CPythonNetworkStream::SendCharacterStatePacket(dwCmdTime=%u, fDstPos=(%f, %f), fDstRot=%f, eFunc=%d uArg=%d) - PACKET SEND ERROR",
			kStatePacket.dwTime,
			float(kStatePacket.lX),
			float(kStatePacket.lY),
			fDstRot,
			kStatePacket.bFunc,
			kStatePacket.bArg);
		return false;
	}

	return true;
}

bool CPythonNetworkStream::SendUseSkillPacket(DWORD dwSkillIndex, DWORD dwTargetVID)
{
	TPacketCGUseSkill UseSkillPacket;
	UseSkillPacket.bHeader = HEADER_CG_USE_SKILL;
	UseSkillPacket.dwVnum = dwSkillIndex;
	UseSkillPacket.dwTargetVID = dwTargetVID;
	if (!Send(sizeof(TPacketCGUseSkill), &UseSkillPacket))
	{
		Tracen("CPythonNetworkStream::SendUseSkillPacket - SEND PACKET ERROR");
		return false;
	}

	return true;
}

#ifdef ENABLE_GLOBAL_MESSAGE_UTILITY
bool CPythonNetworkStream::SendBulkWhisperPacket(const char* c_szContent)
{
	if (strlen(c_szContent) == 0)
		return true;

	if (strlen(c_szContent) >= 512)
		return true;

	TPacketCGBulkWhisper WhisperPacket;
	WhisperPacket.header = HEADER_CG_BULK_WHISPER;
	strncpy(WhisperPacket.szText, c_szContent, sizeof(WhisperPacket.szText) - 1);

	if (!Send(sizeof(WhisperPacket), &WhisperPacket))
		return false;

	return true;
}
#endif

bool CPythonNetworkStream::SendChatPacket(const char* c_szChat, BYTE byType)
{
	if (m_strPhase != "Game")
		return true;

	if (strlen(c_szChat) == 0)
		return true;

	if (strlen(c_szChat) >= 512)
		return true;

	if (c_szChat[0] == '/')
	{
		if (1 == strlen(c_szChat))
		{
			if (!m_strLastCommand.empty())
				c_szChat = m_strLastCommand.c_str();
		}
		else
		{
			m_strLastCommand = c_szChat;
		}
	}

	if (ClientCommand(c_szChat))
		return true;

	int iTextLen = strlen(c_szChat) + 1;

	TPacketCGChat ChatPacket;
	ChatPacket.header = HEADER_CG_CHAT;
	ChatPacket.length = sizeof(ChatPacket) + iTextLen;
	ChatPacket.type = byType;

	if (!Send(sizeof(ChatPacket), &ChatPacket))
		return false;

	if (!Send(iTextLen, c_szChat))
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////
// Emoticon
void CPythonNetworkStream::RegisterEmoticonString(const char* pcEmoticonString)
{
	if (m_EmoticonStringVector.size() >= CInstanceBase::EMOTICON_NUM)
	{
		TraceError("Can't register emoticon string... vector is full (size:%d)", m_EmoticonStringVector.size());
		return;
	}
	m_EmoticonStringVector.push_back(pcEmoticonString);
}

bool CPythonNetworkStream::ParseEmoticon(const char* pChatMsg, DWORD* pdwEmoticon)
{
	for (DWORD dwEmoticonIndex = 0; dwEmoticonIndex < m_EmoticonStringVector.size(); ++dwEmoticonIndex)
	{
		if (strlen(pChatMsg) > m_EmoticonStringVector[dwEmoticonIndex].size())
			continue;

		const char* pcFind = strstr(pChatMsg, m_EmoticonStringVector[dwEmoticonIndex].c_str());

		if (pcFind != pChatMsg)
			continue;

		*pdwEmoticon = dwEmoticonIndex;

		return true;
	}

	return false;
}
// Emoticon
//////////////////////////////////////////////////////////////////////////

#ifdef ENABLE_GLOBAL_MESSAGE_UTILITY
bool CPythonNetworkStream::RecvBulkWhisperPacket()
{
	TPacketGCBulkWhisper kWhisper;
	char buf[1024 + 1];

	if (!Recv(sizeof(kWhisper), &kWhisper))
		return false;

	UINT uWhisperSize = kWhisper.size - sizeof(kWhisper);

	if (!Recv(uWhisperSize, buf))
		return false;

	buf[uWhisperSize] = '\0';

#ifndef LIVE_SERVER
	TraceError("CPythonNetworkStream::RecvBulkWhisperPacket - %s", buf);
#endif

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_OnRecvBulkWhisper", Py_BuildValue("(s)", buf));
	return true;
}
#endif

bool CPythonNetworkStream::RecvChatPacket()
{
	TPacketGCChat kChat;
	char buf[1024 + 1];
	char line[1024 + 1];

	if (!Recv(sizeof(kChat), &kChat))
		return false;

	UINT uChatSize = kChat.size - sizeof(kChat);

	if (!Recv(uChatSize, buf))
		return false;

	buf[uChatSize] = '\0';

	char* p = strchr(buf, ':');
	if (p && p[1] == ' ')
		p[1] = 0x08;

	if (kChat.type >= CHAT_TYPE_MAX_NUM)
		return true;

#ifdef ENABLE_NEW_DUNGEON_TYPE
	if (CHAT_TYPE_DUNGEON_MISSION == kChat.type)
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_SetDungeonMission", Py_BuildValue("(s)", buf));
		kChat.type = CHAT_TYPE_NOTICE;
	}
	else if (CHAT_TYPE_DUNGEON_SUBMISSION == kChat.type)
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_SetDungeonSubMission", Py_BuildValue("(s)", buf));
		kChat.type = CHAT_TYPE_NOTICE;
	}
#endif

	if (CHAT_TYPE_COMMAND == kChat.type)
	{
		ServerCommand(buf);
		return true;
	}

	if (kChat.dwVID != 0)
	{
		CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
		CInstanceBase* pkInstChatter = rkChrMgr.GetInstancePtr(kChat.dwVID);
		if (NULL == pkInstChatter)
			return true;

		switch (kChat.type)
		{
		case CHAT_TYPE_TALKING:
		case CHAT_TYPE_PARTY:
		case CHAT_TYPE_GUILD:
		case CHAT_TYPE_SHOUT:
		case CHAT_TYPE_WHISPER:
		{
			char* p = strchr(buf, ':');

			if (p)
				p += 2;
			else
				p = buf;

			DWORD dwEmoticon;

			if (ParseEmoticon(p, &dwEmoticon))
			{
				pkInstChatter->SetEmoticon(dwEmoticon);
				return true;
			}
			else
				_snprintf(line, sizeof(line), "%s", p);
		}
		break;
		case CHAT_TYPE_COMMAND:
		case CHAT_TYPE_INFO:
		case CHAT_TYPE_NOTICE:
		case CHAT_TYPE_BIG_NOTICE:
			// case CHAT_TYPE_UNK_10:
#ifdef ENABLE_DICE_SYSTEM
		case CHAT_TYPE_DICE_INFO:
#endif
#ifdef ENABLE_CHAT_FILTER
		case CHAT_TYPE_NOTICE_IMPROVING:
#endif
		case CHAT_TYPE_MAX_NUM:
		default:
			_snprintf(line, sizeof(line), "%s", buf);
			break;
		}

		if (CHAT_TYPE_SHOUT != kChat.type)
		{
			CPythonTextTail::Instance().RegisterChatTail(kChat.dwVID, line);
		}

#ifdef ENABLE_CHAT_FILTER_OPTION
		if (pkInstChatter->IsPC() && !CPythonSystem::Instance().IsChatFilter(kChat.type))
			CPythonChat::Instance().AppendChat(kChat.type, buf);
#else
		if (pkInstChatter->IsPC())
			CPythonChat::Instance().AppendChat(kChat.type, buf);
#endif
	}
	else
	{
		if (CHAT_TYPE_NOTICE == kChat.type)
		{
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_SetTipMessage", Py_BuildValue("(s)", buf));
		}
		else if (CHAT_TYPE_BIG_NOTICE == kChat.type)
		{
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_SetBigMessage", Py_BuildValue("(s)", buf));
		}
#ifdef ENABLE_CHAT_FILTER_OPTION
		if (!CPythonSystem::Instance().IsChatFilter(kChat.type))
			CPythonChat::Instance().AppendChat(kChat.type, buf);
#else
		CPythonChat::Instance().AppendChat(kChat.type, buf);
#endif
	}
	return true;
}

bool CPythonNetworkStream::RecvWhisperPacket()
{
	TPacketGCWhisper whisperPacket;
	char buf[512 + 1];

	if (!Recv(sizeof(whisperPacket), &whisperPacket))
		return false;

	assert(whisperPacket.wSize - sizeof(whisperPacket) < 512);

	if (!Recv(whisperPacket.wSize - sizeof(whisperPacket), &buf))
		return false;

	buf[whisperPacket.wSize - sizeof(whisperPacket)] = '\0';

	static char line[256];
	if (CPythonChat::WHISPER_TYPE_CHAT == whisperPacket.bType || CPythonChat::WHISPER_TYPE_GM == whisperPacket.bType)
	{
		_snprintf(line, sizeof(line), "%s : %s", whisperPacket.szNameFrom, buf);
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnRecvWhisper", Py_BuildValue("(iss)", (int)whisperPacket.bType, whisperPacket.szNameFrom, line));
	}
	else if (CPythonChat::WHISPER_TYPE_SYSTEM == whisperPacket.bType || CPythonChat::WHISPER_TYPE_ERROR == whisperPacket.bType)
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnRecvWhisperSystemMessage", Py_BuildValue("(iss)", (int)whisperPacket.bType, whisperPacket.szNameFrom, buf));
	}
	else
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnRecvWhisperError", Py_BuildValue("(iss)", (int)whisperPacket.bType, whisperPacket.szNameFrom, buf));
	}

	return true;
}

bool CPythonNetworkStream::SendWhisperPacket(const char* name, const char* c_szChat)
{
	if (strlen(c_szChat) >= 255)
		return true;

	int iTextLen = strlen(c_szChat) + 1;
	TPacketCGWhisper WhisperPacket;
	WhisperPacket.bHeader = HEADER_CG_WHISPER;
	WhisperPacket.wSize = sizeof(WhisperPacket) + iTextLen;

	strncpy(WhisperPacket.szNameTo, name, sizeof(WhisperPacket.szNameTo) - 1);

	if (!Send(sizeof(WhisperPacket), &WhisperPacket))
		return false;

	if (!Send(iTextLen, c_szChat))
		return false;

	return true;
}

bool CPythonNetworkStream::RecvPointChange()
{
	TPacketGCPointChange PointChange;

	if (!Recv(sizeof(TPacketGCPointChange), &PointChange))
	{
		Tracen("Recv Point Change Packet Error");
		return false;
	}

	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	rkChrMgr.ShowPointEffect(PointChange.Type, PointChange.dwVID);

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetMainInstancePtr();

	if (pInstance && PointChange.dwVID == pInstance->GetVirtualID())
	{
		CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
		rkPlayer.SetStatus(PointChange.Type, PointChange.value);

		switch (PointChange.Type)
		{
		case POINT_STAT_RESET_COUNT:
			__RefreshStatus();
			break;
		case POINT_LEVEL:
		case POINT_ST:
		case POINT_DX:
		case POINT_HT:
		case POINT_IQ:
			__RefreshStatus();
			__RefreshSkillWindow();
			break;
		case POINT_SKILL:
		case POINT_SUB_SKILL:
		case POINT_HORSE_SKILL:
			__RefreshSkillWindow();
			break;
		case POINT_ENERGY:
			if (PointChange.value == 0)
			{
				rkPlayer.SetStatus(POINT_ENERGY_END_TIME, 0);
			}
			__RefreshStatus();
			break;
		default:
			__RefreshStatus();
			break;
		}

#ifdef ENABLE_GOLD_INFO_TEXT
		if (POINT_GOLD == PointChange.Type && CPythonSystem::Instance().IsGoldInfo())
		{
			if (PointChange.amount > 0)
			{
#ifdef ENABLE_GOLD_LIMIT_REWORK
				PyObject* args = PyTuple_New(1);
				PyTuple_SetItem(args, 0, PyLong_FromLongLong(PointChange.amount));
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnPickMoney", args);
#else
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnPickMoney", Py_BuildValue("(i)", PointChange.amount));
#endif
			}
		}
#endif
#ifdef ENABLE_CHEQUE_SYSTEM
		if (POINT_CHEQUE == PointChange.Type)
		{
			if (PointChange.amount > 0)
			{
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnPickCheque", Py_BuildValue("(i)", PointChange.amount));
			}
		}
#endif
#ifdef ENABLE_EXP_INFO_TEXT
		if (CItemData::POINT_EXP == PointChange.Type && CPythonSystem::Instance().IsExpInfo())
		{
			if (PointChange.amount > 0)
			{
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnRecvExp", Py_BuildValue("(i)", PointChange.amount));
			}
		}
#endif
	}
	// @DIKKAT @duzenleme
	// bu amini siktigim niye hala burda ?
	else
	{
		// the /advance command will provide no global refresh! it sends the pointchange only to the specific player and not all
		pInstance = CPythonCharacterManager::Instance().GetInstancePtr(PointChange.dwVID);
		if (pInstance && PointChange.Type == POINT_LEVEL)
		{
			pInstance->SetLevel(PointChange.value);
			pInstance->UpdateTextTailLevel(PointChange.value);
		}
	}

	return true;
}

bool CPythonNetworkStream::RecvStunPacket()
{
	TPacketGCStun StunPacket;

	if (!Recv(sizeof(StunPacket), &StunPacket))
	{
		Tracen("CPythonNetworkStream::RecvStunPacket Error");
		return false;
	}

	//Tracef("RecvStunPacket %d\n", StunPacket.vid);

	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	CInstanceBase* pkInstSel = rkChrMgr.GetInstancePtr(StunPacket.vid);

	if (pkInstSel)
	{
		if (CPythonCharacterManager::Instance().GetMainInstancePtr() == pkInstSel)
			pkInstSel->Die();
		else
			pkInstSel->Stun();
	}

	return true;
}

bool CPythonNetworkStream::RecvDeadPacket()
{
	TPacketGCDead DeadPacket;
	if (!Recv(sizeof(DeadPacket), &DeadPacket))
	{
		Tracen("CPythonNetworkStream::RecvDeadPacket Error");
		return false;
	}

	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	CInstanceBase* pkChrInstSel = rkChrMgr.GetInstancePtr(DeadPacket.vid);
	if (pkChrInstSel)
	{
		CInstanceBase* pkInstMain = rkChrMgr.GetMainInstancePtr();
		if (pkInstMain == pkChrInstSel)
		{
#ifdef ENABLE_AUTO_HUNT_SYSTEM
			CPythonPlayer& olenCh = CPythonPlayer::Instance();
			if (olenCh.OtoAvDurumAl()){ olenCh.OtoAvDurumAta(false); }
#endif
			Tracenf("On MainActor");
			if (false == pkInstMain->GetDuelMode())
			{
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnGameOver", Py_BuildValue("()"));
			}
			CPythonPlayer::Instance().NotifyDeadMainCharacter();
		}

		pkChrInstSel->Die();
	}

	return true;
}

bool CPythonNetworkStream::SendCharacterPositionPacket(BYTE iPosition)
{
	TPacketCGPosition PositionPacket;

	PositionPacket.header = HEADER_CG_CHARACTER_POSITION;
	PositionPacket.position = iPosition;

	if (!Send(sizeof(TPacketCGPosition), &PositionPacket))
	{
		Tracen("Send Character Position Packet Error");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::SendOnClickPacket(DWORD vid)
{
	TPacketCGOnClick OnClickPacket;
	OnClickPacket.header = HEADER_CG_ON_CLICK;
	OnClickPacket.vid = vid;

	if (!Send(sizeof(OnClickPacket), &OnClickPacket))
	{
		Tracen("Send On_Click Packet Error");
		return false;
	}

	Tracef("SendOnClickPacket\n");
	return true;
}

bool CPythonNetworkStream::RecvCharacterPositionPacket()
{
	TPacketGCPosition PositionPacket;

	if (!Recv(sizeof(TPacketGCPosition), &PositionPacket))
		return false;

	CInstanceBase* pChrInstance = CPythonCharacterManager::Instance().GetInstancePtr(PositionPacket.vid);

	if (!pChrInstance)
		return true;

	//pChrInstance->UpdatePosition(PositionPacket.position);

	return true;
}

bool CPythonNetworkStream::RecvMotionPacket()
{
	TPacketGCMotion MotionPacket;

	if (!Recv(sizeof(TPacketGCMotion), &MotionPacket))
		return false;

	CInstanceBase* pMainInstance = CPythonCharacterManager::Instance().GetInstancePtr(MotionPacket.vid);
	CInstanceBase* pVictimInstance = NULL;

	if (0 != MotionPacket.victim_vid)
		pVictimInstance = CPythonCharacterManager::Instance().GetInstancePtr(MotionPacket.victim_vid);

	if (!pMainInstance)
		return false;

	return true;
}

bool CPythonNetworkStream::RecvShopPacket()
{
	std::vector<char> vecBuffer;
	vecBuffer.clear();

	TPacketGCShop  packet_shop;
	if (!Recv(sizeof(packet_shop), &packet_shop))
		return false;

	int iSize = packet_shop.size - sizeof(packet_shop);
	if (iSize > 0)
	{
		vecBuffer.resize(iSize);
		if (!Recv(iSize, &vecBuffer[0]))
			return false;
	}

	switch (packet_shop.subheader)
	{
	case SHOP_SUBHEADER_GC_START:
	{
		if (iSize == 0)
			return true;
		CPythonShop::Instance().Clear();

		DWORD dwVID = *(DWORD*)&vecBuffer[0];

		TPacketGCShopStart* pShopStartPacket = (TPacketGCShopStart*)&vecBuffer[4];
		for (BYTE iItemIndex = 0; iItemIndex < SHOP_HOST_ITEM_MAX_NUM; ++iItemIndex)
		{
			CPythonShop::Instance().SetItemData(iItemIndex, pShopStartPacket->items[iItemIndex]);
		}

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "StartShop", Py_BuildValue("(i)", dwVID));
	}
	break;

	case SHOP_SUBHEADER_GC_END:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "EndShop", Py_BuildValue("()"));
		break;

	case SHOP_SUBHEADER_GC_UPDATE_ITEM:
	{
		if (iSize == 0)
			return true;

		TPacketGCShopUpdateItem* pShopUpdateItemPacket = (TPacketGCShopUpdateItem*)&vecBuffer[0];
		CPythonShop::Instance().SetItemData(pShopUpdateItemPacket->pos, pShopUpdateItemPacket->item);
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshShop", Py_BuildValue("()"));
	}
	break;

	case SHOP_SUBHEADER_GC_UPDATE_PRICE:
#ifdef ENABLE_GOLD_LIMIT_REWORK
	{
		if (iSize == 0)
			return true;
		PyObject* args = PyTuple_New(1);
		PyTuple_SetItem(args, 0, PyLong_FromLongLong(*(long long*)&vecBuffer[0]));
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "SetShopSellingPrice", args);
	}
#else
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "SetShopSellingPrice", Py_BuildValue("(i)", *(int*)&vecBuffer[0]));
#endif
		break;

	case SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "NOT_ENOUGH_MONEY"));
		break;

	case SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY_EX:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "NOT_ENOUGH_MONEY_EX"));
		break;

	case SHOP_SUBHEADER_GC_SOLDOUT:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "SOLDOUT"));
		break;

	case SHOP_SUBHEADER_GC_INVENTORY_FULL:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "INVENTORY_FULL"));
		break;

	case SHOP_SUBHEADER_GC_INVALID_POS:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "INVALID_POS"));
		break;
#ifdef ENABLE_SHOP_PRICE_TYPE_ITEM
	case SHOP_SUBHEADER_GC_NOT_ENOUGH_ITEM:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "NOT_ENOUGH_ITEM"));
		break;
#endif
#ifdef ENABLE_CHEQUE_SYSTEM
	case SHOP_SUBHEADER_GC_NOT_ENOUGH_CHEQUE:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "NOT_ENOUGH_CHEQUE"));
		break;

	case SHOP_SUBHEADER_GC_NOT_ENOUGH_CHEQUE_MONEY:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "NOT_ENOUGH_CHEQUE_MONEY"));
		break;
#endif
	default:
		TraceError("CPythonNetworkStream::RecvShopPacket: Unknown subheader\n");
		break;
	}

	return true;
}

bool CPythonNetworkStream::RecvExchangePacket()
{
	TPacketGCExchange exchange_packet;

	if (!Recv(sizeof(exchange_packet), &exchange_packet))
		return false;

	switch (exchange_packet.subheader)
	{
	case EXCHANGE_SUBHEADER_GC_START:
		CPythonExchange::Instance().Clear();
		CPythonExchange::Instance().Start();
		CPythonExchange::Instance().SetSelfName(CPythonPlayer::Instance().GetName());
#ifdef ENABLE_EXCHANGE_REWORK
		CPythonExchange::Instance().SetSelfLevel(CPythonPlayer::Instance().GetStatus(POINT_LEVEL));
#endif
		{
			CInstanceBase* pCharacterInstance = CPythonCharacterManager::Instance().GetInstancePtr(exchange_packet.arg1);

			if (pCharacterInstance)
			{
				CPythonExchange::Instance().SetTargetName(pCharacterInstance->GetNameString());
#ifdef ENABLE_EXCHANGE_REWORK
				CPythonExchange::Instance().SetTargetLevel(pCharacterInstance->GetLevel());
#endif
			}
		}

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "StartExchange", Py_BuildValue("()"));
		break;

	case EXCHANGE_SUBHEADER_GC_ITEM_ADD:
		if (exchange_packet.is_me)
		{
			int iSlotIndex = exchange_packet.arg2.cell;
#ifdef ENABLE_ITEM_COUNT_LIMIT
			CPythonExchange::Instance().SetItemToSelf(iSlotIndex, exchange_packet.arg1, (DWORD)exchange_packet.arg3);
#else
			CPythonExchange::Instance().SetItemToSelf(iSlotIndex, exchange_packet.arg1, (BYTE)exchange_packet.arg3);
#endif
			for (int i = 0; i < ITEM_SOCKET_SLOT_MAX_NUM; ++i)
				CPythonExchange::Instance().SetItemMetinSocketToSelf(iSlotIndex, i, exchange_packet.alValues[i]);
			for (int j = 0; j < ITEM_ATTRIBUTE_SLOT_MAX_NUM; ++j)
				CPythonExchange::Instance().SetItemAttributeToSelf(iSlotIndex, j, exchange_packet.aAttr[j].bType, exchange_packet.aAttr[j].sValue);
#ifdef ENABLE_ITEM_EVOLUTION_SYSTEM
			CPythonExchange::Instance().SetItemEvolutionToSelf(iSlotIndex, exchange_packet.evolution);
#endif
#ifdef ENABLE_CHANGELOOK_SYSTEM
			CPythonExchange::Instance().SetItemTransmutation(iSlotIndex, exchange_packet.dwTransmutation, true);
#endif
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ExchangeItemAdd", Py_BuildValue("(ii)", iSlotIndex, exchange_packet.arg1));
		}
		else
		{
			int iSlotIndex = exchange_packet.arg2.cell;
#ifdef ENABLE_ITEM_COUNT_LIMIT
			CPythonExchange::Instance().SetItemToTarget(iSlotIndex, exchange_packet.arg1, (DWORD)exchange_packet.arg3);
#else
			CPythonExchange::Instance().SetItemToTarget(iSlotIndex, exchange_packet.arg1, (BYTE)exchange_packet.arg3);
#endif
			for (int i = 0; i < ITEM_SOCKET_SLOT_MAX_NUM; ++i)
				CPythonExchange::Instance().SetItemMetinSocketToTarget(iSlotIndex, i, exchange_packet.alValues[i]);
			for (int j = 0; j < ITEM_ATTRIBUTE_SLOT_MAX_NUM; ++j)
				CPythonExchange::Instance().SetItemAttributeToTarget(iSlotIndex, j, exchange_packet.aAttr[j].bType, exchange_packet.aAttr[j].sValue);
#ifdef ENABLE_ITEM_EVOLUTION_SYSTEM
			CPythonExchange::Instance().SetItemEvolutionToTarget(iSlotIndex, exchange_packet.evolution);
#endif
#ifdef ENABLE_CHANGELOOK_SYSTEM
			CPythonExchange::Instance().SetItemTransmutation(iSlotIndex, exchange_packet.dwTransmutation, false);
#endif
		}
		__RefreshExchangeWindow();
		__RefreshInventoryWindow();
		break;

	case EXCHANGE_SUBHEADER_GC_ITEM_DEL:
		if (exchange_packet.is_me)
			CPythonExchange::Instance().DelItemOfSelf((BYTE)exchange_packet.arg1);
		else
			CPythonExchange::Instance().DelItemOfTarget((BYTE)exchange_packet.arg1);

		__RefreshExchangeWindow();
		__RefreshInventoryWindow();
		break;

	case EXCHANGE_SUBHEADER_GC_ELK_ADD:
		if (exchange_packet.is_me)
			CPythonExchange::Instance().SetElkToSelf(exchange_packet.arg1);
		else
			CPythonExchange::Instance().SetElkToTarget(exchange_packet.arg1);

		__RefreshExchangeWindow();
		break;

	case EXCHANGE_SUBHEADER_GC_ACCEPT:
		if (exchange_packet.is_me)
		{
			CPythonExchange::Instance().SetAcceptToSelf((BYTE)exchange_packet.arg1);
		}
		else
		{
			CPythonExchange::Instance().SetAcceptToTarget((BYTE)exchange_packet.arg1);
		}
		__RefreshExchangeWindow();
		break;

	case EXCHANGE_SUBHEADER_GC_END:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "EndExchange", Py_BuildValue("()"));
		__RefreshInventoryWindow();
		CPythonExchange::Instance().End();
		break;

	case EXCHANGE_SUBHEADER_GC_ALREADY:
		Tracef("trade_already");
		break;

	case EXCHANGE_SUBHEADER_GC_LESS_ELK:
		Tracef("trade_less_elk");
		break;

#ifdef ENABLE_CHEQUE_SYSTEM
	case EXCHANGE_SUBHEADER_GC_CHEQUE_ADD:
		if (exchange_packet.is_me)
			CPythonExchange::Instance().SetChequeToSelf(exchange_packet.arg1);
		else
			CPythonExchange::Instance().SetChequeToTarget(exchange_packet.arg1);

		__RefreshExchangeWindow();
		break;

	case EXCHANGE_SUBHEADER_GC_LESS_CHEQUE:
		Tracef("trade_less_cheque");
		break;
#endif
	};

	return true;
}

bool CPythonNetworkStream::RecvQuestInfoPacket()
{
	TPacketGCQuestInfo QuestInfo;

	if (!Peek(sizeof(TPacketGCQuestInfo), &QuestInfo))
	{
		Tracen("Recv Quest Info Packet Error #1");
		return false;
	}

	if (!Peek(QuestInfo.size))
	{
		Tracen("Recv Quest Info Packet Error #2");
		return false;
	}

	Recv(sizeof(TPacketGCQuestInfo));

	const BYTE& c_rFlag = QuestInfo.flag;

	enum
	{
		QUEST_PACKET_TYPE_NONE,
		QUEST_PACKET_TYPE_BEGIN,
		QUEST_PACKET_TYPE_UPDATE,
		QUEST_PACKET_TYPE_END,
	};

	BYTE byQuestPacketType = QUEST_PACKET_TYPE_NONE;

	if (0 != (c_rFlag & QUEST_SEND_IS_BEGIN))
	{
		BYTE isBegin;
		if (!Recv(sizeof(isBegin), &isBegin))
			return false;

		if (isBegin)
			byQuestPacketType = QUEST_PACKET_TYPE_BEGIN;
		else
			byQuestPacketType = QUEST_PACKET_TYPE_END;
	}
	else
	{
		byQuestPacketType = QUEST_PACKET_TYPE_UPDATE;
	}

	// Recv Data Start
	char szTitle[30 + 1] = "";
	char szClockName[16 + 1] = "";
	int iClockValue = 0;
	char szCounterName[16 + 1] = "";
	int iCounterValue = 0;
	char szIconFileName[24 + 1] = "";

	if (0 != (c_rFlag & QUEST_SEND_TITLE))
	{
		if (!Recv(sizeof(szTitle), &szTitle))
			return false;

		szTitle[30] = '\0';
	}
	if (0 != (c_rFlag & QUEST_SEND_CLOCK_NAME))
	{
		if (!Recv(sizeof(szClockName), &szClockName))
			return false;

		szClockName[16] = '\0';
	}
	if (0 != (c_rFlag & QUEST_SEND_CLOCK_VALUE))
	{
		if (!Recv(sizeof(iClockValue), &iClockValue))
			return false;
	}
	if (0 != (c_rFlag & QUEST_SEND_COUNTER_NAME))
	{
		if (!Recv(sizeof(szCounterName), &szCounterName))
			return false;

		szCounterName[16] = '\0';
	}
	if (0 != (c_rFlag & QUEST_SEND_COUNTER_VALUE))
	{
		if (!Recv(sizeof(iCounterValue), &iCounterValue))
			return false;
	}
	if (0 != (c_rFlag & QUEST_SEND_ICON_FILE))
	{
		if (!Recv(sizeof(szIconFileName), &szIconFileName))
			return false;

		szIconFileName[24] = '\0';
	}
	// Recv Data End

	CPythonQuest& rkQuest = CPythonQuest::Instance();

	// Process Start
	if (QUEST_PACKET_TYPE_END == byQuestPacketType)
	{
		rkQuest.DeleteQuestInstance(QuestInfo.index);
	}
	else if (QUEST_PACKET_TYPE_UPDATE == byQuestPacketType)
	{
		if (!rkQuest.IsQuest(QuestInfo.index))
		{
#ifdef ENABLE_QUEST_CATEGORY_SYSTEM
			rkQuest.MakeQuest(QuestInfo.index, QuestInfo.c_index);
#else
			rkQuest.MakeQuest(QuestInfo.index);
#endif
		}

		if (strlen(szTitle) > 0)
			rkQuest.SetQuestTitle(QuestInfo.index, szTitle);
		if (strlen(szClockName) > 0)
			rkQuest.SetQuestClockName(QuestInfo.index, szClockName);
		if (strlen(szCounterName) > 0)
			rkQuest.SetQuestCounterName(QuestInfo.index, szCounterName);
		if (strlen(szIconFileName) > 0)
			rkQuest.SetQuestIconFileName(QuestInfo.index, szIconFileName);

		if (c_rFlag & QUEST_SEND_CLOCK_VALUE)
			rkQuest.SetQuestClockValue(QuestInfo.index, iClockValue);
		if (c_rFlag & QUEST_SEND_COUNTER_VALUE)
			rkQuest.SetQuestCounterValue(QuestInfo.index, iCounterValue);

// #ifdef ENABLE_QUEST_CATEGORY_SYSTEM
// 		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RecvQuest", Py_BuildValue("(is)", QuestInfo.index, szTitle));
// #endif
	}
	else if (QUEST_PACKET_TYPE_BEGIN == byQuestPacketType)
	{
		CPythonQuest::SQuestInstance QuestInstance;
		QuestInstance.dwIndex = QuestInfo.index;
		QuestInstance.strTitle = szTitle;
		QuestInstance.strClockName = szClockName;
		QuestInstance.iClockValue = iClockValue;
		QuestInstance.strCounterName = szCounterName;
		QuestInstance.iCounterValue = iCounterValue;
		QuestInstance.strIconFileName = szIconFileName;
		CPythonQuest::Instance().RegisterQuestInstance(QuestInstance);
	}
	// Process Start End

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshQuest", Py_BuildValue("()"));
	return true;
}

bool CPythonNetworkStream::RecvQuestConfirmPacket()
{
	TPacketGCQuestConfirm kQuestConfirmPacket;
	if (!Recv(sizeof(kQuestConfirmPacket), &kQuestConfirmPacket))
	{
		Tracen("RecvQuestConfirmPacket Error");
		return false;
	}

	PyObject* poArg = Py_BuildValue("(sii)", kQuestConfirmPacket.msg, kQuestConfirmPacket.timeout, kQuestConfirmPacket.requestPID);
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_OnQuestConfirm", poArg);
	return true;
}

bool CPythonNetworkStream::RecvRequestMakeGuild()
{
	TPacketGCBlank blank;
	if (!Recv(sizeof(blank), &blank))
	{
		Tracen("RecvRequestMakeGuild Packet Error");
		return false;
	}

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "AskGuildName", Py_BuildValue("()"));

	return true;
}

void CPythonNetworkStream::ToggleGameDebugInfo()
{
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ToggleDebugInfo", Py_BuildValue("()"));
}

bool CPythonNetworkStream::SendExchangeStartPacket(DWORD vid)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGExchange	packet;

	packet.header = HEADER_CG_EXCHANGE;
	packet.subheader = EXCHANGE_SUBHEADER_CG_START;
	packet.arg1 = vid;

	if (!Send(sizeof(packet), &packet))
	{
		Tracef("send_trade_start_packet Error\n");
		return false;
	}

	Tracef("send_trade_start_packet   vid %d \n", vid);
	return true;
}

#ifdef ENABLE_GOLD_LIMIT_REWORK
bool CPythonNetworkStream::SendExchangeElkAddPacket(long long elk)
#else
bool CPythonNetworkStream::SendExchangeElkAddPacket(DWORD elk)
#endif
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGExchange	packet;

	packet.header = HEADER_CG_EXCHANGE;
	packet.subheader = EXCHANGE_SUBHEADER_CG_ELK_ADD;
	packet.arg1 = elk;

	if (!Send(sizeof(packet), &packet))
	{
		Tracef("send_trade_elk_add_packet Error\n");
		return false;
	}

	return true;
}

#ifdef ENABLE_CHEQUE_SYSTEM
bool CPythonNetworkStream::SendExchangeChequeAddPacket(DWORD cheque)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGExchange	packet;

	packet.header = HEADER_CG_EXCHANGE;
	packet.subheader = EXCHANGE_SUBHEADER_CG_CHEQUE_ADD;
	packet.arg1 = cheque;

	if (!Send(sizeof(packet), &packet))
	{
		Tracef("send_trade_cheque_add_packet Error\n");
		return false;
	}

	return true;
}
#endif

bool CPythonNetworkStream::SendExchangeItemAddPacket(TItemPos ItemPos, BYTE byDisplayPos)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGExchange	packet;

	packet.header = HEADER_CG_EXCHANGE;
	packet.subheader = EXCHANGE_SUBHEADER_CG_ITEM_ADD;
	packet.Pos = ItemPos;
	packet.arg2 = byDisplayPos;

	if (!Send(sizeof(packet), &packet))
	{
		Tracef("send_trade_item_add_packet Error\n");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::SendExchangeItemDelPacket(BYTE pos)
{
	assert(!"Can't be called function - CPythonNetworkStream::SendExchangeItemDelPacket");
	return true;

	if (!__CanActMainInstance())
		return true;

	TPacketCGExchange	packet;

	packet.header = HEADER_CG_EXCHANGE;
	packet.subheader = EXCHANGE_SUBHEADER_CG_ITEM_DEL;
	packet.arg1 = pos;

	if (!Send(sizeof(packet), &packet))
	{
		Tracef("send_trade_item_del_packet Error\n");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::SendExchangeAcceptPacket()
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGExchange	packet;

	packet.header = HEADER_CG_EXCHANGE;
	packet.subheader = EXCHANGE_SUBHEADER_CG_ACCEPT;

	if (!Send(sizeof(packet), &packet))
	{
		Tracef("send_trade_accept_packet Error\n");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::SendExchangeExitPacket()
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGExchange	packet;

	packet.header = HEADER_CG_EXCHANGE;
	packet.subheader = EXCHANGE_SUBHEADER_CG_CANCEL;

	if (!Send(sizeof(packet), &packet))
	{
		Tracef("send_trade_exit_packet Error\n");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::SendPointResetPacket()
{
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "StartPointReset", Py_BuildValue("()"));
	return true;
}

bool CPythonNetworkStream::__IsPlayerAttacking()
{
	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	CInstanceBase* pkInstMain = rkChrMgr.GetMainInstancePtr();
	if (!pkInstMain)
		return false;

	if (!pkInstMain->IsAttacking())
		return false;

	return true;
}

bool CPythonNetworkStream::RecvScriptPacket()
{
	TPacketGCScript ScriptPacket;

	if (!Recv(sizeof(TPacketGCScript), &ScriptPacket))
	{
		TraceError("RecvScriptPacket_RecvError");
		return false;
	}

	if (ScriptPacket.size < sizeof(TPacketGCScript))
	{
		TraceError("RecvScriptPacket_SizeError");
		return false;
	}

	ScriptPacket.size -= sizeof(TPacketGCScript);

	static string str;
	str = "";
	str.resize(ScriptPacket.size + 1);

	if (!Recv(ScriptPacket.size, &str[0]))
		return false;

	str[str.size() - 1] = '\0';

	int iIndex = CPythonEventManager::Instance().RegisterEventSetFromString(str);

	if (-1 != iIndex)
	{
		CPythonEventManager::Instance().SetVisibleLineCount(iIndex, 30);
		CPythonNetworkStream::Instance().OnScriptEventStart(ScriptPacket.skin, iIndex);
	}

	return true;
}

bool CPythonNetworkStream::SendScriptAnswerPacket(int iAnswer)
{
	TPacketCGScriptAnswer ScriptAnswer;

	ScriptAnswer.header = HEADER_CG_SCRIPT_ANSWER;
	ScriptAnswer.answer = (BYTE)iAnswer;
	if (!Send(sizeof(TPacketCGScriptAnswer), &ScriptAnswer))
	{
		Tracen("Send Script Answer Packet Error");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::SendScriptButtonPacket(unsigned int iIndex)
{
	TPacketCGScriptButton ScriptButton;

	ScriptButton.header = HEADER_CG_SCRIPT_BUTTON;
	ScriptButton.idx = iIndex;
	if (!Send(sizeof(TPacketCGScriptButton), &ScriptButton))
	{
		Tracen("Send Script Button Packet Error");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::SendAnswerMakeGuildPacket(const char* c_szName)
{
	TPacketCGAnswerMakeGuild Packet;

	Packet.header = HEADER_CG_ANSWER_MAKE_GUILD;
	strncpy(Packet.guild_name, c_szName, GUILD_NAME_MAX_LEN);
	Packet.guild_name[GUILD_NAME_MAX_LEN] = '\0';

	if (!Send(sizeof(Packet), &Packet))
	{
		Tracen("SendAnswerMakeGuild Packet Error");
		return false;
	}

	// 	Tracef(" SendAnswerMakeGuildPacket : %s", c_szName);
	return true;
}

bool CPythonNetworkStream::SendQuestInputStringPacket(const char* c_szString)
{
	TPacketCGQuestInputString Packet;
	Packet.bHeader = HEADER_CG_QUEST_INPUT_STRING;
	strncpy(Packet.szString, c_szString, QUEST_INPUT_STRING_MAX_NUM);

	if (!Send(sizeof(Packet), &Packet))
	{
		Tracen("SendQuestInputStringPacket Error");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::SendQuestConfirmPacket(BYTE byAnswer, DWORD dwPID)
{
	TPacketCGQuestConfirm kPacket;
	kPacket.header = HEADER_CG_QUEST_CONFIRM;
	kPacket.answer = byAnswer;
	kPacket.requestPID = dwPID;

	if (!Send(sizeof(kPacket), &kPacket))
	{
		Tracen("SendQuestConfirmPacket Error");
		return false;
	}

	Tracenf(" SendQuestConfirmPacket : %d, %d", byAnswer, dwPID);
	return true;
}

bool CPythonNetworkStream::RecvSkillLevelNew()
{
	TPacketGCSkillLevelNew packet;

	if (!Recv(sizeof(TPacketGCSkillLevelNew), &packet))
	{
		Tracen("CPythonNetworkStream::RecvSkillLevelNew - RecvError");
		return false;
	}

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();

	for (int i = 0; i < SKILL_MAX_NUM; ++i)
	{
		TPlayerSkill& rPlayerSkill = packet.skills[i];
		rkPlayer.SetSkillLevel_(i, rPlayerSkill.bMasterType, rPlayerSkill.bLevel);
	}

	__RefreshSkillWindow();
	__RefreshStatus();
	//Tracef(" >> RecvSkillLevelNew\n");
	return true;
}

bool CPythonNetworkStream::RecvDamageInfoPacket()
{
	TPacketGCDamageInfo DamageInfoPacket;

	if (!Recv(sizeof(TPacketGCDamageInfo), &DamageInfoPacket))
	{
		Tracen("Recv Target Packet Error");
		return false;
	}

	CInstanceBase* pInstTarget = CPythonCharacterManager::Instance().GetInstancePtr(DamageInfoPacket.dwVID);
	bool bSelf = (pInstTarget == CPythonCharacterManager::Instance().GetMainInstancePtr());
	bool bTarget = (pInstTarget == m_pInstTarget);
	if (pInstTarget)
	{
		if (DamageInfoPacket.damage >= 0)
			pInstTarget->AddDamageEffect(DamageInfoPacket.damage, DamageInfoPacket.flag, bSelf, bTarget);
		else
			TraceError("Damage is equal or below 0(%lld).", DamageInfoPacket.damage);
	}

	return true;
}
bool CPythonNetworkStream::RecvTargetPacket()
{
	TPacketGCTarget TargetPacket;

	if (!Recv(sizeof(TPacketGCTarget), &TargetPacket))
	{
		Tracen("Recv Target Packet Error");
		return false;
	}

	CInstanceBase* pInstPlayer = CPythonCharacterManager::Instance().GetMainInstancePtr();
	CInstanceBase* pInstTarget = CPythonCharacterManager::Instance().GetInstancePtr(TargetPacket.dwVID);
	if (pInstPlayer && pInstTarget)
	{
		if (!pInstTarget->IsDead())
		{
			if (pInstTarget->IsPC())
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "CloseTargetBoardIfDifferent", Py_BuildValue("(i)", TargetPacket.dwVID));
			else if (pInstPlayer->CanViewTargetHP(*pInstTarget))
			{
#ifdef ENABLE_TARGET_BOARD_RENEWAL
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "SetHPTargetBoard", Py_BuildValue("(llLL)", TargetPacket.dwVID, TargetPacket.bHPPercent, TargetPacket.dwHP, TargetPacket.dwMaxHP));
#else
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "SetHPTargetBoard", Py_BuildValue("(ll)", TargetPacket.dwVID, TargetPacket.bHPPercent));
#endif
			}
			else
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "CloseTargetBoard", Py_BuildValue("()"));
			m_pInstTarget = pInstTarget;
		}
	}
	else
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "CloseTargetBoard", Py_BuildValue("()"));
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Recv
bool CPythonNetworkStream::SendAttackPacket(UINT uMotAttack, DWORD dwVIDVictim)
{
#ifdef ENABLE_ANTICHEAT
	FunctionProtect FProtect = nullptr;
	FProtect = (FunctionProtect)GetProcAddress(GetModuleHandleA(skCrypt("rascal.dll")), skCrypt("FunctionProtect"));
	FProtect(reinterpret_cast<void*>(getcalleraddr()));
#endif
	if (!__CanActMainInstance())
		return true;
	
	TPacketCGAttack kPacketAtk;

	kPacketAtk.header = HEADER_CG_ATTACK;
	kPacketAtk.bType = uMotAttack;
	kPacketAtk.dwVictimVID = dwVIDVictim;
	if (!SendSpecial(sizeof(kPacketAtk), &kPacketAtk))
	{
		Tracen("Send Battle Attack Packet Error");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::SendSpecial(int nLen, void* pvBuf)
{
	BYTE bHeader = *(BYTE*)pvBuf;

	switch (bHeader)
	{
	case HEADER_CG_ATTACK:
	{
		TPacketCGAttack* pkPacketAtk = (TPacketCGAttack*)pvBuf;
		return Send(nLen, pvBuf);
	}
	break;
	}

	return Send(nLen, pvBuf);
}

bool CPythonNetworkStream::RecvAddFlyTargetingPacket()
{
	TPacketGCFlyTargeting kPacket;
	if (!Recv(sizeof(kPacket), &kPacket))
		return false;

	__GlobalPositionToLocalPosition(kPacket.lX, kPacket.lY);

	Tracef("VID [%d] Added to target settings\n", kPacket.dwShooterVID);

	CPythonCharacterManager& rpcm = CPythonCharacterManager::Instance();

	CInstanceBase* pShooter = rpcm.GetInstancePtr(kPacket.dwShooterVID);

	if (!pShooter)
	{
#ifndef _DEBUG
		TraceError("CPythonNetworkStream::RecvFlyTargetingPacket() - dwShooterVID[%d] NOT EXIST", kPacket.dwShooterVID);
#endif
		return true;
	}

	CInstanceBase* pTarget = rpcm.GetInstancePtr(kPacket.dwTargetVID);

	if (kPacket.dwTargetVID && pTarget)
	{
		pShooter->GetGraphicThingInstancePtr()->AddFlyTarget(pTarget->GetGraphicThingInstancePtr());
	}
	else
	{
		float h = CPythonBackground::Instance().GetHeight(kPacket.lX, kPacket.lY) + 60.0f; // TEMPORARY HEIGHT
		pShooter->GetGraphicThingInstancePtr()->AddFlyTarget(D3DXVECTOR3(kPacket.lX, kPacket.lY, h));
		//pShooter->GetGraphicThingInstancePtr()->SetFlyTarget(kPacket.kPPosTarget.x,kPacket.kPPosTarget.y,);
	}

	return true;
}

bool CPythonNetworkStream::RecvFlyTargetingPacket()
{
	TPacketGCFlyTargeting kPacket;
	if (!Recv(sizeof(kPacket), &kPacket))
		return false;

	__GlobalPositionToLocalPosition(kPacket.lX, kPacket.lY);

	//Tracef("CPythonNetworkStream::RecvFlyTargetingPacket - VID [%d]\n",kPacket.dwShooterVID);

	CPythonCharacterManager& rpcm = CPythonCharacterManager::Instance();

	CInstanceBase* pShooter = rpcm.GetInstancePtr(kPacket.dwShooterVID);

	if (!pShooter)
	{
#ifdef _DEBUG
		TraceError("CPythonNetworkStream::RecvFlyTargetingPacket() - dwShooterVID[%d] NOT EXIST", kPacket.dwShooterVID);
#endif
		return true;
	}

	CInstanceBase* pTarget = rpcm.GetInstancePtr(kPacket.dwTargetVID);

	if (kPacket.dwTargetVID && pTarget)
	{
		pShooter->GetGraphicThingInstancePtr()->SetFlyTarget(pTarget->GetGraphicThingInstancePtr());
	}
	else
	{
		float h = CPythonBackground::Instance().GetHeight(kPacket.lX, kPacket.lY) + 60.0f; // TEMPORARY HEIGHT
		pShooter->GetGraphicThingInstancePtr()->SetFlyTarget(D3DXVECTOR3(kPacket.lX, kPacket.lY, h));
		//pShooter->GetGraphicThingInstancePtr()->SetFlyTarget(kPacket.kPPosTarget.x,kPacket.kPPosTarget.y,);
	}

	return true;
}

bool CPythonNetworkStream::SendShootPacket(UINT uSkill)
{
	TPacketCGShoot kPacketShoot;
	kPacketShoot.bHeader = HEADER_CG_SHOOT;
	kPacketShoot.bType = uSkill;

	if (!Send(sizeof(kPacketShoot), &kPacketShoot))
	{
		Tracen("SendShootPacket Error");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::SendAddFlyTargetingPacket(DWORD dwTargetVID, const TPixelPosition& kPPosTarget)
{
	TPacketCGFlyTargeting packet;

	//CPythonCharacterManager & rpcm = CPythonCharacterManager::Instance();

	packet.bHeader = HEADER_CG_ADD_FLY_TARGETING;
	packet.dwTargetVID = dwTargetVID;
	packet.lX = kPPosTarget.x;
	packet.lY = kPPosTarget.y;

	__LocalPositionToGlobalPosition(packet.lX, packet.lY);

	if (!Send(sizeof(packet), &packet))
	{
		Tracen("Send FlyTargeting Packet Error");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::SendFlyTargetingPacket(DWORD dwTargetVID, const TPixelPosition& kPPosTarget)
{
	TPacketCGFlyTargeting packet;

	//CPythonCharacterManager & rpcm = CPythonCharacterManager::Instance();

	packet.bHeader = HEADER_CG_FLY_TARGETING;
	packet.dwTargetVID = dwTargetVID;
	packet.lX = kPPosTarget.x;
	packet.lY = kPPosTarget.y;

	__LocalPositionToGlobalPosition(packet.lX, packet.lY);

	if (!Send(sizeof(packet), &packet))
	{
		Tracen("Send FlyTargeting Packet Error");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::RecvCreateFlyPacket()
{
	TPacketGCCreateFly kPacket;
	if (!Recv(sizeof(TPacketGCCreateFly), &kPacket))
		return false;

	CFlyingManager& rkFlyMgr = CFlyingManager::Instance();
	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();

	CInstanceBase* pkStartInst = rkChrMgr.GetInstancePtr(kPacket.dwStartVID);
	CInstanceBase* pkEndInst = rkChrMgr.GetInstancePtr(kPacket.dwEndVID);
	if (!pkStartInst || !pkEndInst)
		return true;

	rkFlyMgr.CreateIndexedFly(kPacket.bType, pkStartInst->GetGraphicThingInstancePtr(), pkEndInst->GetGraphicThingInstancePtr());

	return true;
}

bool CPythonNetworkStream::SendTargetPacket(DWORD dwVID)
{
	TPacketCGTarget packet;
	packet.header = HEADER_CG_TARGET;
	packet.dwVID = dwVID;

	if (!Send(sizeof(packet), &packet))
	{
		Tracen("Send Target Packet Error");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::SendSyncPositionElementPacket(DWORD dwVictimVID, DWORD dwVictimX, DWORD dwVictimY)
{
	TPacketCGSyncPositionElement kSyncPos;
	kSyncPos.dwVID = dwVictimVID;
	kSyncPos.lX = dwVictimX;
	kSyncPos.lY = dwVictimY;

	__LocalPositionToGlobalPosition(kSyncPos.lX, kSyncPos.lY);

	if (!Send(sizeof(kSyncPos), &kSyncPos))
	{
		Tracen("CPythonNetworkStream::SendSyncPositionElementPacket - ERROR");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::RecvMessenger()
{
	TPacketGCMessenger p;
	if (!Recv(sizeof(p), &p))
		return false;

	int iSize = p.size - sizeof(p);
	// paket hatasi icin 24 > 64
	char char_name[CHARACTER_NAME_MAX_LEN + 1];

	switch (p.subheader)
	{
	case MESSENGER_SUBHEADER_GC_LIST:
	{
		TPacketGCMessengerListOnline on;
		while (iSize)
		{
			if (!Recv(sizeof(TPacketGCMessengerListOffline), &on))
				return false;

			if (!Recv(on.length, char_name))
				return false;

			char_name[on.length] = 0;

			if (on.connected & MESSENGER_CONNECTED_STATE_ONLINE)
				CPythonMessenger::Instance().OnFriendLogin(char_name);
			else
				CPythonMessenger::Instance().OnFriendLogout(char_name);

			iSize -= sizeof(TPacketGCMessengerListOffline);
			iSize -= on.length;
		}
		break;
	}

	case MESSENGER_SUBHEADER_GC_LOGIN:
	{
		TPacketGCMessengerLogin p;
		if (!Recv(sizeof(p), &p))
			return false;
		if (!Recv(p.length, char_name))
			return false;
		char_name[p.length] = 0;
		CPythonMessenger::Instance().OnFriendLogin(char_name);
		__RefreshTargetBoardByName(char_name);
		break;
	}

	case MESSENGER_SUBHEADER_GC_LOGOUT:
	{
		TPacketGCMessengerLogout logout;
		if (!Recv(sizeof(logout), &logout))
			return false;
		if (!Recv(logout.length, char_name))
			return false;
		char_name[logout.length] = 0;
		CPythonMessenger::Instance().OnFriendLogout(char_name);
		break;
	}

	// @duzenleme messengerden adam silindiginde paket geliyor.
	case MESSENGER_SUBHEADER_GC_REMOVE_FRIEND:
	{
		BYTE bLength;
		if (!Recv(sizeof(bLength), &bLength))
			return false;

		if (!Recv(bLength, char_name))
			return false;

		char_name[bLength] = 0;
		CPythonMessenger::Instance().RemoveFriend(char_name);
		break;
	}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Party

bool CPythonNetworkStream::SendPartyInvitePacket(DWORD dwVID)
{
	TPacketCGPartyInvite kPartyInvitePacket;
	kPartyInvitePacket.header = HEADER_CG_PARTY_INVITE;
	kPartyInvitePacket.vid = dwVID;

	if (!Send(sizeof(kPartyInvitePacket), &kPartyInvitePacket))
	{
		Tracenf("CPythonNetworkStream::SendPartyInvitePacket [%ud] - PACKET SEND ERROR", dwVID);
		return false;
	}

	Tracef(" << SendPartyInvitePacket : %d\n", dwVID);
	return true;
}

bool CPythonNetworkStream::SendPartyInviteAnswerPacket(DWORD dwLeaderVID, BYTE byAnswer)
{
	TPacketCGPartyInviteAnswer kPartyInviteAnswerPacket;
	kPartyInviteAnswerPacket.header = HEADER_CG_PARTY_INVITE_ANSWER;
	kPartyInviteAnswerPacket.leader_pid = dwLeaderVID;
	kPartyInviteAnswerPacket.accept = byAnswer;

	if (!Send(sizeof(kPartyInviteAnswerPacket), &kPartyInviteAnswerPacket))
	{
		Tracenf("CPythonNetworkStream::SendPartyInviteAnswerPacket [%ud %ud] - PACKET SEND ERROR", dwLeaderVID, byAnswer);
		return false;
	}

	Tracef(" << SendPartyInviteAnswerPacket : %d, %d\n", dwLeaderVID, byAnswer);
	return true;
}

bool CPythonNetworkStream::SendPartyRemovePacket(DWORD dwPID)
{
	TPacketCGPartyRemove kPartyInviteRemove;
	kPartyInviteRemove.header = HEADER_CG_PARTY_REMOVE;
	kPartyInviteRemove.pid = dwPID;

	if (!Send(sizeof(kPartyInviteRemove), &kPartyInviteRemove))
	{
		Tracenf("CPythonNetworkStream::SendPartyRemovePacket [%ud] - PACKET SEND ERROR", dwPID);
		return false;
	}

	Tracef(" << SendPartyRemovePacket : %d\n", dwPID);
	return true;
}

bool CPythonNetworkStream::SendPartySetStatePacket(DWORD dwVID, BYTE byState, BYTE byFlag)
{
	TPacketCGPartySetState kPartySetState;
	kPartySetState.byHeader = HEADER_CG_PARTY_SET_STATE;
	kPartySetState.dwVID = dwVID;
	kPartySetState.byState = byState;
	kPartySetState.byFlag = byFlag;

	if (!Send(sizeof(kPartySetState), &kPartySetState))
	{
		Tracenf("CPythonNetworkStream::SendPartySetStatePacket(%ud, %ud) - PACKET SEND ERROR", dwVID, byState);
		return false;
	}

	Tracef(" << SendPartySetStatePacket : %d, %d, %d\n", dwVID, byState, byFlag);
	return true;
}

bool CPythonNetworkStream::SendPartyParameterPacket(BYTE byDistributeMode)
{
	TPacketCGPartyParameter kPartyParameter;
	kPartyParameter.bHeader = HEADER_CG_PARTY_PARAMETER;
	kPartyParameter.bDistributeMode = byDistributeMode;

	if (!Send(sizeof(kPartyParameter), &kPartyParameter))
	{
		Tracenf("CPythonNetworkStream::SendPartyParameterPacket(%d) - PACKET SEND ERROR", byDistributeMode);
		return false;
	}

	Tracef(" << SendPartyParameterPacket : %d\n", byDistributeMode);
	return true;
}

bool CPythonNetworkStream::RecvPartyInvite()
{
	TPacketGCPartyInvite kPartyInvitePacket;
	if (!Recv(sizeof(kPartyInvitePacket), &kPartyInvitePacket))
		return false;

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(kPartyInvitePacket.leader_pid);
	if (!pInstance)
	{
		TraceError(" CPythonNetworkStream::RecvPartyInvite - Failed to find leader instance [%d]\n", kPartyInvitePacket.leader_pid);
		return true;
	}

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RecvPartyInviteQuestion", Py_BuildValue("(is)", kPartyInvitePacket.leader_pid, pInstance->GetNameString()));
	Tracef(" >> RecvPartyInvite : %d, %s\n", kPartyInvitePacket.leader_pid, pInstance->GetNameString());

	return true;
}

bool CPythonNetworkStream::RecvPartyAdd()
{
	TPacketGCPartyAdd kPartyAddPacket;
	if (!Recv(sizeof(kPartyAddPacket), &kPartyAddPacket))
		return false;

	CPythonPlayer::Instance().AppendPartyMember(kPartyAddPacket.pid, kPartyAddPacket.name);
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "AddPartyMember", Py_BuildValue("(is)", kPartyAddPacket.pid, kPartyAddPacket.name));
	Tracef(" >> RecvPartyAdd : %d, %s\n", kPartyAddPacket.pid, kPartyAddPacket.name);

	return true;
}

bool CPythonNetworkStream::RecvPartyUpdate()
{
	TPacketGCPartyUpdate kPartyUpdatePacket;
	if (!Recv(sizeof(kPartyUpdatePacket), &kPartyUpdatePacket))
		return false;

	CPythonPlayer::TPartyMemberInfo* pPartyMemberInfo;
	if (!CPythonPlayer::Instance().GetPartyMemberPtr(kPartyUpdatePacket.pid, &pPartyMemberInfo))
		return true;

	BYTE byOldState = pPartyMemberInfo->byState;

	CPythonPlayer::Instance().UpdatePartyMemberInfo(kPartyUpdatePacket.pid, kPartyUpdatePacket.state, kPartyUpdatePacket.percent_hp);

	for (int i = 0; i < PARTY_AFFECT_SLOT_MAX_NUM; ++i)
	{
		CPythonPlayer::Instance().UpdatePartyMemberAffect(kPartyUpdatePacket.pid, i, kPartyUpdatePacket.affects[i]);
	}

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "UpdatePartyMemberInfo", Py_BuildValue("(i)", kPartyUpdatePacket.pid));

	DWORD dwVID;
	if (CPythonPlayer::Instance().PartyMemberPIDToVID(kPartyUpdatePacket.pid, &dwVID))
		if (byOldState != kPartyUpdatePacket.state)
		{
			__RefreshTargetBoardByVID(dwVID);
		}

	// 	Tracef(" >> RecvPartyUpdate : %d, %d, %d\n", kPartyUpdatePacket.pid, kPartyUpdatePacket.state, kPartyUpdatePacket.percent_hp);

	return true;
}

bool CPythonNetworkStream::RecvPartyRemove()
{
	TPacketGCPartyRemove kPartyRemovePacket;
	if (!Recv(sizeof(kPartyRemovePacket), &kPartyRemovePacket))
		return false;

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RemovePartyMember", Py_BuildValue("(i)", kPartyRemovePacket.pid));
	Tracef(" >> RecvPartyRemove : %d\n", kPartyRemovePacket.pid);

	return true;
}

bool CPythonNetworkStream::RecvPartyLink()
{
	TPacketGCPartyLink kPartyLinkPacket;
	if (!Recv(sizeof(kPartyLinkPacket), &kPartyLinkPacket))
		return false;

	CPythonPlayer::Instance().LinkPartyMember(kPartyLinkPacket.pid, kPartyLinkPacket.vid);
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "LinkPartyMember", Py_BuildValue("(ii)", kPartyLinkPacket.pid, kPartyLinkPacket.vid));
	Tracef(" >> RecvPartyLink : %d, %d\n", kPartyLinkPacket.pid, kPartyLinkPacket.vid);

	return true;
}

bool CPythonNetworkStream::RecvPartyUnlink()
{
	TPacketGCPartyUnlink kPartyUnlinkPacket;
	if (!Recv(sizeof(kPartyUnlinkPacket), &kPartyUnlinkPacket))
		return false;

	CPythonPlayer::Instance().UnlinkPartyMember(kPartyUnlinkPacket.pid);

	if (CPythonPlayer::Instance().IsMainCharacterIndex(kPartyUnlinkPacket.vid))
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "UnlinkAllPartyMember", Py_BuildValue("()"));
	}
	else
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "UnlinkPartyMember", Py_BuildValue("(i)", kPartyUnlinkPacket.pid));
	}

	Tracef(" >> RecvPartyUnlink : %d, %d\n", kPartyUnlinkPacket.pid, kPartyUnlinkPacket.vid);

	return true;
}

bool CPythonNetworkStream::RecvPartyParameter()
{
	TPacketGCPartyParameter kPartyParameterPacket;
	if (!Recv(sizeof(kPartyParameterPacket), &kPartyParameterPacket))
		return false;

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ChangePartyParameter", Py_BuildValue("(i)", kPartyParameterPacket.bDistributeMode));
	Tracef(" >> RecvPartyParameter : %d\n", kPartyParameterPacket.bDistributeMode);

	return true;
}

// Party
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Guild

bool CPythonNetworkStream::SendGuildAddMemberPacket(DWORD dwVID)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_ADD_MEMBER;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;
	if (!Send(sizeof(dwVID), &dwVID))
		return false;

#ifdef _DEBUG
	Tracef(" SendGuildAddMemberPacket\n", dwVID);
#endif
	return true;
}

bool CPythonNetworkStream::SendGuildRemoveMemberPacket(DWORD dwPID)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_REMOVE_MEMBER;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;
	if (!Send(sizeof(dwPID), &dwPID))
		return false;

	Tracef(" SendGuildRemoveMemberPacket %d\n", dwPID);
	return true;
}

bool CPythonNetworkStream::SendGuildChangeGradeNamePacket(BYTE byGradeNumber, const char* c_szName)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_CHANGE_GRADE_NAME;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;
	if (!Send(sizeof(byGradeNumber), &byGradeNumber))
		return false;

	char szName[GUILD_GRADE_NAME_MAX_LEN + 1];
	strncpy(szName, c_szName, GUILD_GRADE_NAME_MAX_LEN);
	szName[GUILD_GRADE_NAME_MAX_LEN] = '\0';

	if (!Send(sizeof(szName), &szName))
		return false;

	Tracef(" SendGuildChangeGradeNamePacket %d, %s\n", byGradeNumber, c_szName);
	return true;
}

bool CPythonNetworkStream::SendGuildChangeGradeAuthorityPacket(BYTE byGradeNumber, BYTE byAuthority)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_CHANGE_GRADE_AUTHORITY;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;
	if (!Send(sizeof(byGradeNumber), &byGradeNumber))
		return false;
	if (!Send(sizeof(byAuthority), &byAuthority))
		return false;

	Tracef(" SendGuildChangeGradeAuthorityPacket %d, %d\n", byGradeNumber, byAuthority);
	return true;
}

bool CPythonNetworkStream::SendGuildOfferPacket(DWORD dwExperience)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_OFFER;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;
	if (!Send(sizeof(dwExperience), &dwExperience))
		return false;

#ifdef _DEBUG
	Tracef(" SendGuildOfferPacket %d\n", dwExperience);
#endif
	return true;
}

bool CPythonNetworkStream::SendGuildPostCommentPacket(const char* c_szMessage)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_POST_COMMENT;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;

	BYTE bySize = BYTE(strlen(c_szMessage)) + 1;
	if (!Send(sizeof(bySize), &bySize))
		return false;
	if (!Send(bySize, c_szMessage))
		return false;

#ifdef _DEBUG
	Tracef(" SendGuildPostCommentPacket %d, %s\n", bySize, c_szMessage);
#endif
	return true;
}

bool CPythonNetworkStream::SendGuildDeleteCommentPacket(DWORD dwIndex)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_DELETE_COMMENT;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;

	if (!Send(sizeof(dwIndex), &dwIndex))
		return false;

	Tracef(" SendGuildDeleteCommentPacket %d\n", dwIndex);
	return true;
}

bool CPythonNetworkStream::SendGuildRefreshCommentsPacket(DWORD dwHighestIndex)
{
	static DWORD s_LastTime = timeGetTime() - 1001;

	if (timeGetTime() - s_LastTime < 1000)
		return true;
	s_LastTime = timeGetTime();

	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_REFRESH_COMMENT;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;

#ifdef _DEBUG
	Tracef(" SendGuildRefreshCommentPacket %d\n", dwHighestIndex);
#endif
	return true;
}

bool CPythonNetworkStream::SendGuildChangeMemberGradePacket(DWORD dwPID, BYTE byGrade)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_CHANGE_MEMBER_GRADE;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;

	if (!Send(sizeof(dwPID), &dwPID))
		return false;
	if (!Send(sizeof(byGrade), &byGrade))
		return false;

	Tracef(" SendGuildChangeMemberGradePacket %d, %d\n", dwPID, byGrade);
	return true;
}

bool CPythonNetworkStream::SendGuildUseSkillPacket(DWORD dwSkillID, DWORD dwTargetVID)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_USE_SKILL;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;

	if (!Send(sizeof(dwSkillID), &dwSkillID))
		return false;
	if (!Send(sizeof(dwTargetVID), &dwTargetVID))
		return false;

	Tracef(" SendGuildUseSkillPacket %d, %d\n", dwSkillID, dwTargetVID);
	return true;
}

bool CPythonNetworkStream::SendGuildChangeMemberGeneralPacket(DWORD dwPID, BYTE byFlag)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_CHANGE_MEMBER_GENERAL;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;

	if (!Send(sizeof(dwPID), &dwPID))
		return false;
	if (!Send(sizeof(byFlag), &byFlag))
		return false;

	Tracef(" SendGuildChangeMemberGeneralFlagPacket %d, %d\n", dwPID, byFlag);
	return true;
}

bool CPythonNetworkStream::SendGuildInviteAnswerPacket(DWORD dwGuildID, BYTE byAnswer)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_GUILD_INVITE_ANSWER;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;

	if (!Send(sizeof(dwGuildID), &dwGuildID))
		return false;
	if (!Send(sizeof(byAnswer), &byAnswer))
		return false;

	Tracef(" SendGuildInviteAnswerPacket %d, %d\n", dwGuildID, byAnswer);
	return true;
}

#ifdef ENABLE_GOLD_LIMIT_REWORK
bool CPythonNetworkStream::SendGuildChargeGSPPacket(long long llMoney)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_CHARGE_GSP;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;

	if (!Send(sizeof(llMoney), &llMoney))
		return false;

	Tracef(" SendGuildChargeGSPPacket %lld\n", llMoney);
	return true;
}

bool CPythonNetworkStream::SendGuildDepositMoneyPacket(long long llMoney)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_DEPOSIT_MONEY;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;
	if (!Send(sizeof(llMoney), &llMoney))
		return false;

	Tracef(" SendGuildDepositMoneyPacket %lld\n", llMoney);
	return true;
}

bool CPythonNetworkStream::SendGuildWithdrawMoneyPacket(long long llMoney)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_WITHDRAW_MONEY;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;
	if (!Send(sizeof(llMoney), &llMoney))
		return false;

	Tracef(" SendGuildWithdrawMoneyPacket %lld\n", llMoney);
	return true;
}
#else
bool CPythonNetworkStream::SendGuildChargeGSPPacket(DWORD dwMoney)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_CHARGE_GSP;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;

	if (!Send(sizeof(dwMoney), &dwMoney))
		return false;

	Tracef(" SendGuildChargeGSPPacket %d\n", dwMoney);
	return true;
}

bool CPythonNetworkStream::SendGuildDepositMoneyPacket(DWORD dwMoney)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_DEPOSIT_MONEY;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;
	if (!Send(sizeof(dwMoney), &dwMoney))
		return false;

	Tracef(" SendGuildDepositMoneyPacket %d\n", dwMoney);
	return true;
}

bool CPythonNetworkStream::SendGuildWithdrawMoneyPacket(DWORD dwMoney)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_WITHDRAW_MONEY;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;
	if (!Send(sizeof(dwMoney), &dwMoney))
		return false;

	Tracef(" SendGuildWithdrawMoneyPacket %d\n", dwMoney);
	return true;
}
#endif
bool CPythonNetworkStream::RecvGuild()
{
	TPacketGCGuild GuildPacket;
	if (!Recv(sizeof(GuildPacket), &GuildPacket))
		return false;

	switch (GuildPacket.subheader)
	{
	case GUILD_SUBHEADER_GC_LOGIN:
	{
		DWORD dwPID;
		if (!Recv(sizeof(DWORD), &dwPID))
			return false;

		// Messenger
		CPythonGuild::TGuildMemberData* pGuildMemberData;
		if (CPythonGuild::Instance().GetMemberDataPtrByPID(dwPID, &pGuildMemberData))
			if (0 != pGuildMemberData->strName.compare(CPythonPlayer::Instance().GetName()))
				CPythonMessenger::Instance().LoginGuildMember(pGuildMemberData->strName.c_str());

		//Tracef(" <Login> %d\n", dwPID);
		break;
	}
	case GUILD_SUBHEADER_GC_LOGOUT:
	{
		DWORD dwPID;
		if (!Recv(sizeof(DWORD), &dwPID))
			return false;

		// Messenger
		CPythonGuild::TGuildMemberData* pGuildMemberData;
		if (CPythonGuild::Instance().GetMemberDataPtrByPID(dwPID, &pGuildMemberData))
			if (0 != pGuildMemberData->strName.compare(CPythonPlayer::Instance().GetName()))
				CPythonMessenger::Instance().LogoutGuildMember(pGuildMemberData->strName.c_str());

		//Tracef(" <Logout> %d\n", dwPID);
		break;
	}
	case GUILD_SUBHEADER_GC_REMOVE:
	{
		DWORD dwPID;
		if (!Recv(sizeof(dwPID), &dwPID))
			return false;

		if (CPythonGuild::Instance().IsMainPlayer(dwPID))
		{
			CPythonGuild::Instance().Destroy();
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "DeleteGuild", Py_BuildValue("()"));
			CPythonMessenger::Instance().RemoveAllGuildMember();
			__SetGuildID(0);
			__RefreshMessengerWindow();
			__RefreshTargetBoard();
			__RefreshCharacterWindow();
		}
		else
		{
			// Get Member Name
			std::string strMemberName = "";
			CPythonGuild::TGuildMemberData* pData;
			if (CPythonGuild::Instance().GetMemberDataPtrByPID(dwPID, &pData))
			{
				strMemberName = pData->strName;
				CPythonMessenger::Instance().RemoveGuildMember(pData->strName.c_str());
			}

			CPythonGuild::Instance().RemoveMember(dwPID);

			// Refresh
			__RefreshTargetBoardByName(strMemberName.c_str());
			__RefreshGuildWindowMemberPage();
		}

		Tracef(" <Remove> %d\n", dwPID);
		break;
	}
	case GUILD_SUBHEADER_GC_LIST:
	{
		int iPacketSize = int(GuildPacket.size) - sizeof(GuildPacket);

		for (; iPacketSize > 0;)
		{
			TPacketGCGuildSubMember memberPacket;
			if (!Recv(sizeof(memberPacket), &memberPacket))
				return false;

			char szName[CHARACTER_NAME_MAX_LEN + 1] = "";
			if (memberPacket.byNameFlag)
			{
				if (!Recv(sizeof(szName), &szName))
					return false;

				iPacketSize -= CHARACTER_NAME_MAX_LEN + 1;
			}
			else
			{
				CPythonGuild::TGuildMemberData* pMemberData;
				if (CPythonGuild::Instance().GetMemberDataPtrByPID(memberPacket.pid, &pMemberData))
				{
					strncpy(szName, pMemberData->strName.c_str(), CHARACTER_NAME_MAX_LEN);
				}
			}

			//Tracef(" <List> %d : %s, %d (%d, %d, %d)\n", memberPacket.pid, szName, memberPacket.byGrade, memberPacket.byJob, memberPacket.byLevel, memberPacket.dwOffer);

			CPythonGuild::SGuildMemberData GuildMemberData;
			GuildMemberData.dwPID = memberPacket.pid;
			GuildMemberData.byGrade = memberPacket.byGrade;
			GuildMemberData.strName = szName;
			GuildMemberData.byJob = memberPacket.byJob;
			GuildMemberData.byLevel = memberPacket.byLevel;
			GuildMemberData.dwOffer = memberPacket.dwOffer;
			GuildMemberData.byGeneralFlag = memberPacket.byIsGeneral;
			CPythonGuild::Instance().RegisterMember(GuildMemberData);

			// Messenger
			if (strcmp(szName, CPythonPlayer::Instance().GetName()))
				CPythonMessenger::Instance().AppendGuildMember(szName);

			__RefreshTargetBoardByName(szName);

			iPacketSize -= sizeof(memberPacket);
		}

		__RefreshGuildWindowInfoPage();
		__RefreshGuildWindowMemberPage();
		__RefreshMessengerWindow();
		__RefreshCharacterWindow();
		break;
	}
	case GUILD_SUBHEADER_GC_GRADE:
	{
		BYTE byCount;
		if (!Recv(sizeof(byCount), &byCount))
			return false;

		for (BYTE i = 0; i < byCount; ++i)
		{
			BYTE byIndex;
			if (!Recv(sizeof(byCount), &byIndex))
				return false;
			TPacketGCGuildSubGrade GradePacket;
			if (!Recv(sizeof(GradePacket), &GradePacket))
				return false;

			CPythonGuild::Instance().SetGradeData(byIndex, CPythonGuild::SGuildGradeData(GradePacket.auth_flag, GradePacket.grade_name));
			//Tracef(" <Grade> [%d/%d] : %s, %d\n", byIndex, byCount, GradePacket.grade_name, GradePacket.auth_flag);
		}
		__RefreshGuildWindowGradePage();
		__RefreshGuildWindowMemberPageGradeComboBox();
		break;
	}
	case GUILD_SUBHEADER_GC_GRADE_NAME:
	{
		BYTE byGradeNumber;
		if (!Recv(sizeof(byGradeNumber), &byGradeNumber))
			return false;

		char szGradeName[GUILD_GRADE_NAME_MAX_LEN + 1] = "";
		if (!Recv(sizeof(szGradeName), &szGradeName))
			return false;

		CPythonGuild::Instance().SetGradeName(byGradeNumber, szGradeName);
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshGuildGrade", Py_BuildValue("()"));

		Tracef(" <Change Grade Name> %d, %s\n", byGradeNumber, szGradeName);
		__RefreshGuildWindowGradePage();
		__RefreshGuildWindowMemberPageGradeComboBox();
		break;
	}
	case GUILD_SUBHEADER_GC_GRADE_AUTH:
	{
		BYTE byGradeNumber;
		if (!Recv(sizeof(byGradeNumber), &byGradeNumber))
			return false;
		BYTE byAuthorityFlag;
		if (!Recv(sizeof(byAuthorityFlag), &byAuthorityFlag))
			return false;

		CPythonGuild::Instance().SetGradeAuthority(byGradeNumber, byAuthorityFlag);
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshGuildGrade", Py_BuildValue("()"));

		Tracef(" <Change Grade Authority> %d, %d\n", byGradeNumber, byAuthorityFlag);
		__RefreshGuildWindowGradePage();
		break;
	}
	case GUILD_SUBHEADER_GC_INFO:
	{
		TPacketGCGuildInfo GuildInfo;
		if (!Recv(sizeof(GuildInfo), &GuildInfo))
			return false;

		CPythonGuild::Instance().EnableGuild();
		CPythonGuild::TGuildInfo& rGuildInfo = CPythonGuild::Instance().GetGuildInfoRef();
		strncpy(rGuildInfo.szGuildName, GuildInfo.name, GUILD_NAME_MAX_LEN);
		rGuildInfo.szGuildName[GUILD_NAME_MAX_LEN] = '\0';

		rGuildInfo.dwGuildID = GuildInfo.guild_id;
		rGuildInfo.dwMasterPID = GuildInfo.master_pid;
		rGuildInfo.dwGuildLevel = GuildInfo.level;
		rGuildInfo.dwCurrentExperience = GuildInfo.exp;
		rGuildInfo.dwCurrentMemberCount = GuildInfo.member_count;
		rGuildInfo.dwMaxMemberCount = GuildInfo.max_member_count;

		__RefreshGuildWindowInfoPage();
		break;
	}
	case GUILD_SUBHEADER_GC_COMMENTS:
	{
		BYTE byCount;
		if (!Recv(sizeof(byCount), &byCount))
			return false;

		CPythonGuild::Instance().ClearComment();
		//Tracef(" >>> Comments Count : %d\n", byCount);

		for (BYTE i = 0; i < byCount; ++i)
		{
			DWORD dwCommentID;
			if (!Recv(sizeof(dwCommentID), &dwCommentID))
				return false;

			char szName[CHARACTER_NAME_MAX_LEN + 1] = "";
			if (!Recv(sizeof(szName), &szName))
				return false;

			char szComment[GULID_COMMENT_MAX_LEN + 1] = "";
			if (!Recv(sizeof(szComment), &szComment))
				return false;

			//Tracef(" [Comment-%d] : %s, %s\n", dwCommentID, szName, szComment);
			CPythonGuild::Instance().RegisterComment(dwCommentID, szName, szComment);
		}

		__RefreshGuildWindowBoardPage();
		break;
	}
	case GUILD_SUBHEADER_GC_CHANGE_EXP:
	{
		BYTE byLevel;
		if (!Recv(sizeof(byLevel), &byLevel))
			return false;
		DWORD dwEXP;
		if (!Recv(sizeof(dwEXP), &dwEXP))
			return false;
		CPythonGuild::Instance().SetGuildEXP(byLevel, dwEXP);
		Tracef(" <ChangeEXP> %d, %d\n", byLevel, dwEXP);
		__RefreshGuildWindowInfoPage();
		break;
	}
	case GUILD_SUBHEADER_GC_CHANGE_MEMBER_GRADE:
	{
		DWORD dwPID;
		if (!Recv(sizeof(dwPID), &dwPID))
			return false;
		BYTE byGrade;
		if (!Recv(sizeof(byGrade), &byGrade))
			return false;
		CPythonGuild::Instance().ChangeGuildMemberGrade(dwPID, byGrade);
		Tracef(" <ChangeMemberGrade> %d, %d\n", dwPID, byGrade);
		__RefreshGuildWindowMemberPage();
		break;
	}
	case GUILD_SUBHEADER_GC_SKILL_INFO:
	{
		CPythonGuild::TGuildSkillData& rSkillData = CPythonGuild::Instance().GetGuildSkillDataRef();
		if (!Recv(sizeof(rSkillData.bySkillPoint), &rSkillData.bySkillPoint))
			return false;
		if (!Recv(sizeof(rSkillData.bySkillLevel), rSkillData.bySkillLevel))
			return false;
		if (!Recv(sizeof(rSkillData.wGuildPoint), &rSkillData.wGuildPoint))
			return false;
		if (!Recv(sizeof(rSkillData.wMaxGuildPoint), &rSkillData.wMaxGuildPoint))
			return false;

		Tracef(" <SkillInfo> %d / %d, %d\n", rSkillData.bySkillPoint, rSkillData.wGuildPoint, rSkillData.wMaxGuildPoint);
		__RefreshGuildWindowSkillPage();
		break;
	}
	case GUILD_SUBHEADER_GC_CHANGE_MEMBER_GENERAL:
	{
		DWORD dwPID;
		if (!Recv(sizeof(dwPID), &dwPID))
			return false;
		BYTE byFlag;
		if (!Recv(sizeof(byFlag), &byFlag))
			return false;

		CPythonGuild::Instance().ChangeGuildMemberGeneralFlag(dwPID, byFlag);
		Tracef(" <ChangeMemberGeneralFlag> %d, %d\n", dwPID, byFlag);
		__RefreshGuildWindowMemberPage();
		break;
	}
	case GUILD_SUBHEADER_GC_GUILD_INVITE:
	{
		DWORD dwGuildID;
		if (!Recv(sizeof(dwGuildID), &dwGuildID))
			return false;
		char szGuildName[GUILD_NAME_MAX_LEN + 1];
		if (!Recv(GUILD_NAME_MAX_LEN + 1, &szGuildName))
			return false;

		szGuildName[GUILD_NAME_MAX_LEN] = 0;

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RecvGuildInviteQuestion", Py_BuildValue("(is)", dwGuildID, szGuildName));
		Tracef(" <Guild Invite> %d, %s\n", dwGuildID, szGuildName);
		break;
	}
	case GUILD_SUBHEADER_GC_WAR:
	{
		TPacketGCGuildWar kGuildWar;
		if (!Recv(sizeof(kGuildWar), &kGuildWar))
			return false;

		switch (kGuildWar.bWarState)
		{
		case GUILD_WAR_SEND_DECLARE:
			Tracef(" >> GUILD_SUBHEADER_GC_WAR : GUILD_WAR_SEND_DECLARE\n");
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME],
				"BINARY_GuildWar_OnSendDeclare",
				Py_BuildValue("(i)", kGuildWar.dwGuildOpp)
			);
			break;
		case GUILD_WAR_RECV_DECLARE:
			Tracef(" >> GUILD_SUBHEADER_GC_WAR : GUILD_WAR_RECV_DECLARE\n");
#ifdef ENABLE_GUILD_WAR_REWORK
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_GuildWar_OnRecvDeclare", Py_BuildValue("(iiii)", kGuildWar.dwGuildOpp, kGuildWar.bType, kGuildWar.iMaxPlayer, kGuildWar.iMaxScore));
#else
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_GuildWar_OnRecvDeclare", Py_BuildValue("(ii)", kGuildWar.dwGuildOpp, kGuildWar.bType));
#endif
			break;
		case GUILD_WAR_ON_WAR:
			Tracef(" >> GUILD_SUBHEADER_GC_WAR : GUILD_WAR_ON_WAR : %d, %d\n", kGuildWar.dwGuildSelf, kGuildWar.dwGuildOpp);
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME],
				"BINARY_GuildWar_OnStart",
				Py_BuildValue("(ii)", kGuildWar.dwGuildSelf, kGuildWar.dwGuildOpp)
			);
			CPythonGuild::Instance().StartGuildWar(kGuildWar.dwGuildOpp);
			break;
		case GUILD_WAR_END:
			Tracef(" >> GUILD_SUBHEADER_GC_WAR : GUILD_WAR_END\n");
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME],
				"BINARY_GuildWar_OnEnd",
				Py_BuildValue("(ii)", kGuildWar.dwGuildSelf, kGuildWar.dwGuildOpp)
			);
			CPythonGuild::Instance().EndGuildWar(kGuildWar.dwGuildOpp);
			break;
		}
		break;
	}
	case GUILD_SUBHEADER_GC_GUILD_NAME:
	{
		DWORD dwID;
		char szGuildName[GUILD_NAME_MAX_LEN + 1];

		int iPacketSize = int(GuildPacket.size) - sizeof(GuildPacket);

		int nItemSize = sizeof(dwID) + GUILD_NAME_MAX_LEN;

		assert(iPacketSize % nItemSize == 0 && "GUILD_SUBHEADER_GC_GUILD_NAME");

		for (; iPacketSize > 0;)
		{
			if (!Recv(sizeof(dwID), &dwID))
				return false;

			if (!Recv(GUILD_NAME_MAX_LEN, &szGuildName))
				return false;

			szGuildName[GUILD_NAME_MAX_LEN] = 0;

			//Tracef(" >> GulidName [%d : %s]\n", dwID, szGuildName);
			CPythonGuild::Instance().RegisterGuildName(dwID, szGuildName);
			iPacketSize -= nItemSize;
		}
		break;
	}
	case GUILD_SUBHEADER_GC_GUILD_WAR_LIST:
	{
		DWORD dwSrcGuildID;
		DWORD dwDstGuildID;

		int iPacketSize = int(GuildPacket.size) - sizeof(GuildPacket);
		int nItemSize = sizeof(dwSrcGuildID) + sizeof(dwDstGuildID);

		assert(iPacketSize % nItemSize == 0 && "GUILD_SUBHEADER_GC_GUILD_WAR_LIST");

		for (; iPacketSize > 0;)
		{
			if (!Recv(sizeof(dwSrcGuildID), &dwSrcGuildID))
				return false;

			if (!Recv(sizeof(dwDstGuildID), &dwDstGuildID))
				return false;

			Tracef(" >> GulidWarList [%d vs %d]\n", dwSrcGuildID, dwDstGuildID);
			CInstanceBase::InsertGVGKey(dwSrcGuildID, dwDstGuildID);
			CPythonCharacterManager::Instance().ChangeGVG(dwSrcGuildID, dwDstGuildID);
			iPacketSize -= nItemSize;
		}
		break;
	}
	case GUILD_SUBHEADER_GC_GUILD_WAR_END_LIST:
	{
		DWORD dwSrcGuildID;
		DWORD dwDstGuildID;

		int iPacketSize = int(GuildPacket.size) - sizeof(GuildPacket);
		int nItemSize = sizeof(dwSrcGuildID) + sizeof(dwDstGuildID);

		assert(iPacketSize % nItemSize == 0 && "GUILD_SUBHEADER_GC_GUILD_WAR_END_LIST");

		for (; iPacketSize > 0;)
		{
			if (!Recv(sizeof(dwSrcGuildID), &dwSrcGuildID))
				return false;

			if (!Recv(sizeof(dwDstGuildID), &dwDstGuildID))
				return false;

			Tracef(" >> GulidWarEndList [%d vs %d]\n", dwSrcGuildID, dwDstGuildID);
			CInstanceBase::RemoveGVGKey(dwSrcGuildID, dwDstGuildID);
			CPythonCharacterManager::Instance().ChangeGVG(dwSrcGuildID, dwDstGuildID);
			iPacketSize -= nItemSize;
		}
		break;
	}
	case GUILD_SUBHEADER_GC_WAR_POINT:
	{
		TPacketGuildWarPoint GuildWarPoint;
		if (!Recv(sizeof(GuildWarPoint), &GuildWarPoint))
			return false;

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME],
			"BINARY_GuildWar_OnRecvPoint",
			Py_BuildValue("(iii)", GuildWarPoint.dwGainGuildID, GuildWarPoint.dwOpponentGuildID, GuildWarPoint.lPoint)
		);
		break;
	}
	}

	return true;
}

// Guild
//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
// Fishing

bool CPythonNetworkStream::SendFishingPacket(int iRotation)
{
	BYTE byHeader = HEADER_CG_FISHING;
	if (!Send(sizeof(byHeader), &byHeader))
		return false;
	BYTE byPacketRotation = iRotation / 5;
	if (!Send(sizeof(BYTE), &byPacketRotation))
		return false;

	return true;
}

bool CPythonNetworkStream::SendGiveItemPacket(DWORD dwTargetVID, TItemPos ItemPos, int iItemCount)
{
	TPacketCGGiveItem GiveItemPacket;
	GiveItemPacket.byHeader = HEADER_CG_GIVE_ITEM;
	GiveItemPacket.dwTargetVID = dwTargetVID;
	GiveItemPacket.ItemPos = ItemPos;
	GiveItemPacket.byItemCount = iItemCount;

	if (!Send(sizeof(GiveItemPacket), &GiveItemPacket))
		return false;

	return true;
}

bool CPythonNetworkStream::RecvFishing()
{
	TPacketGCFishing FishingPacket;
	if (!Recv(sizeof(FishingPacket), &FishingPacket))
		return false;

	CInstanceBase* pFishingInstance = NULL;
	if (FISHING_SUBHEADER_GC_FISH != FishingPacket.subheader)
	{
		pFishingInstance = CPythonCharacterManager::Instance().GetInstancePtr(FishingPacket.info);
		if (!pFishingInstance)
			return true;
	}

	switch (FishingPacket.subheader)
	{
	case FISHING_SUBHEADER_GC_START:
		pFishingInstance->StartFishing(float(FishingPacket.dir) * 5.0f);
		break;
	case FISHING_SUBHEADER_GC_STOP:
		if (pFishingInstance->IsFishing())
			pFishingInstance->StopFishing();
		break;
	case FISHING_SUBHEADER_GC_REACT:
		if (pFishingInstance->IsFishing())
		{
			pFishingInstance->SetFishEmoticon(); // Fish Emoticon
			pFishingInstance->ReactFishing();
		}
		break;
	case FISHING_SUBHEADER_GC_SUCCESS:
		pFishingInstance->CatchSuccess();
		break;
	case FISHING_SUBHEADER_GC_FAIL:
		pFishingInstance->CatchFail();
		if (pFishingInstance == CPythonCharacterManager::Instance().GetMainInstancePtr())
		{
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnFishingFailure", Py_BuildValue("()"));
		}
		break;
	case FISHING_SUBHEADER_GC_FISH:
	{
		DWORD dwFishID = FishingPacket.info;

		if (0 == FishingPacket.info)
		{
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnFishingNotifyUnknown", Py_BuildValue("()"));
			return true;
		}

		CItemData* pItemData;
		if (!CItemManager::Instance().GetItemDataPointer(dwFishID, &pItemData))
			return true;

		CInstanceBase* pMainInstance = CPythonCharacterManager::Instance().GetMainInstancePtr();
		if (!pMainInstance)
			return true;

		if (pMainInstance->IsFishing())
		{
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnFishingNotify", Py_BuildValue("(is)", CItemData::ITEM_TYPE_FISH == pItemData->GetType(), pItemData->GetName()));
		}
		else
		{
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnFishingSuccess", Py_BuildValue("(is)", CItemData::ITEM_TYPE_FISH == pItemData->GetType(), pItemData->GetName()));
		}
		break;
	}
	}

	return true;
}
// Fishing
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
// Dungeon
bool CPythonNetworkStream::RecvDungeon()
{
	TPacketGCDungeon DungeonPacket;
	if (!Recv(sizeof(DungeonPacket), &DungeonPacket))
		return false;

	switch (DungeonPacket.subheader)
	{
	case DUNGEON_SUBHEADER_GC_TIME_ATTACK_START:
	{
		break;
	}
	case DUNGEON_SUBHEADER_GC_DESTINATION_POSITION:
	{
		unsigned long ulx, uly;
		if (!Recv(sizeof(ulx), &ulx))
			return false;
		if (!Recv(sizeof(uly), &uly))
			return false;

		CPythonPlayer::Instance().SetDungeonDestinationPosition(ulx, uly);
		break;
	}
	}

	return true;
}
// Dungeon
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
// MyShop
bool CPythonNetworkStream::SendBuildPrivateShopPacket(const char* c_szName, const std::vector<TShopItemTable>& c_rSellingItemStock)
{
	TPacketCGMyShop packet;
	packet.bHeader = HEADER_CG_MYSHOP;
	strncpy(packet.szSign, c_szName, SHOP_SIGN_MAX_LEN);
	packet.bCount = static_cast<BYTE>(c_rSellingItemStock.size());
	if (!Send(sizeof(packet), &packet))
		return false;

	for (std::vector<TShopItemTable>::const_iterator itor = c_rSellingItemStock.begin(); itor < c_rSellingItemStock.end(); ++itor)
	{
		const TShopItemTable& c_rItem = *itor;
		if (!Send(sizeof(c_rItem), &c_rItem))
			return false;
	}

	return true;
}

bool CPythonNetworkStream::RecvShopSignPacket()
{
	TPacketGCShopSign p;
	if (!Recv(sizeof(TPacketGCShopSign), &p))
		return false;

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();

	if (0 == strlen(p.szSign))
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_PrivateShop_Disappear", Py_BuildValue("(i)", p.dwVID));

		if (rkPlayer.IsMainCharacterIndex(p.dwVID))
			rkPlayer.ClosePrivateShop();
	}
	else
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_PrivateShop_Appear", Py_BuildValue("(is)", p.dwVID, p.szSign));

		if (rkPlayer.IsMainCharacterIndex(p.dwVID))
			rkPlayer.OpenPrivateShop();
	}

	return true;
}
/////////////////////////////////////////////////////////////////////////

bool CPythonNetworkStream::RecvTimePacket()
{
	TPacketGCTime TimePacket;
	if (!Recv(sizeof(TimePacket), &TimePacket))
		return false;

	IAbstractApplication& rkApp = IAbstractApplication::GetSingleton();
	rkApp.SetServerTime(TimePacket.time);

	return true;
}

bool CPythonNetworkStream::RecvWalkModePacket()
{
	TPacketGCWalkMode WalkModePacket;
	if (!Recv(sizeof(WalkModePacket), &WalkModePacket))
		return false;

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(WalkModePacket.vid);
	if (pInstance)
	{
		if (WALKMODE_RUN == WalkModePacket.mode)
		{
			pInstance->SetRunMode();
		}
		else
		{
			pInstance->SetWalkMode();
		}
	}

	return true;
}

bool CPythonNetworkStream::RecvChangeSkillGroupPacket()
{
	TPacketGCChangeSkillGroup ChangeSkillGroup;
	if (!Recv(sizeof(ChangeSkillGroup), &ChangeSkillGroup))
		return false;

	m_dwMainActorSkillGroup = ChangeSkillGroup.skill_group;

	CPythonPlayer::Instance().NEW_ClearSkillData();
	__RefreshCharacterWindow();
	return true;
}

bool CPythonNetworkStream::SendRefinePacket(BYTE byPos, BYTE byType)
{
	TPacketCGRefine kRefinePacket;
	kRefinePacket.header = HEADER_CG_REFINE;
	kRefinePacket.pos = byPos;
	kRefinePacket.type = byType;

	if (!Send(sizeof(kRefinePacket), &kRefinePacket))
		return false;

	return true;
}

bool CPythonNetworkStream::SendSelectItemPacket(DWORD dwItemPos)
{
	TPacketCGScriptSelectItem kScriptSelectItem;
	kScriptSelectItem.header = HEADER_CG_SCRIPT_SELECT_ITEM;
	kScriptSelectItem.selection = dwItemPos;

	if (!Send(sizeof(kScriptSelectItem), &kScriptSelectItem))
		return false;

	return true;
}

bool CPythonNetworkStream::RecvRefineInformationPacketNew()
{
	TPacketGCRefineInformationNew kRefineInfoPacket;
	if (!Recv(sizeof(kRefineInfoPacket), &kRefineInfoPacket))
		return false;

	TRefineTable& rkRefineTable = kRefineInfoPacket.refine_table;
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME],
#ifdef ENABLE_REFINE_REWORK
#ifdef ENABLE_GOLD_LIMIT_REWORK
		"OpenRefineDialog", Py_BuildValue("(iiLiii)", kRefineInfoPacket.pos, kRefineInfoPacket.refine_table.result_vnum, rkRefineTable.cost, rkRefineTable.prob, rkRefineTable.success_prob, kRefineInfoPacket.type)
#else
		"OpenRefineDialog", Py_BuildValue("(iiiiii)", kRefineInfoPacket.pos, kRefineInfoPacket.refine_table.result_vnum, rkRefineTable.cost, rkRefineTable.prob, rkRefineTable.success_prob, kRefineInfoPacket.type)
#endif
#else
		"OpenRefineDialog", Py_BuildValue("(iiiii)", kRefineInfoPacket.pos, kRefineInfoPacket.refine_table.result_vnum, rkRefineTable.cost, rkRefineTable.prob, kRefineInfoPacket.type)
#endif
	);

	for (int i = 0; i < rkRefineTable.material_count; ++i)
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "AppendMaterialToRefineDialog", Py_BuildValue("(ii)", rkRefineTable.materials[i].vnum, rkRefineTable.materials[i].count));
	}

#ifdef _DEBUG
#ifdef ENABLE_GOLD_LIMIT_REWORK
	Tracef(" >> RecvRefineInformationPacketNew(pos=%d, result_vnum=%d, cost=%lld, prob=%d, type=%d)\n",
#else
	Tracef(" >> RecvRefineInformationPacketNew(pos=%d, result_vnum=%d, cost=%d, prob=%d, type=%d)\n",
#endif
		kRefineInfoPacket.pos,
		kRefineInfoPacket.refine_table.result_vnum,
		rkRefineTable.cost,
		rkRefineTable.prob,
		kRefineInfoPacket.type);
#endif

	return true;
}

bool CPythonNetworkStream::RecvAffectAddPacket()
{
	TPacketGCAffectAdd kAffectAdd;
	if (!Recv(sizeof(kAffectAdd), &kAffectAdd))
		return false;

	TPacketAffectElement& rkElement = kAffectAdd.elem;
	if (rkElement.bPointIdxApplyOn == POINT_ENERGY)
	{
		CPythonPlayer::instance().SetStatus(POINT_ENERGY_END_TIME, CPythonApplication::Instance().GetServerTimeStamp() + rkElement.lDuration);
		__RefreshStatus();
	}
#ifdef ENABLE_PB2_PREMIUM_SYSTEM
	if (rkElement.dwType == CInstanceBase::AFFECT_PB2_GLOBAL_CHAT)
		CPythonPlayer::instance().SetPB2GlobalChat(true);
#endif // ENABLE_PB2_PREMIUM_SYSTEM
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_NEW_AddAffect", Py_BuildValue("(iiiii)", rkElement.dwType, rkElement.bPointIdxApplyOn, rkElement.lApplyValue, rkElement.lDuration, kAffectAdd.bByLoad));

	return true;
}

bool CPythonNetworkStream::RecvAffectRemovePacket()
{
	TPacketGCAffectRemove kAffectRemove;
	if (!Recv(sizeof(kAffectRemove), &kAffectRemove))
		return false;

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_NEW_RemoveAffect", Py_BuildValue("(ii)", kAffectRemove.dwType, kAffectRemove.bApplyOn));

	return true;
}

bool CPythonNetworkStream::RecvChannelPacket()
{
	TPacketGCChannel kChannelPacket;
	if (!Recv(sizeof(kChannelPacket), &kChannelPacket))
		return false;
#ifdef ENABLE_CHANNEL_INFO_UPDATE
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_ReceiveChannel", Py_BuildValue("(i)", kChannelPacket.channel));
	SetChannel(kChannelPacket.channel);
#endif
	//Tracef(" >> CPythonNetworkStream::RecvChannelPacket(channel=%d)\n", kChannelPacket.channel);

	return true;
}

bool CPythonNetworkStream::RecvTargetCreatePacketNew()
{
	TPacketGCTargetCreateNew kTargetCreate;
	if (!Recv(sizeof(kTargetCreate), &kTargetCreate))
		return false;

	CPythonMiniMap& rkpyMiniMap = CPythonMiniMap::Instance();
	CPythonBackground& rkpyBG = CPythonBackground::Instance();
	if (CREATE_TARGET_TYPE_LOCATION == kTargetCreate.byType)
	{
		rkpyMiniMap.CreateTarget(kTargetCreate.lID, kTargetCreate.szTargetName);
	}
	else
	{
		rkpyMiniMap.CreateTarget(kTargetCreate.lID, kTargetCreate.szTargetName, kTargetCreate.dwVID);
		rkpyBG.CreateTargetEffect(kTargetCreate.lID, kTargetCreate.dwVID);
	}

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_OpenAtlasWindow", Py_BuildValue("()"));
	return true;
}

bool CPythonNetworkStream::RecvTargetUpdatePacket()
{
	TPacketGCTargetUpdate kTargetUpdate;
	if (!Recv(sizeof(kTargetUpdate), &kTargetUpdate))
		return false;

	CPythonMiniMap& rkpyMiniMap = CPythonMiniMap::Instance();
	rkpyMiniMap.UpdateTarget(kTargetUpdate.lID, kTargetUpdate.lX, kTargetUpdate.lY);

	CPythonBackground& rkpyBG = CPythonBackground::Instance();
	rkpyBG.CreateTargetEffect(kTargetUpdate.lID, kTargetUpdate.lX, kTargetUpdate.lY);

	return true;
}

bool CPythonNetworkStream::RecvTargetDeletePacket()
{
	TPacketGCTargetDelete kTargetDelete;
	if (!Recv(sizeof(kTargetDelete), &kTargetDelete))
		return false;

	CPythonMiniMap& rkpyMiniMap = CPythonMiniMap::Instance();
	rkpyMiniMap.DeleteTarget(kTargetDelete.lID);

	CPythonBackground& rkpyBG = CPythonBackground::Instance();
	rkpyBG.DeleteTargetEffect(kTargetDelete.lID);

	return true;
}

bool CPythonNetworkStream::RecvDigMotionPacket()
{
	TPacketGCDigMotion kDigMotion;
	if (!Recv(sizeof(kDigMotion), &kDigMotion))
		return false;

#ifdef _DEBUG
	Tracef(" Dig Motion [%d/%d]\n", kDigMotion.vid, kDigMotion.count);
#endif

	IAbstractCharacterManager& rkChrMgr = IAbstractCharacterManager::GetSingleton();
	CInstanceBase* pkInstMain = rkChrMgr.GetInstancePtr(kDigMotion.vid);
	CInstanceBase* pkInstTarget = rkChrMgr.GetInstancePtr(kDigMotion.target_vid);
	if (NULL == pkInstMain)
		return true;

	if (pkInstTarget)
		pkInstMain->NEW_LookAtDestInstance(*pkInstTarget);

	for (int i = 0; i < kDigMotion.count; ++i)
		pkInstMain->PushOnceMotion(CRaceMotionData::NAME_DIG);

	return true;
}

bool CPythonNetworkStream::SendDragonSoulRefinePacket(BYTE bRefineType, TItemPos* pos)
{
	TPacketCGDragonSoulRefine pk;
	pk.header = HEADER_CG_DRAGON_SOUL_REFINE;
	pk.bSubType = bRefineType;
	memcpy(pk.ItemGrid, pos, sizeof(TItemPos) * DS_REFINE_WINDOW_MAX_NUM);
	if (!Send(sizeof(pk), &pk))
	{
		return false;
	}
	return true;
}

#ifdef ENABLE_ACCE_SYSTEM
bool CPythonNetworkStream::RecvAccePacket(bool bReturn)
{
	TPacketAcce sPacket;
	if (!Recv(sizeof(sPacket), &sPacket))
		return bReturn;

	bReturn = true;
	switch (sPacket.subheader)
	{
	case ACCE_SUBHEADER_GC_OPEN:
		CPythonAcce::Instance().Clear();
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ActAcce", Py_BuildValue("(ib)", 1, sPacket.bWindow));
		break;

	case ACCE_SUBHEADER_GC_CLOSE:
		CPythonAcce::Instance().Clear();
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ActAcce", Py_BuildValue("(ib)", 2, sPacket.bWindow));
		break;

	case ACCE_SUBHEADER_GC_ADDED:
#ifdef ENABLE_GOLD_LIMIT_REWORK
		CPythonAcce::Instance().AddMaterial(sPacket.llPrice, sPacket.bPos, sPacket.tPos);
#else
		CPythonAcce::Instance().AddMaterial(sPacket.dwPrice, sPacket.bPos, sPacket.tPos);
#endif
		if (sPacket.bPos == 1)
		{
			CPythonAcce::Instance().AddResult(sPacket.dwItemVnum, sPacket.dwMinAbs, sPacket.dwMaxAbs);
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "AlertAcce", Py_BuildValue("(b)", sPacket.bWindow));
		}

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ActAcce", Py_BuildValue("(ib)", 3, sPacket.bWindow));
		break;

	case ACCE_SUBHEADER_GC_REMOVED:
#ifdef ENABLE_GOLD_LIMIT_REWORK
		CPythonAcce::Instance().RemoveMaterial(sPacket.llPrice, sPacket.bPos);
#else
		CPythonAcce::Instance().RemoveMaterial(sPacket.dwPrice, sPacket.bPos);
#endif
		if (sPacket.bPos == 0)
#ifdef ENABLE_GOLD_LIMIT_REWORK
			CPythonAcce::Instance().RemoveMaterial(sPacket.llPrice, 1);
#else
			CPythonAcce::Instance().RemoveMaterial(sPacket.dwPrice, 1);
#endif
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ActAcce", Py_BuildValue("(ib)", 4, sPacket.bWindow));
		break;

	case ACCE_SUBHEADER_CG_REFINED:
#ifdef ENABLE_GOLD_LIMIT_REWORK
		if (sPacket.dwMaxAbs == 0)
			CPythonAcce::Instance().RemoveMaterial(sPacket.llPrice, 1);
		else
		{
			CPythonAcce::Instance().RemoveMaterial(sPacket.llPrice, 0);
			CPythonAcce::Instance().RemoveMaterial(sPacket.llPrice, 1);
		}
#else
		if (sPacket.dwMaxAbs == 0)
			CPythonAcce::Instance().RemoveMaterial(sPacket.dwPrice, 1);
		else
		{
			CPythonAcce::Instance().RemoveMaterial(sPacket.dwPrice, 0);
			CPythonAcce::Instance().RemoveMaterial(sPacket.dwPrice, 1);
		}
#endif
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ActAcce", Py_BuildValue("(ib)", 4, sPacket.bWindow));
		break;

	default:
		TraceError("CPythonNetworkStream::RecvAccePacket: unknown subheader %d\n.", sPacket.subheader);
		break;
	}

	return bReturn;
}

bool CPythonNetworkStream::SendAcceClosePacket()
{
	if (!__CanActMainInstance())
		return true;

	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;

	TPacketAcce sPacket;
	sPacket.header = HEADER_CG_ACCE;
	sPacket.subheader = ACCE_SUBHEADER_CG_CLOSE;
#ifdef ENABLE_GOLD_LIMIT_REWORK
	sPacket.llPrice = 0;
#else
	sPacket.dwPrice = 0;
#endif
	sPacket.bPos = 0;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = 0;
	sPacket.dwMinAbs = 0;
	sPacket.dwMaxAbs = 0;

	if (!Send(sizeof(sPacket), &sPacket))
		return false;

	return true;
}

bool CPythonNetworkStream::SendAcceAddPacket(TItemPos tPos, BYTE bPos)
{
	if (!__CanActMainInstance())
		return true;

	TPacketAcce sPacket;
	sPacket.header = HEADER_CG_ACCE;
	sPacket.subheader = ACCE_SUBHEADER_CG_ADD;
#ifdef ENABLE_GOLD_LIMIT_REWORK
	sPacket.llPrice = 0;
#else
	sPacket.dwPrice = 0;
#endif
	sPacket.bPos = bPos;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = 0;
	sPacket.dwMinAbs = 0;
	sPacket.dwMaxAbs = 0;

	if (!Send(sizeof(sPacket), &sPacket))
		return false;

	return true;
}

bool CPythonNetworkStream::SendAcceRemovePacket(BYTE bPos)
{
	if (!__CanActMainInstance())
		return true;

	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;

	TPacketAcce sPacket;
	sPacket.header = HEADER_CG_ACCE;
	sPacket.subheader = ACCE_SUBHEADER_CG_REMOVE;
#ifdef ENABLE_GOLD_LIMIT_REWORK
	sPacket.llPrice = 0;
#else
	sPacket.dwPrice = 0;
#endif
	sPacket.bPos = bPos;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = 0;
	sPacket.dwMinAbs = 0;
	sPacket.dwMaxAbs = 0;

	if (!Send(sizeof(sPacket), &sPacket))
		return false;

	return true;
}

bool CPythonNetworkStream::SendAcceRefinePacket()
{
	if (!__CanActMainInstance())
		return true;

	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;

	TPacketAcce sPacket;
	sPacket.header = HEADER_CG_ACCE;
	sPacket.subheader = ACCE_SUBHEADER_CG_REFINE;
#ifdef ENABLE_GOLD_LIMIT_REWORK
	sPacket.llPrice = 0;
#else
	sPacket.dwPrice = 0;
#endif
	sPacket.bPos = 0;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = 0;
	sPacket.dwMinAbs = 0;
	sPacket.dwMaxAbs = 0;

	if (!Send(sizeof(sPacket), &sPacket))
		return false;

	return true;
}
#endif
#ifdef ENABLE_CHEST_INFO_SYSTEM
bool CPythonNetworkStream::SendChestDropInfo(DWORD dwChestVnum, TItemPos pos)
{
	TPacketCGChestDropInfo packet;
	packet.header = HEADER_CG_CHEST_DROP_INFO;
	packet.dwChestVnum = dwChestVnum;
	packet.pos = pos;

	if (!Send(sizeof(packet), &packet))
		return false;

	return true;
}

bool CPythonNetworkStream::RecvChestDropInfo()
{
	TPacketGCChestDropInfo packet;
	if (!Recv(sizeof(packet), &packet))
		return false;

	packet.wSize -= sizeof(packet);
	while (packet.wSize > 0)
	{
		TChestDropInfoTable kTab;
		if (!Recv(sizeof(kTab), &kTab))
			return false;

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_AddChestDropInfo", Py_BuildValue("(iiiiiii)", packet.dwChestVnum, packet.pos.window_type, packet.pos.cell, kTab.bPageIndex, kTab.bSlotIndex, kTab.dwItemVnum, kTab.bItemCount));

		packet.wSize -= sizeof(kTab);
	}

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_RefreshChestDropInfo", Py_BuildValue("(i)", packet.dwChestVnum));

	return true;
}
#endif

#ifdef ENABLE_EVENT_SYSTEM
bool CPythonNetworkStream::RecvEventInfo()
{
	TPacketGCEventInfo	infoPacket;
	if (!Recv(sizeof(infoPacket), &infoPacket))
		return false;
	CPythonGameEvents::instance().SetActivateEvent(infoPacket.isActivate, infoPacket.event_id);
	CPythonGameEvents::instance().SetEventTime(infoPacket.event_id, infoPacket.event_time);
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnRecvEventInformation", Py_BuildValue("()"));
	return true;
}
#endif

#ifdef ENABLE_SKILL_CHOOSE_SYSTEM
bool CPythonNetworkStream::RecvSkillChoose()
{
	TPacketGCSkillChoose packet;
	if (!Recv(sizeof(packet), &packet))
		return false;

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_SkillChoose", Py_BuildValue("(i)", packet.job));
	return true;
}

bool CPythonNetworkStream::SendSkillChooseInfo(int data)
{
	TPacketCGSkillChoose packet;
	packet.bHeader = HEADER_CG_SKILLCHOOSE;
	packet.job = data;

	if (!Send(sizeof(packet), &packet))
		return false;

	return true;
}
#endif

#ifdef ENABLE_CHANGELOOK_SYSTEM
bool CPythonNetworkStream::RecvChangeLookPacket()
{
	TPacketChangeLook sPacket;
	if (!Recv(sizeof(sPacket), &sPacket))
		return false;

	switch (sPacket.subheader)
	{
	case CL_SUBHEADER_OPEN:
	{
		CPythonChangeLook::Instance().Clear();
#ifdef ENABLE_GOLD_LIMIT_REWORK
		CPythonChangeLook::Instance().SetCost(sPacket.llCost);
#else
		CPythonChangeLook::Instance().SetCost(sPacket.dwCost);
#endif
#ifdef ENABLE_MOUNT_CHANGELOOK_SYSTEM
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ActChangeLook", Py_BuildValue("(ii)", 1, sPacket.bMount));
#else
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ActChangeLook", Py_BuildValue("(i)", 1));
#endif
	}
	break;
	case CL_SUBHEADER_CLOSE:
	{
		CPythonChangeLook::Instance().Clear();
#ifdef ENABLE_MOUNT_CHANGELOOK_SYSTEM
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ActChangeLook", Py_BuildValue("(ii)", 2, 0));
#else
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ActChangeLook", Py_BuildValue("(i)", 2));
#endif
	}
	break;
	case CL_SUBHEADER_ADD:
	{
		CPythonChangeLook::Instance().AddMaterial(sPacket.bPos, sPacket.tPos);
#ifdef ENABLE_MOUNT_CHANGELOOK_SYSTEM
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ActChangeLook", Py_BuildValue("(ii)", 3, 0));
#else
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ActChangeLook", Py_BuildValue("(i)", 3));
#endif
		if (sPacket.bPos == 1)
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "AlertChangeLook", Py_BuildValue("()"));
	}
	break;
	case CL_SUBHEADER_REMOVE:
	{
		if (sPacket.bPos == 1 || sPacket.bPos == 2)
			CPythonChangeLook::Instance().RemoveMaterial(sPacket.bPos);
		else
			CPythonChangeLook::Instance().RemoveAllMaterials();

#ifdef ENABLE_MOUNT_CHANGELOOK_SYSTEM
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ActChangeLook", Py_BuildValue("(ii)", 4, 0));
#else
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ActChangeLook", Py_BuildValue("(i)", 4));
#endif
	}
	break;
	case CL_SUBHEADER_REFINE:
	{
		CPythonChangeLook::Instance().RemoveAllMaterials();
#ifdef ENABLE_MOUNT_CHANGELOOK_SYSTEM
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ActChangeLook", Py_BuildValue("(ii)", 4, 0));
#else
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ActChangeLook", Py_BuildValue("(i)", 4));
#endif
	}
	break;
	default:
		TraceError("CPythonNetworkStream::RecvChangeLookPacket: unknown subheader %d\n.", sPacket.subheader);
		break;
	}

	return true;
}

bool CPythonNetworkStream::SendClClosePacket()
{
	if (!__CanActMainInstance())
		return true;

	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;

	TPacketChangeLook sPacket;
	sPacket.header = HEADER_CG_CL;
	sPacket.subheader = CL_SUBHEADER_CLOSE;
#ifdef ENABLE_GOLD_LIMIT_REWORK
	sPacket.llCost = 0;
#else
	sPacket.dwCost = 0;
#endif
	sPacket.bPos = 0;
	sPacket.tPos = tPos;
	if (!Send(sizeof(sPacket), &sPacket))
		return false;

	return true;
}

bool CPythonNetworkStream::SendClAddPacket(TItemPos tPos, BYTE bPos)
{
	if (!__CanActMainInstance())
		return true;

	TPacketChangeLook sPacket;
	sPacket.header = HEADER_CG_CL;
	sPacket.subheader = CL_SUBHEADER_ADD;
#ifdef ENABLE_GOLD_LIMIT_REWORK
	sPacket.llCost = 0;
#else
	sPacket.dwCost = 0;
#endif
	sPacket.bPos = bPos;
	sPacket.tPos = tPos;
	if (!Send(sizeof(sPacket), &sPacket))
		return false;

	return true;
}

bool CPythonNetworkStream::SendClRemovePacket(BYTE bPos)
{
	if (!__CanActMainInstance())
		return true;

	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;

	TPacketChangeLook sPacket;
	sPacket.header = HEADER_CG_CL;
	sPacket.subheader = CL_SUBHEADER_REMOVE;
#ifdef ENABLE_GOLD_LIMIT_REWORK
	sPacket.llCost = 0;
#else
	sPacket.dwCost = 0;
#endif
	sPacket.bPos = bPos;
	sPacket.tPos = tPos;
	if (!Send(sizeof(sPacket), &sPacket))
		return false;

	return true;
}

bool CPythonNetworkStream::SendClRefinePacket()
{
	if (!__CanActMainInstance())
		return true;

	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;

	TPacketChangeLook sPacket;
	sPacket.header = HEADER_CG_CL;
	sPacket.subheader = CL_SUBHEADER_REFINE;
#ifdef ENABLE_GOLD_LIMIT_REWORK
	sPacket.llCost = 0;
#else
	sPacket.dwCost = 0;
#endif
	sPacket.bPos = 0;
	sPacket.tPos = tPos;
	if (!Send(sizeof(sPacket), &sPacket))
		return false;

	return true;
}
#endif

#ifdef ENABLE_SKILL_COLOR_SYSTEM
bool CPythonNetworkStream::SendSkillColorPacket(BYTE skill, DWORD col1, DWORD col2, DWORD col3, DWORD col4, DWORD col5)
{
	TPacketCGSkillColor pack;
	pack.bheader = HEADER_CG_SKILL_COLOR;
	pack.skill = skill;
	pack.col1 = col1;
	pack.col2 = col2;
	pack.col3 = col3;
	pack.col4 = col4;
	pack.col5 = col5;

	if (!Send(sizeof(pack), &pack))
	{
		Tracen("Send Skill Color Packet Error");
		return false;
	}

	return true;
}
#endif

#ifdef ENABLE_DUNGEON_INFO_SYSTEM
bool CPythonNetworkStream::DungeonTeleport(BYTE dungeonIndex)
{
	if (!__CanActMainInstance())
		return true;

	TPacketGCTeleport p;
	p.header = HEADER_CG_TELEPORT;
	p.dungeonIndex = dungeonIndex;
	if (!Send(sizeof(TPacketGCTeleport), &p))
	{
		Tracen("DungeonTeleport Error");
		return false;
	}

	return true;
}
#endif

#ifdef ENABLE_FAST_CHEQUE_TRANSFER
bool CPythonNetworkStream::SendWonExchangeSellPacket(DWORD wValue)
{
	TPacketCGWonExchange kWonExchangePacket;
	kWonExchangePacket.bSubHeader = WON_EXCHANGE_CG_SUBHEADER_SELL;
	kWonExchangePacket.wValue = wValue;

	if (!Send(sizeof(TPacketCGWonExchange), &kWonExchangePacket))
		return false;

	return true;
}

bool CPythonNetworkStream::SendWonExchangeBuyPacket(DWORD wValue)
{
	TPacketCGWonExchange kWonExchangePacket;
	kWonExchangePacket.bSubHeader = WON_EXCHANGE_CG_SUBHEADER_BUY;
	kWonExchangePacket.wValue = wValue;

	if (!Send(sizeof(TPacketCGWonExchange), &kWonExchangePacket))
		return false;

	return true;
}
#endif

#ifdef ENABLE_SWITCHBOT_SYSTEM
bool CPythonNetworkStream::RecvSwitchbotPacket()
{
	TPacketGCSwitchbot pack;
	if (!Recv(sizeof(pack), &pack))
	{
		return false;
	}

	size_t packet_size = int(pack.size) - sizeof(TPacketGCSwitchbot);
	if (pack.subheader == SUBHEADER_GC_SWITCHBOT_UPDATE)
	{
		if (packet_size != sizeof(CPythonSwitchbot::TSwitchbotTable))
		{
			return false;
		}

		CPythonSwitchbot::TSwitchbotTable table;
		if (!Recv(sizeof(table), &table))
		{
			return false;
		}

		CPythonSwitchbot::Instance().Update(table);
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshSwitchbotWindow", Py_BuildValue("()"));
	}
	else if (pack.subheader == SUBHEADER_GC_SWITCHBOT_UPDATE_ITEM)
	{
		if (packet_size != sizeof(TSwitchbotUpdateItem))
		{
			return false;
		}

		TSwitchbotUpdateItem update;
		if (!Recv(sizeof(update), &update))
		{
			return false;
		}

		TItemPos pos(SWITCHBOT, update.slot);

		IAbstractPlayer& rkPlayer = IAbstractPlayer::GetSingleton();
		rkPlayer.SetItemCount(pos, update.count);

		for (int i = 0; i < ITEM_SOCKET_SLOT_MAX_NUM; ++i)
		{
			rkPlayer.SetItemMetinSocket(pos, i, update.alSockets[i]);
		}

		for (int j = 0; j < ITEM_ATTRIBUTE_SLOT_MAX_NUM; ++j)
		{
			rkPlayer.SetItemAttribute(pos, j, update.aAttr[j].bType, update.aAttr[j].sValue);
		}

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshSwitchbotItem", Py_BuildValue("(i)", update.slot));
		return true;
	}
	else if (pack.subheader == SUBHEADER_GC_SWITCHBOT_SEND_ATTRIBUTE_INFORMATION)
	{
		CPythonSwitchbot::Instance().ClearAttributeMap();

		size_t table_size = sizeof(CPythonSwitchbot::TSwitchbottAttributeTable);
		while (packet_size >= table_size)
		{
			CPythonSwitchbot::TSwitchbottAttributeTable table;
			if (!Recv(table_size, &table))
			{
				return false;
			}

			CPythonSwitchbot::Instance().AddAttributeToMap(table);
			packet_size -= table_size;
		}
	}

	return true;
}
bool CPythonNetworkStream::SendSwitchbotStartPacket(BYTE slot, std::vector<CPythonSwitchbot::TSwitchbotAttributeAlternativeTable> alternatives)
{
	TPacketCGSwitchbot pack;
	pack.header = HEADER_CG_SWITCHBOT;
	pack.subheader = SUBHEADER_CG_SWITCHBOT_START;
	pack.size = sizeof(TPacketCGSwitchbot) + sizeof(CPythonSwitchbot::TSwitchbotAttributeAlternativeTable) * SWITCHBOT_ALTERNATIVE_COUNT;
	pack.slot = slot;

	if (!Send(sizeof(pack), &pack))
	{
		return false;
	}

	for (const auto& it : alternatives)
	{
		if (!Send(sizeof(it), &it))
		{
			return false;
		}
	}

	return true;
}

bool CPythonNetworkStream::SendSwitchbotStopPacket(BYTE slot)
{
	TPacketCGSwitchbot pack;
	pack.header = HEADER_CG_SWITCHBOT;
	pack.subheader = SUBHEADER_CG_SWITCHBOT_STOP;
	pack.size = sizeof(TPacketCGSwitchbot);
	pack.slot = slot;

	if (!Send(sizeof(pack), &pack))
	{
		return false;
	}

	return true;
}

bool CPythonNetworkStream::SendSwitchbotChangeSpeed(BYTE speed)
{
	TPacketCGSwitchbot pack;
	pack.header = HEADER_CG_SWITCHBOT;
	pack.subheader = SUBHEADER_CG_SWITCHBOT_CHANGE_SPEED;
	pack.size = sizeof(TPacketCGSwitchbot);
	pack.slot = speed;

	if (!Send(sizeof(pack), &pack))
	{
		return false;
	}

	return true;
}
#endif

#ifdef ENABLE_CUBE_RENEWAL
bool CPythonNetworkStream::CubeRenewalMakeItem(int index_item, int count_item, int index_item_improve)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGCubeRenewalSend	packet;

	packet.header = HEADER_CG_CUBE_RENEWAL;
	packet.subheader = CUBE_RENEWAL_SUB_HEADER_MAKE_ITEM;
	packet.index_item = index_item;
	packet.count_item = count_item;
	packet.index_item_improve = index_item_improve;

	if (!Send(sizeof(TPacketCGCubeRenewalSend), &packet))
	{
		Tracef("CPythonNetworkStream::CubeRenewalMakeItem Error\n");
		return false;
	}

	return true;
}
bool CPythonNetworkStream::CubeRenewalClose()
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGCubeRenewalSend	packet;

	packet.header = HEADER_CG_CUBE_RENEWAL;
	packet.subheader = CUBE_RENEWAL_SUB_HEADER_CLOSE;

	if (!Send(sizeof(TPacketCGCubeRenewalSend), &packet))
	{
		Tracef("CPythonNetworkStream::CubeRenewalClose Error\n");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::RecvCubeRenewalPacket()
{
	TPacketGCCubeRenewalReceive CubeRenewalPacket;

	if (!Recv(sizeof(CubeRenewalPacket), &CubeRenewalPacket))
		return false;

	switch (CubeRenewalPacket.subheader)
	{
	case CUBE_RENEWAL_SUB_HEADER_OPEN_RECEIVE:
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_CUBE_RENEWAL_OPEN", Py_BuildValue("()"));
	}
	break;

	case CUBE_RENEWAL_SUB_HEADER_DATES_RECEIVE:
	{
		CPythonCubeRenewal::Instance().ReceiveList(CubeRenewalPacket.date_cube_renewal);
	}
	break;

	case CUBE_RENEWAL_SUB_HEADER_DATES_LOADING:
	{
		CPythonCubeRenewal::Instance().LoadingList();
	}
	break;

	case CUBE_RENEWAL_SUB_HEADER_CLEAR_DATES_RECEIVE:
	{
		CPythonCubeRenewal::Instance().ClearList();
	}
	break;
	}

	return true;
}
#endif

#ifdef ENABLE_TARGET_BOARD_RENEWAL
bool CPythonNetworkStream::RecvMobInfo()
{
	TPacketGCMobInformation pack;
	if (!Recv(sizeof(pack), &pack))
		return false;

	size_t packet_size = int(pack.size) - sizeof(TPacketGCMobInformation);

	switch (pack.subheader)
	{
	case MOB_INFORMATION_DROP:
	{
		if (packet_size != sizeof(TMobInformationDrop))
		{
			TraceError("MOB_INFORMATION_DROP: invalid packet size %d", packet_size);
			return false;
		}

		TMobInformationDrop mobInformation;
		if (!Recv(sizeof(mobInformation), &mobInformation))
		{
			TraceError("mobInformation table recv error.");
			return false;
		}

		for (int i = 0; i < MOB_INFO_ITEM_LIST_MAX; ++i)
		{
			DWORD itemVnum = mobInformation.item_list[i].vnum;
			DWORD itemCount = mobInformation.item_list[i].count;
			if (itemVnum <= 0)
				continue;
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_MobInformationDropItem", Py_BuildValue("(ii)", itemVnum, itemCount));
		}

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_RecvMobInformation", Py_BuildValue("(i)", MOB_INFORMATION_DROP));
		return true;
	}
	break;

#ifdef ENABLE_DUNGEON_INFO_SYSTEM
	case MOB_INFORMATION_DUNGEON_DROP:
	{
		if (packet_size != sizeof(TMobInformationDrop))
		{
			TraceError("MOB_INFORMATION_DUNGEON_DROP: invalid packet size %d", packet_size);
			return false;
		}

		TMobInformationDrop mobInformation;
		if (!Recv(sizeof(mobInformation), &mobInformation))
		{
			TraceError("mobInformation table recv error.");
			return false;
		}

		for (int i = 0; i < MOB_INFO_ITEM_LIST_MAX; ++i)
		{
			DWORD itemVnum = mobInformation.item_list[i].vnum;
			DWORD itemCount = mobInformation.item_list[i].count;
			if (itemVnum <= 0)
				continue;
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_MobInformationDungeonDropItem", Py_BuildValue("(ii)", itemVnum, itemCount));
		}

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_RecvDungeonMobInformation", Py_BuildValue("()"));
		return true;
	}
	break;
#endif

	case MOB_INFORMATION_BONUS:
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_RecvMobInformation", Py_BuildValue("(i)", MOB_INFORMATION_BONUS));
		return true;
	}
	break;

	case MOB_INFORMATION_SPECIALITY:
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_RecvMobInformation", Py_BuildValue("(i)", MOB_INFORMATION_SPECIALITY));
		return true;
	}
	break;

	default:
		TraceError("RecvRanking invalid subheader %d", pack.subheader);
		break;
	}

	return false;
}

void CPythonNetworkStream::SendMobInfoPacket(DWORD dwMobVnum, DWORD dwVirtualID, BYTE bType)
{
	if (!__CanActMainInstance())
		return;

	TPacketCGMobInformation	pack;
	pack.header = HEADER_CG_MOB_INFO;
	pack.dwMobVnum = dwMobVnum;
	pack.dwVirtualID = dwVirtualID;
	pack.bType = bType;

	if (!Send(sizeof(TPacketCGMobInformation), &pack))
	{
		Tracef("CPythonNetworkStream::SendMobInfoPacket Error\n");
		return;
	}
}
#endif

#ifdef ENABLE_PLAYER_STATISTICS
bool CPythonNetworkStream::RecvPlayerStatistics()
{
	TPacketGCPlayerStatistics playerstatistics;

	if (!Recv(sizeof(TPacketGCPlayerStatistics), &playerstatistics))
	{
		Tracen("Recv playerstatistics Packet Error");
		return false;
	}

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ReceivePlayerStatisticsPacket", Py_BuildValue("(iiiiiiiiiiiLL)",
		playerstatistics.iKilledShinsoo,
		playerstatistics.iKilledChunjo,
		playerstatistics.iKilledJinno,
		playerstatistics.iTotalKill,
		playerstatistics.iDuelWon,
		playerstatistics.iDuelLost,
		playerstatistics.iKilledMonster,
		playerstatistics.iKilledStone,
		playerstatistics.iKilledBoss,
		playerstatistics.iCompletedDungeon,
		playerstatistics.iTakedFish,
		playerstatistics.iBestStoneDamage,
		playerstatistics.iBestBossDamage
	));
	return true;
}
#endif
#ifdef ENABLE_INGAME_MALL_SYSTEM
bool CPythonNetworkStream::RecvItemShopData()
{
	TPacketItemShopData p;
	if (!Recv(sizeof(TPacketItemShopData), &p))
	{
		Tracenf("Recv TPacketItemShopData Packet Error");
		return false;
	}

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_ItemShop_ItemData", Py_BuildValue("(iiiiiii)",
		p.id,
		p.category,
		p.sub_category,
		p.vnum,
		p.count,
		p.coins,
		p.socketzero
	));

	return true;
}
#endif // ENABLE_INGAME_MALL_SYSTEM


#ifdef ENABLE_BATTLE_PASS_SYSTEM
bool CPythonNetworkStream::SendBattlePassAction(BYTE bAction, BYTE bSubAction)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGBattlePassAction packet;
	packet.bHeader = HEADER_CG_BATTLE_PASS;
	packet.bAction = bAction;
	packet.bSubAction = bSubAction;

	if (!Send(sizeof(TPacketCGBattlePassAction), &packet))
	{
		Tracef("SendBattlePassAction Send Packet Error\n");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::RecvBattlePassPacket()
{
	TPacketGCBattlePass packet;
	if (!Recv(sizeof(packet), &packet))
		return false;

	// PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_BattlePassClearMission", Py_BuildValue("()"));

	packet.wSize -= sizeof(packet);
	while (packet.wSize > 0)
	{
		TBattlePassMissionInfo missionInfo;
		if (!Recv(sizeof(missionInfo), &missionInfo))
			return false;

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_BattlePassAddMission", Py_BuildValue("(iiiii)",
			missionInfo.bMissionType, missionInfo.dwMissionInfo[0], missionInfo.dwMissionInfo[1], missionInfo.dwMissionInfo[2], missionInfo.dwSkipCost));

		for (int i = 0; i < 3; ++i)
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_BattlePassAddMissionReward", Py_BuildValue("(iii)",
				missionInfo.bMissionType, missionInfo.aRewardList[i].dwVnum, missionInfo.aRewardList[i].bCount));

		packet.wSize -= sizeof(missionInfo);
	}

	while (packet.wRewardSize > 0)
	{
		TBattlePassRewardItem rewardInfo;
		if (!Recv(sizeof(rewardInfo), &rewardInfo))
			return false;

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_BattlePassAddReward", Py_BuildValue("(ii)", rewardInfo.dwVnum, rewardInfo.bCount));

		packet.wRewardSize -= sizeof(rewardInfo);
	}

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_BattlePassOpen", Py_BuildValue("()"));

	return true;
}

bool CPythonNetworkStream::RecvBattlePassRankingPacket()
{
	TPacketGCBattlePassRanking packet;
	if (!Recv(sizeof(packet), &packet))
		return false;

	packet.wSize -= sizeof(packet);
	while (packet.wSize > 0)
	{
		TBattlePassRanking rankingInfo;
		if (!Recv(sizeof(rankingInfo), &rankingInfo))
			return false;

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_BattlePassAddRanking", Py_BuildValue("(isi)",
			rankingInfo.bPos, rankingInfo.playerName, rankingInfo.dwFinishTime));

		packet.wSize -= sizeof(rankingInfo);
	}

	if (packet.bIsGlobal)
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_BattlePassOpenRanking", Py_BuildValue("()"));
	else
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_BattlePassRefreshRanking", Py_BuildValue("()"));

	return true;
}

bool CPythonNetworkStream::RecvBattlePassUpdatePacket()
{
	TPacketGCBattlePassUpdate packet;
	if (!Recv(sizeof(packet), &packet))
		return false;

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_BattlePassUpdate", Py_BuildValue("(ii)", packet.bMissionType, packet.dwNewProgress));

	return true;
}
#endif
