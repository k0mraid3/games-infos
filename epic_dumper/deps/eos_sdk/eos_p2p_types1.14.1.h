// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#pragma pack(push, 8)

EXTERN_C typedef struct EOS_P2PHandle* EOS_HP2P;

/**
 * A packet's maximum size in bytes
 */
#define EOS_P2P_MAX_PACKET_SIZE 1170

 /**
  * The maximum amount of unique Socket ID connections that can be opened with each remote user. As this limit is only per remote user, you may have more
  * than this number of Socket IDs across multiple remote users.
  */
#define EOS_P2P_MAX_CONNECTIONS 32

/**
 * Categories of NAT strictness.
 */
EOS_ENUM(EOS_ENATType,
	/** NAT type either unknown (remote) or we are unable to determine it (local) */
	EOS_NAT_Unknown = 0,
	/** All peers can directly-connect to you */
	EOS_NAT_Open = 1,
	/** You can directly-connect to other Moderate and Open peers */
	EOS_NAT_Moderate = 2,
	/** You can only directly-connect to Open peers */
	EOS_NAT_Strict = 3
);

/** The most recent version of the EOS_P2P_SocketId structure. */
#define EOS_P2P_SOCKETID_API_001 1

/**
 * P2P Socket ID
 *
 * The Socket ID contains an application-defined name for the connection between a local person and another peer.
 *
 * When a remote user receives a connection request from you, they will receive this information.  It can be important
 * to only accept connections with a known socket-name and/or from a known user, to prevent leaking of private
 * information, such as a user's IP address. Using the socket name as a secret key can help prevent such leaks. Shared
 * private data, like a private match's Session ID are good candidates for a socket name.
 */
EOS_STRUCT(EOS_P2P_SocketId, (
	/** API Version: Set this to EOS_P2P_SOCKETID_API_LATEST. */
	int32_t ApiVersion;
	/** A name for the connection. Must be a NULL-terminated string of between 1-32 alpha-numeric characters (A-Z, a-z, 0-9) */
	char SocketName[33];
));

/**
 * Types of packet reliability.
 *
 * Ordered packets will only be ordered relative to other ordered packets. Reliable/unreliable and ordered/unordered communication
 * can be sent on the same Socket ID and Channel.
 */
EOS_ENUM(EOS_EPacketReliability,
	/** Packets will only be sent once and may be received out of order */
	EOS_PR_UnreliableUnordered = 0,
	/** Packets may be sent multiple times and may be received out of order */
	EOS_PR_ReliableUnordered = 1,
	/** Packets may be sent multiple times and will be received in order */
	EOS_PR_ReliableOrdered = 2
);

/** The most recent version of the EOS_P2P_SendPacket API. */
#define EOS_P2P_SENDPACKET_API_002 2

/**
 * Structure containing information about the data being sent and to which player
 */
EOS_STRUCT(EOS_P2P_SendPacketOptions002, (
	/** API Version: Set this to EOS_P2P_SENDPACKET_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the local user who is sending this packet */
	EOS_ProductUserId LocalUserId;
	/** The Product User ID of the Peer you would like to send a packet to */
	EOS_ProductUserId RemoteUserId;
	/** The socket ID for data you are sending in this packet */
	const EOS_P2P_SocketId* SocketId;
	/** Channel associated with this data */
	uint8_t Channel;
	/** The size of the data to be sent to the RemoteUser */
	uint32_t DataLengthBytes;
	/** The data to be sent to the RemoteUser */
	const void* Data;
	/** If false and we do not already have an established connection to the peer, this data will be dropped */
	EOS_Bool bAllowDelayedDelivery;
	/** Setting to control the delivery reliability of this packet */
	EOS_EPacketReliability Reliability;
));

/** The most recent version of the EOS_P2P_GetNextReceivedPacketSize API. */
#define EOS_P2P_GETNEXTRECEIVEDPACKETSIZE_API_002 2

