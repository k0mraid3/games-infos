#pragma once

#include "eos_common.h"

enum { k_iRTCAudioCallbackBase = 25000 };
// next free callback_id: k_iRTCAudioCallbackBase + 8

#define EOS_RTCAudio_RegisterPlatformAudioUserOptions    EOS_RTCAudio_RegisterPlatformAudioUserOptions001
#define EOS_RTCAudio_UnregisterPlatformAudioUserOptions  EOS_RTCAudio_UnregisterPlatformAudioUserOptions001
#define EOS_RTCAudio_AddNotifyParticipantUpdatedOptions  EOS_RTCAudio_AddNotifyParticipantUpdatedOptions001
#define EOS_RTCAudio_AddNotifyAudioDevicesChangedOptions EOS_RTCAudio_AddNotifyAudioDevicesChangedOptions001
#define EOS_RTCAudio_GetAudioInputDevicesCountOptions    EOS_RTCAudio_GetAudioInputDevicesCountOptions001
#define EOS_RTCAudio_GetAudioInputDeviceByIndexOptions   EOS_RTCAudio_GetAudioInputDeviceByIndexOptions001
#define EOS_RTCAudio_AudioInputDeviceInfo                EOS_RTCAudio_AudioInputDeviceInfo001
#define EOS_RTCAudio_GetAudioOutputDevicesCountOptions   EOS_RTCAudio_GetAudioOutputDevicesCountOptions001
#define EOS_RTCAudio_GetAudioOutputDeviceByIndexOptions  EOS_RTCAudio_GetAudioOutputDeviceByIndexOptions001
#define EOS_RTCAudio_AudioOutputDeviceInfo               EOS_RTCAudio_AudioOutputDeviceInfo001
#define EOS_RTCAudio_SetAudioInputSettingsOptions        EOS_RTCAudio_SetAudioInputSettingsOptions001
#define EOS_RTCAudio_SetAudioOutputSettingsOptions       EOS_RTCAudio_SetAudioOutputSettingsOptions001
#define EOS_RTCAudio_AddNotifyAudioInputStateOptions     EOS_RTCAudio_AddNotifyAudioInputStateOptions001
#define EOS_RTCAudio_AddNotifyAudioOutputStateOptions    EOS_RTCAudio_AddNotifyAudioOutputStateOptions001
#define EOS_RTCAudio_AddNotifyAudioBeforeSendOptions     EOS_RTCAudio_AddNotifyAudioBeforeSendOptions001
#define EOS_RTCAudio_AudioBuffer                         EOS_RTCAudio_AudioBuffer001
#define EOS_RTCAudio_AddNotifyAudioBeforeRenderOptions   EOS_RTCAudio_AddNotifyAudioBeforeRenderOptions001
#define EOS_RTCAudio_SendAudioOptions                    EOS_RTCAudio_SendAudioOptions001
#define EOS_RTCAudio_UpdateSendingOptions                EOS_RTCAudio_UpdateSendingOptions001
#define EOS_RTCAudio_UpdateReceivingOptions              EOS_RTCAudio_UpdateReceivingOptions001

#include "eos_rtc_audio_types1.14.0.h"

#define EOS_RTCAUDIO_REGISTERPLATFORMAUDIOUSER_API_LATEST     EOS_RTCAUDIO_REGISTERPLATFORMAUDIOUSER_API_001
#define EOS_RTCAUDIO_UNREGISTERPLATFORMAUDIOUSER_API_LATEST   EOS_RTCAUDIO_UNREGISTERPLATFORMAUDIOUSER_API_001
#define EOS_RTCAUDIO_ADDNOTIFYPARTICIPANTUPDATED_API_LATEST   EOS_RTCAUDIO_ADDNOTIFYPARTICIPANTUPDATED_API_001
#define EOS_RTCAUDIO_ADDNOTIFYAUDIODEVICESCHANGED_API_LATEST  EOS_RTCAUDIO_ADDNOTIFYAUDIODEVICESCHANGED_API_001
#define EOS_RTCAUDIO_GETAUDIOINPUTDEVICESCOUNT_API_LATEST     EOS_RTCAUDIO_GETAUDIOINPUTDEVICESCOUNT_API_001
#define EOS_RTCAUDIO_GETAUDIOINPUTDEVICEBYINDEX_API_LATEST    EOS_RTCAUDIO_GETAUDIOINPUTDEVICEBYINDEX_API_001
#define EOS_RTCAUDIO_AUDIOINPUTDEVICEINFO_API_LATEST          EOS_RTCAUDIO_AUDIOINPUTDEVICEINFO_API_001
#define EOS_RTCAUDIO_GETAUDIOOUTPUTDEVICESCOUNT_API_LATEST    EOS_RTCAUDIO_GETAUDIOOUTPUTDEVICESCOUNT_API_001
#define EOS_RTCAUDIO_GETAUDIOOUTPUTDEVICEBYINDEX_API_LATEST   EOS_RTCAUDIO_GETAUDIOOUTPUTDEVICEBYINDEX_API_001
#define EOS_RTCAUDIO_AUDIOOUTPUTDEVICEINFO_API_LATEST         EOS_RTCAUDIO_AUDIOOUTPUTDEVICEINFO_API_001
#define EOS_RTCAUDIO_SETAUDIOINPUTSETTINGS_API_LATEST         EOS_RTCAUDIO_SETAUDIOINPUTSETTINGS_API_001
#define EOS_RTCAUDIO_SETAUDIOOUTPUTSETTINGS_API_LATEST        EOS_RTCAUDIO_SETAUDIOOUTPUTSETTINGS_API_001
#define EOS_RTCAUDIO_ADDNOTIFYAUDIOINPUTSTATE_API_LATEST      EOS_RTCAUDIO_ADDNOTIFYAUDIOINPUTSTATE_API_001
#define EOS_RTCAUDIO_ADDNOTIFYAUDIOOUTPUTSTATE_API_LATEST     EOS_RTCAUDIO_ADDNOTIFYAUDIOOUTPUTSTATE_API_001
#define EOS_RTCAUDIO_ADDNOTIFYAUDIOBEFORESEND_API_LATEST      EOS_RTCAUDIO_ADDNOTIFYAUDIOBEFORESEND_API_001
#define EOS_RTCAUDIO_AUDIOBUFFER_API_LATEST                   EOS_RTCAUDIO_AUDIOBUFFER_API_001
#define EOS_RTCAUDIO_ADDNOTIFYAUDIOBEFORERENDER_API_LATEST    EOS_RTCAUDIO_ADDNOTIFYAUDIOBEFORERENDER_API_001
#define EOS_RTCAUDIO_SENDAUDIO_API_LATEST                     EOS_RTCAUDIO_SENDAUDIO_API_001
#define EOS_RTCAUDIO_UPDATESENDING_API_LATEST                 EOS_RTCAUDIO_UPDATESENDING_API_001
#define EOS_RTCAUDIO_UPDATERECEIVING_API_LATEST               EOS_RTCAUDIO_UPDATERECEIVING_API_001
