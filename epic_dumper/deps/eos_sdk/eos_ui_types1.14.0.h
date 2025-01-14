// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#pragma pack(push, 8)

/** Handle to the UI interface */
EXTERN_C typedef struct EOS_UIHandle* EOS_HUI;

/** ID representing a specific UI event. */
EXTERN_C typedef uint64_t EOS_UI_EventId;
#define EOS_UI_EVENTID_INVALID 0

/** The most recent version of the EOS_UI_ShowFriends API. */
#define EOS_UI_SHOWFRIENDS_API_001 1

/**
 * Input parameters for the EOS_UI_ShowFriends function.
 */
EOS_STRUCT(EOS_UI_ShowFriendsOptions001, (
	/** API Version: Set this to EOS_UI_SHOWFRIENDS_API_LATEST. */
	int32_t ApiVersion;
	/** The Epic Account ID of the user whose friend list is being shown. */
	EOS_EpicAccountId LocalUserId;
));

/**
 * Output parameters for the EOS_UI_ShowFriends function.
 */
EOS_STRUCT(EOS_UI_ShowFriendsCallbackInfo, (
	enum { k_iCallback = k_iUICallbackBase + 0 };
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_UI_ShowFriends */
	void* ClientData;
	/** The Epic Account ID of the user whose friend list is being shown. */
	EOS_EpicAccountId LocalUserId;
));

/**
 * Function prototype definition for callbacks passed to EOS_UI_ShowFriends
 * @param Data A EOS_UI_ShowFriendsCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_UI_OnShowFriendsCallback, const EOS_UI_ShowFriendsCallbackInfo* Data);

/** The most recent version of the EOS_UI_HideFriends API. */
#define EOS_UI_HIDEFRIENDS_API_001 1

/**
 * Input parameters for the EOS_UI_HideFriends function.
 */
EOS_STRUCT(EOS_UI_HideFriendsOptions001, (
	/** API Version: Set this to EOS_UI_HIDEFRIENDS_API_LATEST. */
	int32_t ApiVersion;
	/** The Epic Account ID of the user whose friend list is being shown. */
	EOS_EpicAccountId LocalUserId;
));

/**
 * Output parameters for the EOS_UI_HideFriends function.
 */
EOS_STRUCT(EOS_UI_HideFriendsCallbackInfo, (
	enum { k_iCallback = k_iUICallbackBase + 1 };
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_UI_HideFriends */
	void* ClientData;
	/** The Epic Account ID of the user whose friend list is being shown. */
	EOS_EpicAccountId LocalUserId;
));

/**
 * Function prototype definition for callbacks passed to EOS_UI_HideFriends
 * @param Data A EOS_UI_HideFriendsCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_UI_OnHideFriendsCallback, const EOS_UI_HideFriendsCallbackInfo* Data);

/** The most recent version of the EOS_UI_GetFriendsVisible API. */
#define EOS_UI_GETFRIENDSVISIBLE_API_001 1

/**
 * Input parameters for the EOS_UI_GetFriendsVisible function.
 */
EOS_STRUCT(EOS_UI_GetFriendsVisibleOptions001, (
	/** API Version: Set this to EOS_UI_GETFRIENDSVISIBLE_API_LATEST. */
	int32_t ApiVersion;
	/** The Epic Account ID of the user whose overlay is being updated. */
	EOS_EpicAccountId LocalUserId;
));

/** The most recent version of the EOS_UI_AddNotifyDisplaySettingsUpdated API. */
#define EOS_UI_ADDNOTIFYDISPLAYSETTINGSUPDATED_API_001 1

/**
 * Input parameters for the EOS_UI_AddNotifyDisplaySettingsUpdated function.
 */