/**
 * Structure containing information about who would like to receive a packet.
 */
EOS_STRUCT(EOS_P2P_GetNextReceivedPacketSizeOptions002, (
	/** API Version: Set this to EOS_P2P_GETNEXTRECEIVEDPACKETSIZE_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the local user who is receiving the packet */
	EOS_ProductUserId LocalUserId;
	/** An optional channel to request the data for. If NULL, we're retrieving the size of the next packet on any channel. */
	const uint8_t* RequestedChannel;

));

/** The most recent version of the EOS_P2P_ReceivePacket API. */
#define EOS_P2P_RECEIVEPACKET_API_002 2

/**
 * Structure containing information about who would like to receive a packet, and how much data can be stored safely.
 */
EOS_STRUCT(EOS_P2P_ReceivePacketOptions002, (
	/** API Version: Set this to EOS_P2P_RECEIVEPACKET_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the user who is receiving the packet */
	EOS_ProductUserId LocalUserId;
	/** The maximum amount of data in bytes that can be safely copied to OutData in the function call */
	uint32_t MaxDataSizeBytes;
	/** An optional channel to request the data for. If NULL, we're retrieving the next packet on any channel */
	const uint8_t* RequestedChannel;
));

/** The most recent version of the EOS_P2P_AddNotifyPeerConnectionRequest API. */
#define EOS_P2P_ADDNOTIFYPEERCONNECTIONREQUEST_API_001 1

/**
 * Structure containing information about who would like connection notifications, and about which socket.
 */
EOS_STRUCT(EOS_P2P_AddNotifyPeerConnectionRequestOptions001, (
	/** API Version: Set this to EOS_P2P_ADDNOTIFYPEERCONNECTIONREQUEST_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the user who is listening for incoming connection requests */
	EOS_ProductUserId LocalUserId;
	/** The optional socket ID to listen for, used as a filter for incoming connection requests; If NULL, incoming connection requests will not be filtered */
	const EOS_P2P_SocketId* SocketId;
));

/**
 * Structure containing information about an incoming connection request.
 */
EOS_STRUCT(EOS_P2P_OnIncomingConnectionRequestInfo, (
	enum { k_iCallback = k_iP2PCallbackBase + 0 };
	/** Client-specified data passed into EOS_Presence_AddNotifyOnPresenceChanged */
	void* ClientData;
	/** The local user who is being requested to open a P2P session with RemoteUserId */
	EOS_ProductUserId LocalUserId;
	/** The remote user who requested a peer connection with the local user */
	EOS_ProductUserId RemoteUserId;
	/** The ID of the socket the Remote User wishes to communicate on */
	const EOS_P2P_SocketId* SocketId;
));

/**
 * Callback for information related to incoming connection requests.
 */
EOS_DECLARE_CALLBACK(EOS_P2P_OnIncomingConnectionRequestCallback, const EOS_P2P_OnIncomingConnectionRequestInfo* Data);

/** The most recent version of the EOS_P2P_AddNotifyPeerConnectionClosed API. */
#define EOS_P2P_ADDNOTIFYPEERCONNECTIONCLOSED_API_001 1

/**
 * Structure containing information about who would like notifications about closed connections, and for which socket.
 */
EOS_STRUCT(EOS_P2P_AddNotifyPeerConnectionClosedOptions001, (
	/** API Version: Set this to EOS_P2P_ADDNOTIFYPEERCONNECTIONCLOSED_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the local user who would like notifications */
	EOS_ProductUserId LocalUserId;
	/** The optional socket ID to listen for to be closed. If NULL, this handler will be called for all closed connections */
	const EOS_P2P_SocketId* SocketId;
));

/**
 * Reasons why a P2P connection was closed
 */
