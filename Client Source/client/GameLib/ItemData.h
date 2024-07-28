#pragma once

//        Icon, Model (droped on ground), Game Data

#include "../eterLib/GrpSubImage.h"
#include "../eterGrnLib/Thing.h"
#include "GameLibDefines.h"

class CItemData
{
public:
	enum EItemOption
	{
		ITEM_NAME_MAX_LEN = 64,
		ITEM_LIMIT_MAX_NUM = 2,
		ITEM_VALUES_MAX_NUM = 6,
		ITEM_SMALL_DESCR_MAX_LEN = 256,
		ITEM_APPLY_MAX_NUM = 3,
		ITEM_SOCKET_MAX_NUM = 3,
#ifdef ENABLE_SHINING_EFFECT_UTILITY
		ITEM_SHINING_MAX_COUNT = 3,
#endif
	};

	enum EItemType
	{
		ITEM_TYPE_NONE,					//0
		ITEM_TYPE_WEAPON,
		ITEM_TYPE_ARMOR,
		ITEM_TYPE_USE,
		ITEM_TYPE_AUTOUSE,				//4
		ITEM_TYPE_MATERIAL,				//5
		ITEM_TYPE_SPECIAL,
		ITEM_TYPE_TOOL,					//7
		ITEM_TYPE_LOTTERY,
		ITEM_TYPE_ELK,
		ITEM_TYPE_METIN,				//10
		ITEM_TYPE_CONTAINER,			//11
		ITEM_TYPE_FISH,
		ITEM_TYPE_ROD,					//13
		ITEM_TYPE_RESOURCE,				//14
		ITEM_TYPE_CAMPFIRE,				//15
		ITEM_TYPE_UNIQUE,				//16
		ITEM_TYPE_SKILLBOOK,			//17
		ITEM_TYPE_QUEST,				//18
		ITEM_TYPE_POLYMORPH,			//19
		ITEM_TYPE_TREASURE_BOX,
		ITEM_TYPE_TREASURE_KEY,
		ITEM_TYPE_SKILLFORGET,			//22
		ITEM_TYPE_GIFTBOX,				//23
		ITEM_TYPE_PICK,					//24
		ITEM_TYPE_HAIR,
		ITEM_TYPE_TOTEM,
		ITEM_TYPE_BLEND,
		ITEM_TYPE_COSTUME,
		ITEM_TYPE_DS,
		ITEM_TYPE_SPECIAL_DS,
		ITEM_TYPE_EXTRACT,
		ITEM_TYPE_SECONDARY_COIN,
		ITEM_TYPE_RING,
		ITEM_TYPE_BELT,

		ITEM_TYPE_PET,						//35
		ITEM_TYPE_MEDIUM,					//36
		ITEM_TYPE_GACHA,
		ITEM_TYPE_SOUL,
#ifdef ENABLE_SHINING_ITEM_SYSTEM
		ITEM_TYPE_SHINING,
#endif
#ifdef ENABLE_CAKRA_ITEM_SYSTEM
		ITEM_TYPE_CAKRA,
#endif
#ifdef ENABLE_SEBNEM_ITEM_SYSTEM
		ITEM_TYPE_SEBNEM,
#endif
		ITEM_TYPE_MAX_NUM,
	};

	enum EWeaponSubTypes
	{
		WEAPON_SWORD, //0
		WEAPON_DAGGER,
		WEAPON_BOW, //2
		WEAPON_TWO_HANDED, //3
		WEAPON_BELL, //4
		WEAPON_FAN, //5
		WEAPON_ARROW, //6
		WEAPON_MOUNT_SPEAR, //7
#ifdef ENABLE_WOLFMAN_CHARACTER
		WEAPON_CLAW = 8, //8
#endif
#ifdef ENABLE_QUIVER_SYSTEM
		WEAPON_QUIVER = 9, //9
#endif
		WEAPON_NUM_TYPES, //11 2015/11/12

		WEAPON_NONE = WEAPON_NUM_TYPES + 1,
	};

	enum EMaterialSubTypes
	{
		MATERIAL_LEATHER,
		MATERIAL_BLOOD,
		MATERIAL_ROOT,
		MATERIAL_NEEDLE,
		MATERIAL_JEWEL,
		MATERIAL_DS_REFINE_NORMAL,
		MATERIAL_DS_REFINE_BLESSED,
		MATERIAL_DS_REFINE_HOLLY,
#ifdef ENABLE_DSS_REFINE_ITEM
		MATERIAL_DS_REFINE_1,
		MATERIAL_DS_REFINE_2,
		MATERIAL_DS_REFINE_3,
		MATERIAL_DS_REFINE_4,
		MATERIAL_DS_REFINE_5,
#endif
	};

	enum EArmorSubTypes
	{
		ARMOR_BODY,
		ARMOR_HEAD,
		ARMOR_SHIELD,
		ARMOR_WRIST,
		ARMOR_FOOTS,
		ARMOR_NECK,
		ARMOR_EAR,
		ARMOR_PENDANT,
		ARMOR_NUM_TYPES
	};

	enum ECostumeSubTypes
	{
		COSTUME_BODY,
		COSTUME_HAIR,
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
		COSTUME_MOUNT = 2,		//2
#endif
#ifdef ENABLE_ACCE_SYSTEM
		COSTUME_ACCE = 3,		//3
#endif
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
		COSTUME_WEAPON = 4,		//4
#endif
#ifdef ENABLE_AURA_COSTUME_SYSTEM
		COSTUME_AURA = 5,
#endif
		COSTUME_NUM_TYPES,
	};