EOS_STRUCT(EOS_UI_AddNotifyDisplaySettingsUpdatedOptions001, (
	/** API Version: Set this to EOS_UI_ADDNOTIFYDISPLAYSETTINGSUPDATED_API_LATEST. */
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_UI_OnDisplaySettingsUpdatedCallbackInfo, (
	enum { k_iCallback = k_iUICallbackBase + 2 };
	/** Context that was passed into EOS_UI_AddNotifyDisplaySettingsUpdated */
	void* ClientData;
	/** True when any portion of the overlay is visible. */
	EOS_Bool bIsVisible;
	/** 
	 * True when the overlay has switched to exclusive input mode. 
	 * While in exclusive input mode, no keyboard or mouse input will be sent to the game.
	 */
	EOS_Bool bIsExclusiveInput;
));

/**
 * Function prototype definition for callbacks passed to EOS_UI_AddNotifyDisplaySettingsUpdated 
 * @param Data A EOS_UI_OnDisplaySettingsUpdatedCallbackInfo containing the current display state.
 */
EOS_DECLARE_CALLBACK(EOS_UI_OnDisplaySettingsUpdatedCallback, const EOS_UI_OnDisplaySettingsUpdatedCallbackInfo* Data);

/** 
 * Enum flags for storing a key combination. The low 16 bits are the key type, and modifiers are 
 * stored in the next significant bits 
 */
#define EOS_UI_KEY_CONSTANT(Prefix, Name, Value) Prefix ## Name = Value,
#define EOS_UI_KEY_MODIFIER(Prefix, Name, Value) Prefix ## Name = Value,
#define EOS_UI_KEY_ENTRY_FIRST(Prefix, Name, Value) Prefix ## Name = Value,
#define EOS_UI_KEY_ENTRY(Prefix, Name) Prefix ## Name,
#define EOS_UI_KEY_CONSTANT_LAST(Prefix, Name) Prefix ## Name 
EOS_ENUM_START(EOS_UI_EKeyCombination)
#include "eos_ui_keys.h"
EOS_ENUM_END(EOS_UI_EKeyCombination);
#undef EOS_UI_KEY_CONSTANT
#undef EOS_UI_KEY_MODIFIER
#undef EOS_UI_KEY_ENTRY_FIRST
#undef EOS_UI_KEY_ENTRY
#undef EOS_UI_KEY_CONSTANT_LAST

EOS_ENUM_BOOLEAN_OPERATORS(EOS_UI_EKeyCombination)

/** The most recent version of the EOS_UI_SetToggleFriendsKey API. */
#define EOS_UI_SETTOGGLEFRIENDSKEY_API_001 1

/**
 * Input parameters for the EOS_UI_SetToggleFriendsKey function.
 */
EOS_STRUCT(EOS_UI_SetToggleFriendsKeyOptions001, (
	/** API Version: Set this to EOS_UI_SETTOGGLEFRIENDSKEY_API_LATEST. */
	int32_t ApiVersion;
	/** 
	 * The new key combination which will be used to toggle the friends overlay.
	 * The combination can be any set of modifiers and one key.
	 * A value of EOS_UIK_None will cause the key to revert to the default.
	 */
	EOS_UI_EKeyCombination KeyCombination;
));

/** The most recent version of the EOS_UI_GetToggleFriendsKey API. */
#define EOS_UI_GETTOGGLEFRIENDSKEY_API_001 1

/**
 * Input parameters for the EOS_UI_GetToggleFriendsKey function.
 */
EOS_STRUCT(EOS_UI_GetToggleFriendsKeyOptions001, (
	/** API Version: Set this to EOS_UI_GETTOGGLEFRIENDSKEY_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Notification locations to be used to set the preference
 * for pop-up.
 *
 * @see EOS_UI_SetDisplayPreference
 */
EOS_ENUM(EOS_UI_ENotificationLocation,
	EOS_UNL_TopLeft,
	EOS_UNL_TopRight,
	EOS_UNL_BottomLeft,
	EOS_UNL_BottomRight
);

/** The most recent version of the EOS_UI_SetDisplayPreference API. */
#define EOS_UI_SETDISPLAYPREFERENCE_API_001 1

/**
 * Input parameters for the EOS_UI_SetDisplayPreference function.
 */
EOS_STRUCT(EOS_UI_SetDisplayPreferenceOptions001, (
	/** API Version: Set this to EOS_UI_SETDISPLAYPREFERENCE_API_LATEST. */
	int32_t ApiVersion;
	/** Preference for notification pop-up locations. */
	EOS_UI_ENotificationLocation NotificationLocation;
));

/** The most recent version of the EOS_UI_AcknowledgeEventId API. */
#define EOS_UI_ACKNOWLEDGEEVENTID_API_001 1

/** DEPRECATED! Use EOS_UI_ACKNOWLEDGEEVENTID_API_LATEST instead. */
#define EOS_UI_ACKNOWLEDGECORRELATIONID_API_LATEST EOS_UI_ACKNOWLEDGEEVENTID_API_001

/**
 * Input parameters for the EOS_UI_AcknowledgeEventId.
 */
EOS_STRUCT(EOS_UI_AcknowledgeEventIdOptions001, (
    /** API Version: Set this to EOS_UI_ACKNOWLEDGEEVENTID_API_LATEST. */
	int32_t ApiVersion;
	/** The ID being acknowledged. */
	EOS_UI_EventId UiEventId;
	/** 
	 * The result to use for the acknowledgment. 
	 * When acknowledging EOS_Presence_JoinGameAcceptedCallbackInfo this should be the 
	 * result code from the JoinSession call.
	 */
	EOS_EResult Result;
));

/** The most recent version of the EOS_UI_ReportKeyEvent API. */
#define EOS_UI_REPORTKEYEVENT_API_LATEST 1

/**
 * Input parameters for the EOS_UI_ReportKeyEvent function.
 */
EOS_STRUCT(EOS_UI_ReportKeyEventOptions, (
	/** API Version: Set this to EOS_UI_REPORTKEYEVENT_API_LATEST. */
	int32_t ApiVersion;
	/** The input data pushed to the SDK. */
	const void* PlatformSpecificInputData;
));

/** The most recent version of the EOS_UI_PrePresent API. */
#define EOS_UI_PREPRESENT_API_LATEST 1

/**
 * Parameters for the EOS_UI_PrePresent function.
 */
EOS_STRUCT(EOS_UI_PrePresentOptions, (
	/** API Version: Set this to EOS_UI_PREPRESENT_API_LATEST. */
	int32_t ApiVersion;
	/** Platform specific data. */
	const void* PlatformSpecificData;
));

#pragma pack(pop)
