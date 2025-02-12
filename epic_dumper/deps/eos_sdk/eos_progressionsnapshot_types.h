// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "eos_common.h"

enum { k_iProgressionSnapshotCallbackBase = 26000 };
// next free callback_id: enum { k_iCallback = k_iProgressionSnapshotCallbackBase + 2 };

#define EOS_ProgressionSnapshot_BeginSnapshotOptions  EOS_ProgressionSnapshot_BeginSnapshotOptions001
#define EOS_ProgressionSnapshot_AddProgressionOptions EOS_ProgressionSnapshot_AddProgressionOptions001
#define EOS_ProgressionSnapshot_SubmitSnapshotOptions EOS_ProgressionSnapshot_SubmitSnapshotOptions001
#define EOS_ProgressionSnapshot_EndSnapshotOptions    EOS_ProgressionSnapshot_EndSnapshotOptions001
#define EOS_ProgressionSnapshot_DeleteSnapshotOptions EOS_ProgressionSnapshot_DeleteSnapshotOptions001

#include "eos_progressionsnapshot_types1.14.0.h"

#define EOS_PROGRESSIONSNAPSHOT_BEGINSNAPSHOT_API_LATEST  EOS_PROGRESSIONSNAPSHOT_BEGINSNAPSHOT_API_001
#define EOS_PROGRESSIONSNAPSHOT_ADDPROGRESSION_API_LATEST EOS_PROGRESSIONSNAPSHOT_ADDPROGRESSION_API_001
#define EOS_PROGRESSIONSNAPSHOT_SUBMITSNAPSHOT_API_LATEST EOS_PROGRESSIONSNAPSHOT_SUBMITSNAPSHOT_API_001
#define EOS_PROGRESSIONSNAPSHOT_ENDSNAPSHOT_API_LATEST    EOS_PROGRESSIONSNAPSHOT_ENDSNAPSHOT_API_001
#define EOS_PROGRESSIONSNAPSHOT_DELETESNAPSHOT_API_LATEST EOS_PROGRESSIONSNAPSHOT_DELETESNAPSHOT_API_001