EOS_ENUM(EOS_EConnectionClosedReason,
	/** The connection was closed for unknown reasons */
	EOS_CCR_Unknown = 0,
	/** The connection was gracefully closed by the local user */
	EOS_CCR_ClosedByLocalUser = 1,
	/** The connection was gracefully closed by the remote user */
	EOS_CCR_ClosedByPeer = 2,
	/** The connection timed out */
	EOS_CCR_TimedOut = 3,
	/** The connection could not be created due to too many other connections */
	EOS_CCR_TooManyConnections = 4,
	/** The remote user sent an invalid message */
	EOS_CCR_InvalidMessage = 5,
	/** The remote user sent us invalid data */
	EOS_CCR_InvalidData = 6,
	/** We failed to establish a connection with the remote user */
	EOS_CCR_ConnectionFailed = 7,
	/** The connection was unexpectedly closed */
	EOS_CCR_ConnectionClosed = 8,
	/** We failed to negotiate a connection with the remote user */
	EOS_CCR_NegotiationFailed = 9,
	/** There was an unexpected error and the connection cannot continue */
	EOS_CCR_UnexpectedError = 10
);

/**
 * Structure containing information about an connection request that is being closed.
 */
EOS_STRUCT(EOS_P2P_OnRemoteConnectionClosedInfo, (
	enum { k_iCallback = k_iP2PCallbackBase + 1 };
	/** Client-specified data passed into EOS_Presence_AddNotifyOnPresenceChanged */
	void* ClientData;
	/** The local user who is being notified of a connection being closed */
	EOS_ProductUserId LocalUserId;
	/** The remote user who this connection was with */
	EOS_ProductUserId RemoteUserId;
	/** The socket ID of the connection being closed */
	const EOS_P2P_SocketId* SocketId;
	/** The reason the connection was closed (if known) */
	EOS_EConnectionClosedReason Reason;
));

/**
 * Callback for information related to open connections being closed.
 */
EOS_DECLARE_CALLBACK(EOS_P2P_OnRemoteConnectionClosedCallback, const EOS_P2P_OnRemoteConnectionClosedInfo* Data);

/** The most recent version of the EOS_P2P_AddNotifyPeerConnectionEstablished API. */
#define EOS_P2P_ADDNOTIFYPEERCONNECTIONESTABLISHED_API_001 1

/**
 * Structure containing information about which connections should be notified
 */
EOS_STRUCT(EOS_P2P_AddNotifyPeerConnectionEstablishedOptions001, (
	/** API Version: Set this to EOS_P2P_ADDNOTIFYPEERCONNECTIONESTABLISHED_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the local user who would like to receive notifications */
	EOS_ProductUserId LocalUserId;
	/** The optional socket ID, used as a filter for established connections. If NULL, this handler will be called for all sockets */
	const EOS_P2P_SocketId* SocketId;
));

/**
 * Type of established connection
 */
EOS_ENUM(EOS_EConnectionEstablishedType,
	/** The connection is brand new */
	EOS_CET_NewConnection = 0,
	/** The connection is reestablished (reconnection) */
	EOS_CET_Reconnection = 1
);

/**
 * Structure containing information about a connection being established
 */
EOS_STRUCT(EOS_P2P_OnPeerConnectionEstablishedInfo, (
	enum { k_iCallback = k_iP2PCallbackBase + 2 };
	/** Client-specified data passed into EOS_P2P_AddNotifyPeerConnectionEstablished */
	void* ClientData;
	/** The Product User ID of the local user who is being notified of a connection being established */
	EOS_ProductUserId LocalUserId;
	/** The Product User ID of the remote user who this connection was with */
	EOS_ProductUserId RemoteUserId;
	/** The socket ID of the connection being established */
	const EOS_P2P_SocketId* SocketId;
	/** Information if this is a new connection or reconnection */
	EOS_EConnectionEstablishedType ConnectionType;
));

/**
 * Callback for information related to new connections being established
 */
