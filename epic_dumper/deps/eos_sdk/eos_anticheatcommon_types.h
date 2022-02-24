#pragma once

#include "eos_common.h"

enum { k_iAntiCheatCommonCallbackBase = 21000 };
// next free callback_id: k_iAntiCheatCommonCallbackBase + 3

#define EOS_AntiCheatCommon_SetClientDetailsOptions    EOS_AntiCheatCommon_SetClientDetailsOptions001
#define EOS_AntiCheatCommon_SetGameSessionIdOptions    EOS_AntiCheatCommon_SetGameSessionIdOptions001
#define EOS_AntiCheatCommon_RegisterEventOptions       EOS_AntiCheatCommon_RegisterEventOptions001
#define EOS_AntiCheatCommon_LogEventOptions            EOS_AntiCheatCommon_LogEventOptions001
#define EOS_AntiCheatCommon_LogGameRoundStartOptions   EOS_AntiCheatCommon_LogGameRoundStartOptions001
#define EOS_AntiCheatCommon_LogGameRoundEndOptions     EOS_AntiCheatCommon_LogGameRoundEndOptions001
#define EOS_AntiCheatCommon_LogPlayerSpawnOptions      EOS_AntiCheatCommon_LogPlayerSpawnOptions001
#define EOS_AntiCheatCommon_LogPlayerDespawnOptions    EOS_AntiCheatCommon_LogPlayerDespawnOptions001
#define EOS_AntiCheatCommon_LogPlayerReviveOptions     EOS_AntiCheatCommon_LogPlayerReviveOptions001
#define EOS_AntiCheatCommon_LogPlayerTickOptions       EOS_AntiCheatCommon_LogPlayerTickOptions002
#define EOS_AntiCheatCommon_LogPlayerUseWeaponOptions  EOS_AntiCheatCommon_LogPlayerUseWeaponOptions002
#define EOS_AntiCheatCommon_LogPlayerUseAbilityOptions EOS_AntiCheatCommon_LogPlayerUseAbilityOptions001
#define EOS_AntiCheatCommon_LogPlayerTakeDamageOptions EOS_AntiCheatCommon_LogPlayerTakeDamageOptions003

#include "eos_anticheatcommon_types1.14.0.h"
#include "eos_anticheatcommon_types1.12.0.h"

#define EOS_ANTICHEATCOMMON_SETCLIENTDETAILS_API_LATEST    EOS_ANTICHEATCOMMON_SETCLIENTDETAILS_API_001
#define EOS_ANTICHEATCOMMON_SETGAMESESSIONID_API_LATEST    EOS_ANTICHEATCOMMON_SETGAMESESSIONID_API_001
#define EOS_ANTICHEATCOMMON_REGISTEREVENT_API_LATEST       EOS_ANTICHEATCOMMON_REGISTEREVENT_API_001
#define EOS_ANTICHEATCOMMON_LOGEVENT_API_LATEST            EOS_ANTICHEATCOMMON_LOGEVENT_API_001
#define EOS_ANTICHEATCOMMON_LOGGAMEROUNDSTART_API_LATEST   EOS_ANTICHEATCOMMON_LOGGAMEROUNDSTART_API_001
#define EOS_ANTICHEATCOMMON_LOGGAMEROUNDEND_API_LATEST     EOS_ANTICHEATCOMMON_LOGGAMEROUNDEND_API_001
#define EOS_ANTICHEATCOMMON_LOGPLAYERSPAWN_API_LATEST      EOS_ANTICHEATCOMMON_LOGPLAYERSPAWN_API_001
#define EOS_ANTICHEATCOMMON_LOGPLAYERDESPAWN_API_LATEST    EOS_ANTICHEATCOMMON_LOGPLAYERDESPAWN_API_001
#define EOS_ANTICHEATCOMMON_LOGPLAYERREVIVE_API_LATEST     EOS_ANTICHEATCOMMON_LOGPLAYERREVIVE_API_001
#define EOS_ANTICHEATCOMMON_LOGPLAYERTICK_API_LATEST       EOS_ANTICHEATCOMMON_LOGPLAYERTICK_API_002
#define EOS_ANTICHEATCOMMON_LOGPLAYERUSEWEAPON_API_LATEST  EOS_ANTICHEATCOMMON_LOGPLAYERUSEWEAPON_API_002
#define EOS_ANTICHEATCOMMON_LOGPLAYERUSEABILITY_API_LATEST EOS_ANTICHEATCOMMON_LOGPLAYERUSEABILITY_API_001
#define EOS_ANTICHEATCOMMON_LOGPLAYERTAKEDAMAGE_API_LATEST EOS_ANTICHEATCOMMON_LOGPLAYERTAKEDAMAGE_API_003