	enum EUseSubTypes
	{
		USE_POTION,					// 0
		USE_TALISMAN,
		USE_TUNING,
		USE_MOVE,
		USE_TREASURE_BOX,
		USE_MONEYBAG,
		USE_BAIT,
		USE_ABILITY_UP,
		USE_AFFECT,
		USE_CREATE_STONE,
		USE_SPECIAL,				// 10
		USE_POTION_NODELAY,
		USE_CLEAR,
		USE_INVISIBILITY,
		USE_DETACHMENT,
		USE_BUCKET,
		USE_POTION_CONTINUE,
		USE_CLEAN_SOCKET,
		USE_CHANGE_ATTRIBUTE,
		USE_ADD_ATTRIBUTE,
		USE_ADD_ACCESSORY_SOCKET,	// 20
		USE_PUT_INTO_ACCESSORY_SOCKET,
		USE_ADD_ATTRIBUTE2,
		USE_RECIPE,
		USE_CHANGE_ATTRIBUTE2,
		USE_TIME_CHARGE_PER,
		USE_TIME_CHARGE_FIX,				// 28
		USE_PUT_INTO_BELT_SOCKET,
		USE_PUT_INTO_RING_SOCKET,
		USE_CHANGE_COSTUME_ATTR,			// 31
		USE_RESET_COSTUME_ATTR,				// 32
		USE_SELECT_ATTRIBUTE,
		USE_FLOWER,
		USE_PET,
#ifdef ENABLE_PENDANT_ATTRIBUTE_SYSTEM
		USE_ADD_PENDANT_ATTRIBUTE,
		USE_ADD_PENDANT_FIVE_ATTRIBUTE,
		USE_CHANGE_PENDANT_ATTRIBUTE,
#endif
#ifdef ENABLE_AURA_COSTUME_SYSTEM
		USE_ADD_AURA_ATTRIBUTE,
		USE_ADD_AURA_FIVE_ATTRIBUTE,
		USE_CHANGE_AURA_ATTRIBUTE,
#endif
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
		USE_ADD_MOUNT_ATTRIBUTE,
		USE_CHANGE_MOUNT_ATTRIBUTE,
#endif
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
		USE_ADD_WEAPON_COSTUME_ATTRIBUTE,
		USE_CHANGE_WEAPON_COSTUME_ATTRIBUTE,
#endif
#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
		USE_ADD_BELT_ATTRIBUTE,
		USE_CHANGE_BELT_ATTRIBUTE,
#endif
#ifdef ENABLE_HAIR_COSTUME_ATTRIBUTE
		USE_ADD_HAIR_COSTUME_ATTRIBUTE,
		USE_CHANGE_HAIR_COSTUME_ATTRIBUTE,
#endif
#ifdef ENABLE_BODY_COSTUME_ATTRIBUTE
		USE_ADD_BODY_COSTUME_ATTRIBUTE,
		USE_CHANGE_BODY_COSTUME_ATTRIBUTE,
#endif
#ifdef ENABLE_SHINING_ITEM_SYSTEM
		USE_ADD_ACCE_SHINING_ATTRIBUTE,
		USE_CHANGE_ACCE_SHINING_ATTRIBUTE,
		USE_ADD_SOUL_SHINING_ATTRIBUTE,
		USE_ADD_SOUL_SHINING_FIVE_ATTRIBUTE,
		USE_CHANGE_SOUL_SHINING_ATTRIBUTE,
#endif
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
		USE_ADD_FIVE_MOUNT_ATTRIBUTE,
#endif
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
		USE_ADD_FIVE_WEAPON_COSTUME_ATTRIBUTE,
#endif
#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
		USE_ADD_FIVE_BELT_ATTRIBUTE,
#endif
#ifdef ENABLE_HAIR_COSTUME_ATTRIBUTE
		USE_ADD_FIVE_HAIR_COSTUME_ATTRIBUTE,
#endif
#ifdef ENABLE_BODY_COSTUME_ATTRIBUTE
		USE_ADD_FIVE_BODY_COSTUME_ATTRIBUTE,
#endif
#ifdef ENABLE_SHINING_ITEM_SYSTEM
		USE_ADD_FIVE_ACCE_SHINING_ATTRIBUTE,
#endif
#ifdef ENABLE_CAKRA_ITEM_SYSTEM
		USE_ADD_CAKRA_ITEM_ATTRIBUTE,
		USE_ADD_FIVE_CAKRA_ITEM_ATTRIBUTE,
		USE_CHANGE_CAKRA_ITEM_ATTRIBUTE,
#endif
#ifdef ENABLE_EXTENDED_PET_ITEM
		USE_ADD_PET_ITEM_ATTRIBUTE,
		USE_ADD_FIVE_PET_ITEM_ATTRIBUTE,
		USE_CHANGE_PET_ITEM_ATTRIBUTE,
#endif
#ifdef ENABLE_SEBNEM_ITEM_SYSTEM
		USE_ADD_SEBNEM_ITEM_ATTRIBUTE,
		USE_ADD_FIVE_SEBNEM_ITEM_ATTRIBUTE,
		USE_CHANGE_SEBNEM_ITEM_ATTRIBUTE,
#endif
		USE_UNKNOWN_TYPE,
	};

	enum EDragonSoulSubType
	{
		DS_SLOT1,
		DS_SLOT2,
		DS_SLOT3,
		DS_SLOT4,
		DS_SLOT5,
		DS_SLOT6,
		DS_SLOT_NUM_TYPES = 6,
	};

#ifdef ENABLE_BLEND_REWORK
	enum EBlendSubType
	{
		NORMAL_BLEND,
		INFINITY_BLEND,
	};
#endif

#ifdef ENABLE_DSS_RECHARGE_ITEM
	enum EDSSDuration
	{
		MIN_INFINITE_DURATION = 100 * 24 * 60 * 60,
		MAX_INFINITE_DURATION = 60 * 365 * 24 * 60 * 60,
	};
#endif