EOS_DECLARE_CALLBACK(EOS_P2P_OnPeerConnectionEstablishedCallback, const EOS_P2P_OnPeerConnectionEstablishedInfo* Data);

/** The most recent version of the EOS_P2P_AcceptConnection API. */
#define EOS_P2P_ACCEPTCONNECTION_API_001 1

/**
 * Structure containing information about who would like to accept a connection, and which connection.
 */
EOS_STRUCT(EOS_P2P_AcceptConnectionOptions001, (
	/** API Version: Set this to EOS_P2P_ACCEPTCONNECTION_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the local user who is accepting any pending or future connections with RemoteUserId */
	EOS_ProductUserId LocalUserId;
	/** The Product User ID of the remote user who has either sent a connection request or is expected to in the future */
	EOS_ProductUserId RemoteUserId;
	/** The socket ID of the connection to accept on */
	const EOS_P2P_SocketId* SocketId;
));

/** The most recent version of the EOS_P2P_CloseConnection API. */
#define EOS_P2P_CLOSECONNECTION_API_001 1

/**
 * Structure containing information about who would like to close a connection, and which connection.
 */
EOS_STRUCT(EOS_P2P_CloseConnectionOptions001, (
	/** API Version: Set this to EOS_P2P_CLOSECONNECTION_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the local user who would like to close a previously accepted connection (or decline a pending invite) */
	EOS_ProductUserId LocalUserId;
	/** The Product User ID of the remote user to disconnect from (or to reject a pending invite from) */
	EOS_ProductUserId RemoteUserId;
	/** The socket ID of the connection to close (or optionally NULL to not accept any connection requests from the Remote User) */
	const EOS_P2P_SocketId* SocketId;
));

/** The most recent version of the EOS_P2P_CloseConnections API. */
#define EOS_P2P_CLOSECONNECTIONS_API_001 1

/**
 * Structure containing information about who would like to close connections, and by what socket ID
 */
EOS_STRUCT(EOS_P2P_CloseConnectionsOptions001, (
	/** API Version: Set this to EOS_P2P_CLOSECONNECTIONS_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the local user who would like to close all connections that use a particular socket ID */
	EOS_ProductUserId LocalUserId;
	/** The socket ID of the connections to close */
	const EOS_P2P_SocketId* SocketId;
));

/** The most recent version of the EOS_P2P_QueryNATType API. */
#define EOS_P2P_QUERYNATTYPE_API_001 1

/**
 * Structure containing information needed to query NAT-types
 */
