// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#pragma pack(push, 8)

#define EOS_PLATFORM_OPTIONS_API_005 5

/** Platform options for EOS_Platform_Create. */
EOS_STRUCT(EOS_Platform_Options005, (
	/** API version of EOS_Platform_Create. */
	int32_t ApiVersion;
	/** A reserved field that should always be nulled. */
	void* Reserved;
	/** The product id for the running application, found on the dev portal */
	const char* ProductId;
	/** The sandbox id for the running application, found on the dev portal */
	const char* SandboxId;
	/** Set of service permissions associated with the running application */
	EOS_Platform_ClientCredentials ClientCredentials;
	/** Is this running as a server */
	EOS_Bool bIsServer;
	/** 256-bit Encryption Key for file encryption in hexadecimal format (64 hex chars)*/
	const char* EncryptionKey;
	/** The override country code to use for the logged in user. (EOS_COUNTRYCODE_MAX_LEN)*/
	const char* OverrideCountryCode;
	/** The override locale code to use for the logged in user. This follows ISO 639. (EOS_LOCALECODE_MAX_LEN)*/
	const char* OverrideLocaleCode;
	/** The deployment id for the running application, found on the dev portal */
	const char* DeploymentId;
	/** Platform creation flags. This is a bitwise-or union of EOS_PF_... flags defined above */
	uint64_t Flags;
));

#pragma pack(pop)