	enum EMetinSubTypes
	{
		METIN_NORMAL,
		METIN_GOLD,
	};

	enum ELimitTypes
	{
		LIMIT_NONE,

		LIMIT_LEVEL,
		LIMIT_STR,
		LIMIT_DEX,
		LIMIT_INT,
		LIMIT_CON,
		LIMIT_UNUSED,

		LIMIT_REAL_TIME,

		LIMIT_REAL_TIME_START_FIRST_USE,

		LIMIT_TIMER_BASED_ON_WEAR,

		LIMIT_MAX_NUM
	};

	enum EItemAntiFlag
	{
		ITEM_ANTIFLAG_FEMALE = (1 << 0),
		ITEM_ANTIFLAG_MALE = (1 << 1),
		ITEM_ANTIFLAG_WARRIOR = (1 << 2),
		ITEM_ANTIFLAG_ASSASSIN = (1 << 3),
		ITEM_ANTIFLAG_SURA = (1 << 4),
		ITEM_ANTIFLAG_SHAMAN = (1 << 5),
		ITEM_ANTIFLAG_GET = (1 << 6),
		ITEM_ANTIFLAG_DROP = (1 << 7),
		ITEM_ANTIFLAG_SELL = (1 << 8),
		ITEM_ANTIFLAG_EMPIRE_A = (1 << 9),
		ITEM_ANTIFLAG_EMPIRE_B = (1 << 10),
		ITEM_ANTIFLAG_EMPIRE_R = (1 << 11),
		ITEM_ANTIFLAG_SAVE = (1 << 12),
		ITEM_ANTIFLAG_GIVE = (1 << 13),
		ITEM_ANTIFLAG_PKDROP = (1 << 14),
		ITEM_ANTIFLAG_STACK = (1 << 15),
		ITEM_ANTIFLAG_MYSHOP = (1 << 16),
		ITEM_ANTIFLAG_SAFEBOX = (1 << 17),
		ITEM_ANTIFLAG_WOLFMAN = (1 << 18),
#ifdef ENABLE_SLOT_MARKING_SYSTEM
		ITEM_ANTIFLAG_QUICKSLOT = (1 << 19),
		ITEM_ANTIFLAG_PET = (1 << 20),
		ITEM_ANTIFLAG_CHANGELOOK = (1 << 21),
		ITEM_ANTIFLAG_REINFORCE = (1 << 22),
		ITEM_ANTIFLAG_ENCHANT = (1 << 23),
		ITEM_ANTIFLAG_ENERGY = (1 << 24),
		ITEM_ANTIFLAG_PETFEED = (1 << 25),
		ITEM_ANTIFLAG_APPLY = (1 << 26),
		ITEM_ANTIFLAG_ACCE = (1 << 27),
		ITEM_ANTIFLAG_MAIL = (1 << 28),
		ITEM_ANTIFLAG_AURA = (1 << 29),
		ITEM_ANTIFLAG_FORCE_ALL = (1 << 30),
		ITEM_ANTIFLAG_COMB = (1 << 31),
#endif
	};

	enum EItemFlag
	{
		ITEM_FLAG_REFINEABLE = (1 << 0),
		ITEM_FLAG_SAVE = (1 << 1),
		ITEM_FLAG_STACKABLE = (1 << 2),
		ITEM_FLAG_COUNT_PER_1GOLD = (1 << 3),
		ITEM_FLAG_SLOW_QUERY = (1 << 4),
		ITEM_FLAG_RARE = (1 << 5),
		ITEM_FLAG_UNIQUE = (1 << 6),
		ITEM_FLAG_MAKECOUNT = (1 << 7),
		ITEM_FLAG_IRREMOVABLE = (1 << 8),
		ITEM_FLAG_CONFIRM_WHEN_USE = (1 << 9),
		ITEM_FLAG_QUEST_USE = (1 << 10),
		ITEM_FLAG_QUEST_USE_MULTIPLE = (1 << 11),
		ITEM_FLAG_QUEST_GIVE = (1 << 12),
		ITEM_FLAG_LOG = (1 << 13),
		ITEM_FLAG_APPLICABLE = (1 << 14),
		ITEM_FLAG_RARE_ABILITY = (1 << 15),
		ITEM_FLAG_LOG_SPECIAL = (1 << 16),
	};

	enum EWearPositions
	{
		WEAR_BODY,          // 0
		WEAR_HEAD,          // 1
		WEAR_FOOTS,         // 2
		WEAR_WRIST,         // 3
		WEAR_WEAPON,        // 4
		WEAR_NECK,          // 5
		WEAR_EAR,           // 6
		WEAR_UNIQUE1,       // 7
		WEAR_UNIQUE2,       // 8
		WEAR_ARROW,         // 9
		WEAR_SHIELD,        // 10