EOS_STRUCT(EOS_P2P_QueryNATTypeOptions001, (
	/** API Version: Set this to EOS_P2P_QUERYNATTYPE_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Structure containing information about the local network NAT type
 */
EOS_STRUCT(EOS_P2P_OnQueryNATTypeCompleteInfo, (
	enum { k_iCallback = k_iP2PCallbackBase + 3 };
	/** Result code for the operation. EOS_Success is returned for a successful query, other codes indicate an error */
	EOS_EResult ResultCode;
	/** Client-specified data passed into EOS_P2P_QueryNATType */
	void* ClientData;
	/** The queried NAT type */
	EOS_ENATType NATType;
));

/**
 * Callback for information related to our NAT type query completing.
 */
EOS_DECLARE_CALLBACK(EOS_P2P_OnQueryNATTypeCompleteCallback, const EOS_P2P_OnQueryNATTypeCompleteInfo* Data);

/** The most recent version of the EOS_P2P_GetNATType API. */
#define EOS_P2P_GETNATTYPE_API_001 1

/**
 * Structure containing information needed to get perviously queried NAT-types
 */
EOS_STRUCT(EOS_P2P_GetNATTypeOptions001, (
	/** API Version: Set this to EOS_P2P_GETNATTYPE_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Setting for controlling whether relay servers are used
 */
EOS_ENUM(EOS_ERelayControl,
	/** Peer connections will never attempt to use relay servers. Clients with restrictive NATs may not be able to connect to peers. */
	EOS_RC_NoRelays = 0,
	/** Peer connections will attempt to use relay servers, but only after direct connection attempts fail. This is the default value if not changed. */
	EOS_RC_AllowRelays = 1,
	/** Peer connections will only ever use relay servers. This will add latency to all connections, but will hide IP Addresses from peers. */
	EOS_RC_ForceRelays = 2
);

/** The most recent version of the EOS_P2P_SetRelayControl API. */
#define EOS_P2P_SETRELAYCONTROL_API_001 1

/**
 * Structure containing information about new relay configurations.
 */
EOS_STRUCT(EOS_P2P_SetRelayControlOptions001, (
	/** API Version: Set this to EOS_P2P_SETRELAYCONTROL_API_LATEST. */
	int32_t ApiVersion;
	/**
	 * The requested level of relay servers for P2P connections. This setting is only applied to new P2P connections, or when existing P2P connections
	 * reconnect during a temporary connectivity outage. Peers with an incompatible setting to the local setting will not be able to connnect.
	 */
	EOS_ERelayControl RelayControl;
));

/** The most recent version of the EOS_P2P_GetRelayControl API. */
#define EOS_P2P_GETRELAYCONTROL_API_001 1

/**
 * Structure containing information about getting the relay control setting.
 */
EOS_STRUCT(EOS_P2P_GetRelayControlOptions001, (
	/** API Version of the EOS_P2P_GetRelayControlOptions structure */
	int32_t ApiVersion;
));

/** The most recent version of the EOS_P2P_SetPortRange API. */
#define EOS_P2P_SETPORTRANGE_API_001 1

/**
 * Structure containing information about new port range settings.
 */
EOS_STRUCT(EOS_P2P_SetPortRangeOptions001, (
	/** API Version: Set this to EOS_P2P_SETPORTRANGE_API_LATEST. */
	int32_t ApiVersion;
	/** The ideal port to use for P2P traffic. The default value is 7777. If set to 0, the OS will choose a port. If set to 0, MaxAdditionalPortsToTry must be set to 0. */
	uint16_t Port;
	/**
	 * The maximum amount of additional ports to try if Port is unavailable. Ports will be tried from Port to Port + MaxAdditionalPortsToTry
	 * inclusive, until one is available or we run out of ports. If no ports are available, P2P connections will fail. The default value is 99.
	 */
	uint16_t MaxAdditionalPortsToTry;
));

/** The most recent version of the EOS_P2P_GetPortRange API. */
#define EOS_P2P_GETPORTRANGE_API_001 1

/**
 * Structure containing information about getting the configured port range settings.
 */
EOS_STRUCT(EOS_P2P_GetPortRangeOptions001, (
	/** API Version: Set this to EOS_P2P_GETPORTRANGE_API_LATEST. */
	int32_t ApiVersion;
));


/** Helper constant to signify that the packet queue is allowed to grow indefinitely */
#define EOS_P2P_MAX_QUEUE_SIZE_UNLIMITED 0

/** The most recent version of the EOS_P2P_SetPacketQueueSize API. */
#define EOS_P2P_SETPACKETQUEUESIZE_API_001 1

/**
 * Structure containing information about new packet queue size settings.
 */
EOS_STRUCT(EOS_P2P_SetPacketQueueSizeOptions001, (
	/** API Version: Set this to EOS_P2P_SETPACKETQUEUESIZE_API_LATEST. */
	int32_t ApiVersion;
	/** The ideal maximum amount of bytes the Incoming packet queue can consume */
	uint64_t IncomingPacketQueueMaxSizeBytes;
	/** The ideal maximum amount of bytes the Outgoing packet queue can consume */
	uint64_t OutgoingPacketQueueMaxSizeBytes;
));


/** The most recent version of the EOS_P2P_GetPacketQueueInfo API. */
#define EOS_P2P_GETPACKETQUEUEINFO_API_001 1

/**
 * Structure containing information needed to get the current packet queue information.
 */
EOS_STRUCT(EOS_P2P_GetPacketQueueInfoOptions001, (
	/** API Version: Set this to EOS_P2P_GETPACKETQUEUEINFO_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Information related to the current state of the packet queues. It is possible for the current size
 * to be larger than the maximum size if the maximum size changes or if the maximum queue size is
 * set to EOS_P2P_MAX_QUEUE_SIZE_UNLIMITED.
 */
EOS_STRUCT(EOS_P2P_PacketQueueInfo, (
	/** The maximum size in bytes of the incoming packet queue */
	uint64_t IncomingPacketQueueMaxSizeBytes;
	/** The current size in bytes of the incoming packet queue */
	uint64_t IncomingPacketQueueCurrentSizeBytes;
	/** The current number of queued packets in the incoming packet queue */
	uint64_t IncomingPacketQueueCurrentPacketCount;
	/** The maximum size in bytes of the outgoing packet queue */
	uint64_t OutgoingPacketQueueMaxSizeBytes;
	/** The current size in bytes of the outgoing packet queue */
	uint64_t OutgoingPacketQueueCurrentSizeBytes;
	/** The current amount of queued packets in the outgoing packet queue */
	uint64_t OutgoingPacketQueueCurrentPacketCount;
));


/** The most recent version of the EOS_P2P_AddNotifyIncomingPacketQueueFull API. */
#define EOS_P2P_ADDNOTIFYINCOMINGPACKETQUEUEFULL_API_001 1

/**
 * Structure containing information about what version of the EOS_P2P_AddNotifyIncomingPacketQueueFull function is supported.
 */
EOS_STRUCT(EOS_P2P_AddNotifyIncomingPacketQueueFullOptions001, (
	/** API Version: Set this to EOS_P2P_ADDNOTIFYINCOMINGPACKETQUEUEFULL_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Structure containing information about the packet queue's state and the incoming packet that would overflow the queue
 */
EOS_STRUCT(EOS_P2P_OnIncomingPacketQueueFullInfo, (
	enum { k_iCallback = k_iP2PCallbackBase + 4 };
	/** Client-specified data passed into AddNotifyIncomingPacketQueueFull */
	void* ClientData;
	/** The maximum size in bytes the incoming packet queue is allowed to use */
	uint64_t PacketQueueMaxSizeBytes;
	/** The current size in bytes the incoming packet queue is currently using */
	uint64_t PacketQueueCurrentSizeBytes;
	/** The Product User ID of the local user who is receiving the packet that would overflow the queue */
	EOS_ProductUserId OverflowPacketLocalUserId;
	/** The channel the incoming packet is for */
	uint8_t OverflowPacketChannel;
	/** The size in bytes of the incoming packet (and related metadata) that would overflow the queue */
	uint32_t OverflowPacketSizeBytes;
));

/**
 * Callback for information related to incoming connection requests.
 */
EOS_DECLARE_CALLBACK(EOS_P2P_OnIncomingPacketQueueFullCallback, const EOS_P2P_OnIncomingPacketQueueFullInfo* Data);

/** The most recent version of the EOS_P2P_ClearPacketQueue API. */
#define EOS_P2P_CLEARPACKETQUEUE_API_001 1

/**
 * Structure containing information about the packet queue to be cleared
 */
EOS_STRUCT(EOS_P2P_ClearPacketQueueOptions001, (
	/** API Version: Set this to EOS_P2P_CLEARPACKETQUEUE_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the local user for whom we want to clear the queued packets */
	EOS_ProductUserId LocalUserId;
	/** The Product User ID to who (outgoing) or from who (incoming) packets are queued */
	EOS_ProductUserId RemoteUserId;
	/** The socket used for packets to be cleared */
	const EOS_P2P_SocketId* SocketId;
));

#pragma pack(pop)