		WEAR_ABILITY1,  // 11
		WEAR_ABILITY2,  // 12
		WEAR_ABILITY3,  // 13
		WEAR_ABILITY4,  // 14
		WEAR_ABILITY5,  // 15
		WEAR_ABILITY6,  // 16
		WEAR_ABILITY7,  // 17
		WEAR_ABILITY8,  // 18
		WEAR_COSTUME_BODY,	// 19
		WEAR_COSTUME_HAIR,	// 20
		WEAR_COSTUME_MOUNT,// 21
		WEAR_COSTUME_ACCE,// 22
		WEAR_COSTUME_WEAPON,// 23
#ifdef ENABLE_AURA_COSTUME_SYSTEM
		WEAR_COSTUME_AURA,// 24
#endif
		WEAR_RING1,			// 25
		WEAR_RING2,			// 26
		WEAR_BELT,			// 27
		WEAR_PENDANT,// 28
#ifdef ENABLE_SHINING_ITEM_SYSTEM
		WEAR_SHINING_WEAPON,
		WEAR_SHINING_ARMOR,
		WEAR_SHINING_SPECIAL,
		WEAR_SHINING_SPECIAL2,
		WEAR_SHINING_SPECIAL3,
		WEAR_SHINING_WING,
#endif
#ifdef ENABLE_CAKRA_ITEM_SYSTEM
		WEAR_CAKRA_1,
		WEAR_CAKRA_2,
		WEAR_CAKRA_3,
		WEAR_CAKRA_4,
		WEAR_CAKRA_5,
		WEAR_CAKRA_6,
		WEAR_CAKRA_7,
		WEAR_CAKRA_8,
#endif
#ifdef ENABLE_EXTENDED_PET_ITEM
		WEAR_PET,
#endif
#ifdef ENABLE_SEBNEM_ITEM_SYSTEM
		WEAR_SEBNEM_1,
		WEAR_SEBNEM_2,
		WEAR_SEBNEM_3,
		WEAR_SEBNEM_4,
		WEAR_SEBNEM_5,
		WEAR_SEBNEM_6,
		WEAR_SEBNEM_7,
		WEAR_SEBNEM_8,
#endif
		WEAR_MAX_NUM = 55,
	};

	enum EItemWearableFlag
	{
		WEARABLE_BODY = (1 << 0),
		WEARABLE_HEAD = (1 << 1),
		WEARABLE_FOOTS = (1 << 2),
		WEARABLE_WRIST = (1 << 3),
		WEARABLE_WEAPON = (1 << 4),
		WEARABLE_NECK = (1 << 5),
		WEARABLE_EAR = (1 << 6),
		WEARABLE_UNIQUE = (1 << 7),
		WEARABLE_SHIELD = (1 << 8),
		WEARABLE_ARROW = (1 << 9),
		WEARABLE_ABILITY = (1 << 10),
		WEARABLE_COSTUME_BODY = (1 << 11),
		WEARABLE_COSTUME_HAIR = (1 << 12),
		WEARABLE_COSTUME_MOUNT = (1 << 13),
		WEARABLE_COSTUME_ACCE = (1 << 14),
		WEARABLE_COSTUME_WEAPON = (1 << 15),
		WEARABLE_PENDANT = (1 << 16),
#ifdef ENABLE_AURA_COSTUME_SYSTEM
		WEARABLE_COSTUME_AURA = (1 << 17),
#endif
#ifdef ENABLE_EXTENDED_PET_ITEM
		WEARABLE_PET = (1 << 18),
#endif
	};

	enum EApplyTypes
	{
		APPLY_NONE,                 // 0
		APPLY_MAX_HP,               // 1
		APPLY_MAX_SP,               // 2
		APPLY_CON,                  // 3
		APPLY_INT,                  // 4
		APPLY_STR,                  // 5
		APPLY_DEX,                  // 6
		APPLY_ATT_SPEED,            // 7
		APPLY_MOV_SPEED,            // 8
		APPLY_CAST_SPEED,           // 9
		APPLY_HP_REGEN,             // 10
		APPLY_SP_REGEN,             // 11
		APPLY_POISON_PCT,           // 12
		APPLY_STUN_PCT,             // 13
		APPLY_SLOW_PCT,             // 14
		APPLY_CRITICAL_PCT,         // 15
		APPLY_PENETRATE_PCT,        // 16
		APPLY_ATTBONUS_HUMAN,       // 17
		APPLY_ATTBONUS_ANIMAL,      // 18
		APPLY_ATTBONUS_ORC,         // 19
		APPLY_ATTBONUS_MILGYO,      // 20
		APPLY_ATTBONUS_UNDEAD,      // 21
		APPLY_ATTBONUS_DEVIL,       // 22
		APPLY_STEAL_HP,             // 23
		APPLY_STEAL_SP,             // 24
		APPLY_MANA_BURN_PCT,        // 25
		APPLY_DAMAGE_SP_RECOVER,    // 26
		APPLY_BLOCK,                // 27
		APPLY_DODGE,                // 28
		APPLY_RESIST_SWORD,         // 29
		APPLY_RESIST_TWOHAND,       // 30
		APPLY_RESIST_DAGGER,        // 31
		APPLY_RESIST_BELL,          // 32
		APPLY_RESIST_FAN,           // 33
		APPLY_RESIST_BOW,           // 34
		APPLY_RESIST_FIRE,          // 35
		APPLY_RESIST_ELEC,          // 36
		APPLY_RESIST_MAGIC,         // 37
		APPLY_RESIST_WIND,          // 38
		APPLY_REFLECT_MELEE,        // 39
		APPLY_REFLECT_CURSE,        // 40
		APPLY_POISON_REDUCE,        // 41
		APPLY_KILL_SP_RECOVER,      // 42
		APPLY_EXP_DOUBLE_BONUS,     // 43
		APPLY_GOLD_DOUBLE_BONUS,    // 44
		APPLY_ITEM_DROP_BONUS,      // 45
		APPLY_POTION_BONUS,         // 46
		APPLY_KILL_HP_RECOVER,      // 47
		APPLY_IMMUNE_STUN,          // 48
		APPLY_IMMUNE_SLOW,          // 49
		APPLY_IMMUNE_FALL,          // 50
		APPLY_SKILL,                // 51
		APPLY_BOW_DISTANCE,         // 52
		APPLY_ATT_GRADE_BONUS,            // 53
		APPLY_DEF_GRADE_BONUS,            // 54
		APPLY_MAGIC_ATT_GRADE,      // 55
		APPLY_MAGIC_DEF_GRADE,      // 56
		APPLY_CURSE_PCT,            // 57
		APPLY_MAX_STAMINA,			// 58
		APPLY_ATT_BONUS_TO_WARRIOR, // 59
		APPLY_ATT_BONUS_TO_ASSASSIN,// 60
		APPLY_ATT_BONUS_TO_SURA,    // 61
		APPLY_ATT_BONUS_TO_SHAMAN,  // 62
		APPLY_ATT_BONUS_TO_MONSTER, // 63
		APPLY_MALL_ATTBONUS,
		APPLY_MALL_DEFBONUS,
		APPLY_MALL_EXPBONUS,
		APPLY_MALL_ITEMBONUS,
		APPLY_MALL_GOLDBONUS,
		APPLY_MAX_HP_PCT,
		APPLY_MAX_SP_PCT,
		APPLY_SKILL_DAMAGE_BONUS,
		APPLY_NORMAL_HIT_DAMAGE_BONUS,
		APPLY_SKILL_DEFEND_BONUS,
		APPLY_NORMAL_HIT_DEFEND_BONUS,
		APPLY_EXTRACT_HP_PCT,		//75
		APPLY_UNUSED_EXP_BONUS,		//76
		APPLY_UNUSED_DROP_BONUS,		//77
		APPLY_RESIST_WARRIOR,			//78
		APPLY_RESIST_ASSASSIN,			//79
		APPLY_RESIST_SURA,				//80
		APPLY_RESIST_SHAMAN,			//81
		APPLY_ENERGY,					//82
		APPLY_DEF_GRADE,
		APPLY_COSTUME_ATTR_BONUS,
		APPLY_MAGIC_ATTBONUS_PER,
		APPLY_MELEE_MAGIC_ATTBONUS_PER,

		APPLY_RESIST_ICE,
		APPLY_RESIST_EARTH,
		APPLY_RESIST_DARK,

		APPLY_ANTI_CRITICAL_PCT,
		APPLY_ANTI_PENETRATE_PCT,

#ifdef ENABLE_WOLFMAN_CHARACTER
		APPLY_BLEEDING_REDUCE = 92,	//92
		APPLY_BLEEDING_PCT = 93,	//93
		APPLY_ATTBONUS_WOLFMAN = 94,	//94
		APPLY_RESIST_WOLFMAN = 95,	//95
		APPLY_RESIST_CLAW = 96,	//96
#endif

#ifdef ENABLE_ACCE_SYSTEM
		APPLY_ACCEDRAIN_RATE = 97,	//97
#endif

#ifdef ENABLE_MAGIC_REDUCTION_SYSTEM
		APPLY_RESIST_MAGIC_REDUCTION = 98,	//98
#endif
		APPLY_MOUNT = 99,
		APPLY_ENCHANT_FIRE = 100,
		APPLY_ENCHANT_ICE = 101,
		APPLY_ENCHANT_EARTH = 102,
		APPLY_ENCHANT_DARK = 103,
		APPLY_ENCHANT_WIND = 104,
		APPLY_ENCHANT_ELECT = 105,
		APPLY_RESIST_HUMAN = 106,
		APPLY_ATTBONUS_SWORD = 107,
		APPLY_ATTBONUS_TWOHAND = 108,
		APPLY_ATTBONUS_DAGGER = 109,
		APPLY_ATTBONUS_BELL = 110,
		APPLY_ATTBONUS_FAN = 111,
		APPLY_ATTBONUS_BOW = 112,
		APPLY_ATTBONUS_CLAW = 113,
		APPLY_ATTBONUS_CZ = 114,
		APPLY_ATTBONUS_DESERT = 115,
		APPLY_ATTBONUS_INSECT = 116,
#ifdef ENABLE_ATTR_ADDONS
		APPLY_ATTBONUS_STONE = 117,
		APPLY_ATTBONUS_BOSS = 118,
		APPLY_ATTBONUS_ELEMENTS = 119,
		APPLY_ENCHANT_ELEMENTS = 120,
		APPLY_ATTBONUS_CHARACTERS = 121,
		APPLY_ENCHANT_CHARACTERS = 122,
#endif
#ifdef ENABLE_CHEST_DROP_POINT
		APPLY_CHEST_BONUS = 123,
#endif
		MAX_APPLY_NUM,
	};

	enum EPointTypes
	{
		POINT_NONE,                 // 0
		POINT_LEVEL,                // 1
		POINT_EXP,                  // 3
		POINT_NEXT_EXP,             // 4
		POINT_HP,                   // 5
		POINT_MAX_HP,               // 6
		POINT_SP,                   // 7
		POINT_MAX_SP,               // 8
		POINT_STAMINA,
		POINT_MAX_STAMINA,

		POINT_GOLD,                 // 11
		POINT_ST,
		POINT_HT,
		POINT_DX,
		POINT_IQ,
		POINT_ATT_POWER,
		POINT_ATT_SPEED,
		POINT_EVADE_RATE,
		POINT_MOV_SPEED,
		POINT_DEF_GRADE,
		POINT_CASTING_SPEED,
		POINT_MAGIC_ATT_GRADE,
		POINT_MAGIC_DEF_GRADE,
		POINT_EMPIRE_POINT,
		POINT_LEVEL_STEP,
		POINT_STAT,
		POINT_SUB_SKILL,
		POINT_SKILL,
		POINT_MIN_ATK,
		POINT_MAX_ATK,
		POINT_PLAYTIME,
		POINT_HP_REGEN,
		POINT_SP_REGEN,

		POINT_BOW_DISTANCE,

		POINT_HP_RECOVERY,
		POINT_SP_RECOVERY,

		POINT_POISON_PCT,
		POINT_STUN_PCT,
		POINT_SLOW_PCT,
		POINT_CRITICAL_PCT,
		POINT_PENETRATE_PCT,
		POINT_CURSE_PCT,

		POINT_ATTBONUS_HUMAN,
		POINT_ATTBONUS_ANIMAL,
		POINT_ATTBONUS_ORC,
		POINT_ATTBONUS_MILGYO,
		POINT_ATTBONUS_UNDEAD,
		POINT_ATTBONUS_DEVIL,
		POINT_ATTBONUS_INSECT,
		POINT_ATTBONUS_FIRE,
		POINT_ATTBONUS_ICE,
		POINT_ATTBONUS_DESERT,
		POINT_ATTBONUS_UNUSED0,     // 53 UNUSED0
		POINT_ATTBONUS_UNUSED1,     // 54 UNUSED1
		POINT_ATTBONUS_UNUSED2,     // 55 UNUSED2
		POINT_ATTBONUS_UNUSED3,     // 56 UNUSED3
		POINT_ATTBONUS_UNUSED4,     // 57 UNUSED4
		POINT_ATTBONUS_UNUSED5,     // 58 UNUSED5
		POINT_ATTBONUS_UNUSED6,     // 59 UNUSED6
		POINT_ATTBONUS_UNUSED7,     // 60 UNUSED7
		POINT_ATTBONUS_UNUSED8,     // 61 UNUSED8
		POINT_ATTBONUS_UNUSED9,     // 62 UNUSED9

		POINT_STEAL_HP,
		POINT_STEAL_SP,

		POINT_MANA_BURN_PCT,

		POINT_DAMAGE_SP_RECOVER,

		POINT_BLOCK,
		POINT_DODGE,

		POINT_RESIST_SWORD,         // 69
		POINT_RESIST_TWOHAND,       // 70
		POINT_RESIST_DAGGER,        // 71
		POINT_RESIST_BELL,          // 72
		POINT_RESIST_FAN,           // 73
		POINT_RESIST_BOW,
		POINT_RESIST_FIRE,
		POINT_RESIST_ELEC,
		POINT_RESIST_MAGIC,
		POINT_RESIST_WIND,

		POINT_REFLECT_MELEE,

		POINT_REFLECT_CURSE,
		POINT_POISON_REDUCE,

		POINT_KILL_SP_RECOVER,
		POINT_EXP_DOUBLE_BONUS,     // 83
		POINT_GOLD_DOUBLE_BONUS,    // 84
		POINT_ITEM_DROP_BONUS,      // 85

		POINT_POTION_BONUS,         // 86
		POINT_KILL_HP_RECOVER,      // 87

		POINT_IMMUNE_STUN,          // 88
		POINT_IMMUNE_SLOW,          // 89
		POINT_IMMUNE_FALL,          // 90
		//////////////////

		POINT_PARTY_ATT_GRADE,      // 91
		POINT_PARTY_DEF_GRADE,      // 92

		POINT_ATT_BONUS,            // 93
		POINT_DEF_BONUS,            // 94

		POINT_ATT_GRADE_BONUS,			// 95
		POINT_DEF_GRADE_BONUS,			// 96
		POINT_MAGIC_ATT_GRADE_BONUS,	// 97
		POINT_MAGIC_DEF_GRADE_BONUS,	// 98

		POINT_RESIST_NORMAL_DAMAGE,		// 99

		POINT_STAT_RESET_COUNT = 111,
		POINT_HORSE_SKILL = 112,

		POINT_MALL_ATTBONUS,
		POINT_MALL_DEFBONUS,
		POINT_MALL_EXPBONUS,
		POINT_MALL_ITEMBONUS,
		POINT_MALL_GOLDBONUS,
		POINT_MAX_HP_PCT,
		POINT_MAX_SP_PCT,

		POINT_SKILL_DAMAGE_BONUS,
		POINT_NORMAL_HIT_DAMAGE_BONUS,

		POINT_SKILL_DEFEND_BONUS,
		POINT_NORMAL_HIT_DEFEND_BONUS,
		POINT_UNUSED_EXP_BONUS,        // 125
		POINT_UNUSED_DROP_BONUS,

		POINT_ENERGY = 128,

		POINT_ENERGY_END_TIME = 129,
		POINT_COSTUME_ATTR_BONUS = 130,
		POINT_MAGIC_ATT_BONUS_PER = 131,
		POINT_MELEE_MAGIC_ATT_BONUS_PER = 132,

		POINT_RESIST_ICE = 133,
		POINT_RESIST_EARTH = 134,
		POINT_RESIST_DARK = 135,

		POINT_RESIST_CRITICAL = 136,
		POINT_RESIST_PENETRATE = 137,

#ifdef ENABLE_WOLFMAN_CHARACTER
		POINT_BLEEDING_REDUCE = 138,
		POINT_BLEEDING_PCT = 139,

		POINT_ATTBONUS_WOLFMAN = 140,
		POINT_RESIST_WOLFMAN = 141,
		POINT_RESIST_CLAW = 142,
#endif

#ifdef ENABLE_ACCE_SYSTEM
		POINT_ACCEDRAIN_RATE = 143,
#endif
#ifdef ENABLE_MAGIC_REDUCTION_SYSTEM
		POINT_RESIST_MAGIC_REDUCTION = 144,
#endif
#ifdef ENABLE_CHEQUE_SYSTEM
		POINT_CHEQUE = 145,
#endif
#ifdef ENABLE_PENDANT_SYSTEM
		POINT_ENCHANT_FIRE = 146,
		POINT_ENCHANT_ICE = 147,
		POINT_ENCHANT_EARTH = 148,
		POINT_ENCHANT_DARK = 149,
		POINT_ENCHANT_WIND = 150,
		POINT_ENCHANT_ELECT = 151,
		POINT_RESIST_HUMAN = 152,

		POINT_ATTBONUS_SWORD = 153,
		POINT_ATTBONUS_TWOHAND = 154,
		POINT_ATTBONUS_DAGGER = 155,
		POINT_ATTBONUS_BELL = 156,
		POINT_ATTBONUS_FAN = 157,
		POINT_ATTBONUS_BOW = 158,
#ifdef ENABLE_WOLFMAN_CHARACTER
		POINT_ATTBONUS_CLAW = 159,
#endif
		POINT_ATTBONUS_CZ = 160,
#endif
#ifdef ENABLE_ATTR_ADDONS
		POINT_ATTBONUS_STONE = 161,
		POINT_ATTBONUS_BOSS = 162,
#endif
#ifdef ENABLE_PARTY_ROLE_REWORK
		POINT_PARTY_ATTACKER_MONSTER_BONUS = 163,
		POINT_PARTY_ATTACKER_STONE_BONUS = 164,
		POINT_PARTY_ATTACKER_BOSS_BONUS = 165,
#endif
#ifdef ENABLE_ATTR_ADDONS
		POINT_ATTBONUS_ELEMENTS = 166,
		POINT_ENCHANT_ELEMENTS = 167,
		POINT_ATTBONUS_CHARACTERS = 168,
		POINT_ENCHANT_CHARACTERS = 169,
#endif
#ifdef ENABLE_CHEST_DROP_POINT
		POINT_CHEST_BONUS = 170,
#endif
#ifdef ENABLE_BATTLE_PASS_SYSTEM
		POINT_BATTLE_PASS_ID = 171,
#endif
		POINT_MIN_WEP = 200,
		POINT_MAX_WEP,
		POINT_MIN_MAGIC_WEP,
		POINT_MAX_MAGIC_WEP,
		POINT_HIT_RATE,
	};

	enum EImmuneFlags
	{
		IMMUNE_PARA = (1 << 0),
		IMMUNE_CURSE = (1 << 1),
		IMMUNE_STUN = (1 << 2),
		IMMUNE_SLEEP = (1 << 3),
		IMMUNE_SLOW = (1 << 4),
		IMMUNE_POISON = (1 << 5),
		IMMUNE_TERROR = (1 << 6),
	};
#ifdef ENABLE_SHINING_ITEM_SYSTEM
	enum EShiningSubTypes
	{
		SHINING_WEAPON,
		SHINING_ARMOR,
		SHINING_SPECIAL,
		SHINING_SPECIAL2,
		SHINING_SPECIAL3,
		SHINING_WING,
	};
#endif

#ifdef ENABLE_CAKRA_ITEM_SYSTEM
	enum ECakraSubTypes
	{
		CAKRA_ITEM_1,
		CAKRA_ITEM_2,
		CAKRA_ITEM_3,
		CAKRA_ITEM_4,
		CAKRA_ITEM_5,
		CAKRA_ITEM_6,
		CAKRA_ITEM_7,
		CAKRA_ITEM_8,
	};
#endif

#ifdef ENABLE_SEBNEM_ITEM_SYSTEM
	enum ESebnemSubTypes
	{
		SEBNEM_ITEM_1,
		SEBNEM_ITEM_2,
		SEBNEM_ITEM_3,
		SEBNEM_ITEM_4,
		SEBNEM_ITEM_5,
		SEBNEM_ITEM_6,
	};
#endif
#if defined(ENABLE_ACCE_SYSTEM)
	enum EAcceMisc
	{
		ACCE_GRADE_MAX_NUM = 5,
	};
#endif

#pragma pack(push)
#pragma pack(1)
	typedef struct SItemLimit
	{
		BYTE        bType;
		long        lValue;
	} TItemLimit;

	typedef struct SItemApply
	{
		BYTE        bType;
		long        lValue;
	} TItemApply;

	typedef struct SItemTable
	{
		DWORD       dwVnum;
		DWORD       dwVnumRange;
		char        szName[ITEM_NAME_MAX_LEN + 1];
		char        szLocaleName[ITEM_NAME_MAX_LEN + 1];
		BYTE        bType;
		BYTE        bSubType;

		BYTE        bWeight;
		BYTE        bSize;

		DWORD       dwAntiFlags;
		DWORD       dwFlags;
		DWORD       dwWearFlags;
		DWORD       dwImmuneFlag;

#ifdef ENABLE_GOLD_LIMIT_REWORK
		long long       llIBuyItemPrice;
		long long		llISellItemPrice;
#else
		DWORD       dwIBuyItemPrice;
		DWORD		dwISellItemPrice;
#endif

		TItemLimit  aLimits[ITEM_LIMIT_MAX_NUM];
		TItemApply  aApplies[ITEM_APPLY_MAX_NUM];
		long        alValues[ITEM_VALUES_MAX_NUM];
		long        alSockets[ITEM_SOCKET_MAX_NUM];
		DWORD       dwRefinedVnum;
		DWORD		dwRefineSet;
		BYTE        bAlterToMagicItemPct;
		BYTE		bSpecular;
		BYTE        bGainSocketPct;
	} TItemTable;

#pragma pack(pop)

public:
	CItemData();
	virtual ~CItemData();

	void Clear();
	void SetSummary(const std::string& c_rstSumm);
	void SetDescription(const std::string& c_rstDesc);

	CGraphicThing* GetModelThing();
	CGraphicThing* GetSubModelThing();
	CGraphicThing* GetDropModelThing();
	CGraphicSubImage* GetIconImage();
	bool HasRealIcon();

	DWORD GetLODModelThingCount();
	BOOL GetLODModelThingPointer(DWORD dwIndex, CGraphicThing** ppModelThing);

	DWORD GetAttachingDataCount();
	BOOL GetCollisionDataPointer(DWORD dwIndex, const NRaceData::TAttachingData** c_ppAttachingData);
	BOOL GetAttachingDataPointer(DWORD dwIndex, const NRaceData::TAttachingData** c_ppAttachingData);

	/////
	const TItemTable* GetTable() const;
	DWORD GetIndex() const;
	const char* GetName() const;
	const char* GetDescription() const;
	const char* GetSummary() const;
	BYTE GetType() const;
	BYTE GetSubType() const;
	UINT GetRefine() const;
	const char* GetUseTypeString() const;
	DWORD GetWeaponType() const;
	BYTE GetSize() const;
	BOOL IsAntiFlag(DWORD dwFlag) const;
	BOOL IsFlag(DWORD dwFlag) const;
	BOOL IsWearableFlag(DWORD dwFlag) const;
	BOOL HasNextGrade() const;
	DWORD GetWearFlags() const;
#ifdef ENABLE_GOLD_LIMIT_REWORK
	long long GetIBuyItemPrice() const;
	long long GetISellItemPrice() const;
#else
	DWORD GetIBuyItemPrice() const;
	DWORD GetISellItemPrice() const;
#endif
	BOOL GetLimit(BYTE byIndex, TItemLimit* pItemLimit) const;
	BOOL GetApply(BYTE byIndex, TItemApply* pItemApply) const;
	long GetValue(BYTE byIndex) const;
	long GetSocket(BYTE byIndex) const;
	long SetSocket(BYTE byIndex, DWORD value);
	int GetSocketCount() const;
	DWORD GetIconNumber() const;

	UINT	GetSpecularPoweru() const;
	float	GetSpecularPowerf() const;

	/////

	BOOL IsEquipment() const;

	/////

	//BOOL LoadItemData(const char * c_szFileName);
	void SetDefaultItemData(const char* c_szIconFileName, const char* c_szModelFileName = NULL);
	void SetItemTableData(TItemTable* pItemTable);

#ifdef ENABLE_SHINING_EFFECT_UTILITY
public:
	struct TItemShiningTable
	{
		char szShinings[ITEM_SHINING_MAX_COUNT][256];
	public:
		bool Any() const
		{
			for (int i = 0; i < CItemData::ITEM_SHINING_MAX_COUNT; i++)
			{
				if (strcmp(szShinings[i], ""))
				{
					return true;
				}
			}
			return false;
		}
	};

	void SetItemShiningTableData(BYTE bIndex, const char* szEffectname);
	CItemData::TItemShiningTable GetItemShiningTable() { return m_ItemShiningTable; }
#endif

#ifdef ENABLE_ACCE_SYSTEM
protected:
	typedef struct SItemScaleTable
	{
		D3DXVECTOR3 v3Scale[NRaceData::SEX_MAX_NUM][NRaceData::JOB_MAX_NUM];
		float fScaleParticle[NRaceData::SEX_MAX_NUM][NRaceData::JOB_MAX_NUM];
	} TItemScaleTable;
	TItemScaleTable m_ItemScaleTable;

public:
	float GetItemParticleScale(BYTE bJob, BYTE bSex);
	void SetItemTableScaleData(BYTE bJob, BYTE bSex, float fScaleX, float fScaleY, float fScaleZ, float fScaleParticle);
	D3DXVECTOR3& GetItemScaleVector(BYTE bJob, BYTE bSex);
#endif

#ifdef ENABLE_AURA_COSTUME_SYSTEM
	enum EAuraMisc
	{
		AURA_GRADE_MAX_NUM = 7,
	};
protected:
	typedef struct SAuraScaleTable
	{
		D3DXVECTOR3 v3MeshScale[NRaceData::SEX_MAX_NUM][NRaceData::JOB_MAX_NUM];
		float fParticleScale[NRaceData::SEX_MAX_NUM][NRaceData::JOB_MAX_NUM];
	} TAuraScaleTable;

	TAuraScaleTable m_AuraScaleTable;
	DWORD m_dwAuraEffectID;

public:
	void SetAuraScaleTableData(BYTE byJob, BYTE bySex, float fMeshScaleX, float fMeshScaleY, float fMeshScaleZ, float fParticleScale);
	D3DXVECTOR3& GetAuraMeshScaleVector(BYTE byJob, BYTE bySex);
	float GetAuraParticleScale(BYTE byJob, BYTE bySex);

	void SetAuraEffectID(const char* szAuraEffectPath);
	DWORD GetAuraEffectID() const { return m_dwAuraEffectID; }
#endif

protected:
	void __LoadFiles();
	void __SetIconImage(const char* c_szFileName);

protected:
	std::string m_strModelFileName;
	std::string m_strSubModelFileName;
	std::string m_strDropModelFileName;
	std::string m_strIconFileName;
	std::string m_strDescription;
	std::string m_strSummary;
	std::vector<std::string> m_strLODModelFileNameVector;

	CGraphicThing* m_pModelThing;
	CGraphicThing* m_pSubModelThing;
	CGraphicThing* m_pDropModelThing;
	CGraphicSubImage* m_pIconImage;
	std::vector<CGraphicThing*> m_pLODModelThingVector;

	NRaceData::TAttachingDataVector m_AttachingDataVector;
	TItemTable m_ItemTable;
#ifdef ENABLE_SHINING_EFFECT_UTILITY
	TItemShiningTable m_ItemShiningTable;
#endif

public:
	static void DestroySystem();

	static CItemData* New();
	static void Delete(CItemData* pkItemData);

	static CDynamicPool<CItemData>		ms_kPool;
};
