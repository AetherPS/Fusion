#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "List.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations for kernel structs */
struct mtx;
struct proc;

/* Macro definitions */
#define CCP_OP(cmd) ((cmd) >> 24)
#define SWAP_16(x) ((((uint16_t)(x) & 0xff) << 8) | ((uint16_t)(x) >> 8))
#define BE16(val) SWAP_16(val)
#define LE32(val) (val)

enum
{
    EKPFS_SIZE = 0x20,
    EEKPFS_SIZE = 0x100,
    PFS_SEED_SIZE = 0x10,
    PFS_FINAL_KEY_SIZE = 0x20,
    SIZEOF_PFS_KEY_BLOB = 0x140,
    CONTENT_KEY_SEED_SIZE = 0x10,
    SELF_KEY_SEED_SIZE = 0x10,
    EEKC_SIZE = 0x20,
    MAX_FAKE_KEYS = 32,
    SIZEOF_RSA_KEY = 0x48,
    PFS_FAKE_OBF_KEY_ID = 0x1337,
    SIZEOF_PFS_HEADER = 0x5A0,

    /* RIF */
    RIF_DATA_SIZE = 0x90,
    RIF_DIGEST_SIZE = 0x10,
    RIF_KEY_TABLE_SIZE = 0x230,
    RIF_MAX_KEY_SIZE = 0x20,
    SIZEOF_ACTDAT = 0x200,
    SIZEOF_RIF = 0x400,
    RIF_PAYLOAD_SIZE = (RIF_DIGEST_SIZE + RIF_DATA_SIZE),

    CCP_MAX_PAYLOAD_SIZE = 0x88,
    CCP_OP_AES = 0,
    CCP_OP_XTS = 2,
    CCP_OP_HMAC = 9,
    CCP_USE_KEY_FROM_SLOT = (1 << 18),
    CCP_GENERATE_KEY_AT_SLOT = (1 << 19),
    CCP_USE_KEY_HANDLE = (1 << 20),

    SCE_SBL_ERROR_NPDRM_ENOTSUP = 0x800F0A25,
    SIZEOF_SBL_KEY_RBTREE_ENTRY = 0xA8, /* sceSblKeymgrSetKey */
    SIZEOF_SBL_MAP_LIST_ENTRY = 0x50, /* sceSblDriverMapPages */
    TYPE_SBL_KEY_RBTREE_ENTRY_DESC_OFFSET = 0x04,
    TYPE_SBL_KEY_RBTREE_ENTRY_LOCKED_OFFSET = 0x80,
    SIZEOF_SBL_KEY_DESC = 0x7C, /* sceSblKeymgrSetKey */
    SBL_MSG_SERVICE_MAILBOX_MAX_SIZE = 0x80,
    SBL_MSG_CCP = 0x8
};

typedef union _SblKeyDesc
{
    struct _Pfs
    {
        uint16_t obfuscatedKeyId;
        uint16_t keySize;
        uint8_t escrowedKey[0x20];
    } Pfs;
    struct _Portability
    {
        uint16_t command;
        uint16_t pad;
        uint16_t keyId;
    } Portability;
    uint8_t raw[SIZEOF_SBL_KEY_DESC];
} SblKeyDesc;

typedef struct _SblKeySlotDesc
{
    uint32_t keyId;
    uint32_t unk04;
    uint32_t keyHandle; /* -1 if freed */
    uint32_t unk12;
    TAILQ_ENTRY(_SblKeySlotDesc) list;
} SblKeySlotDesc;

TAILQ_HEAD(_SblKeySlotQueue, _SblKeySlotDesc);

typedef struct _SblKeyRbtreeEntry
{
    uint32_t handle;
    uint32_t occupied;
    SblKeyDesc desc;
    uint8_t pad[0x4];
    /* uint32_t locked; this seems wrong, it says 0x80, but that's in the SblKeyDesc?? */
    struct _SblKeyRbtreeEntry* left;
    struct _SblKeyRbtreeEntry* right;
    struct _SblKeyRbtreeEntry* parent;
    uint32_t set;
} SblKeyRbtreeEntry;

typedef struct _FakeKeyDesc
{
    uint8_t key[0x20];
    bool occupied;
} FakeKeyDesc;

typedef struct _FakeKeyD
{
    uint32_t index;
    uint8_t seed[PFS_SEED_SIZE];
} FakeKeyD;

typedef struct _Ekc
{
    uint8_t contentKeySeed[CONTENT_KEY_SEED_SIZE];
    uint8_t selfKeySeed[SELF_KEY_SEED_SIZE];
} Ekc;

struct ccp_link
{
    void* p;
};

union ccp_op
{
    struct
    {
        uint32_t cmd;
        uint32_t status;
    } common;
    struct
    {
        uint32_t cmd;
        uint32_t status;
        uint64_t data_size;
        uint64_t in_data;
        uint64_t out_data;
        union
        {
            uint32_t key_index;
            uint8_t key[0x20];
        };
        uint8_t iv[0x10];
    } aes;
    uint8_t buf[CCP_MAX_PAYLOAD_SIZE];
};

struct ccp_msg
{
    union ccp_op op;
    uint32_t index;
    uint32_t result;
    TAILQ_ENTRY(ccp_msg) next;
    uint64_t message_id;
    LIST_ENTRY(ccp_link) links;
};

struct ccp_req
{
    TAILQ_HEAD(, ccp_msg) msgs;
    void (*cb)(void* arg, int result);
    void* arg;
    uint64_t message_id;
    LIST_ENTRY(ccp_link) links;
};

typedef union _SblMsgService
{
    struct
    {
        union ccp_op op;
    } ccp;

} SblMsgService;

typedef struct _SblMsgHeader
{
    uint32_t cmd;
    uint32_t status;
    uint64_t message_id;
    uint64_t extended_msgs;
} SblMsgHeader;

typedef struct _SblMsg
{
    SblMsgHeader hdr;
    union
    {
        SblMsgService service;
        uint8_t raw[0x1000];
    };

} SblMsg;

typedef struct _RifKeyBlob
{
    Ekc eekc;
    uint8_t entitlementKey[0x10];
} RifKeyBlob;

typedef union _PfsKeyBlob
{
    struct _In
    {
        uint8_t eekpfs[EEKPFS_SIZE];
        Ekc eekc;
        uint32_t pubkeyVer; /* 0x1/0x80000001/0xC0000001 */
        uint32_t keyVer;    /* 1 (if (rif_ver_major & 0x1) != 0, then pfs_key_ver=1, otherwise pfs_key_ver=0) */
        uint64_t headerGva;
        uint32_t headerSize;
        uint32_t type;
        uint32_t finalized;
        uint32_t isDisc;
    } In;
    struct _Out
    {
        uint8_t escrowedKeys[0x40];
    } Out;

} PfsKeyBlob;

typedef union _KeymgrPayload
{
    struct
    {
        uint32_t cmd;
        uint32_t status;
        uint64_t data;
    };
    uint8_t buf[0x80];
} KeymgrPayload;

typedef struct _RsaKey
{
    uint8_t _pad00[0x20];
    uint8_t* p;
    uint8_t* q;
    uint8_t* dmp1;
    uint8_t* dmq1;
    uint8_t* iqmp;
} RsaKey;

typedef struct _ActDat
{
    uint32_t magic;
    uint16_t versionMajor;
    uint16_t versionMinor;
    uint64_t accountId;
    uint64_t startTime;
    uint64_t endTime;
    uint64_t flags;
    uint32_t unk3;
    uint32_t unk4;
    uint8_t _pad30[0x30];
    uint8_t openPsidHash[0x20];
    uint8_t staticPerConsoleData1[0x20];
    uint8_t digest[0x10];
    uint8_t keyTable[0x20];
    uint8_t staticPerConsoleData2[0x10];
    uint8_t staticPerConsoleData3[0x20];
    uint8_t signature[0x100];
} ActDat;

typedef struct _Rif
{
    uint32_t magic;
    uint16_t versionMajor;
    uint16_t versionMinor;
    uint64_t accountId;
    uint64_t startTime;
    uint64_t endTime;
    char contentId[0x30];
    uint16_t format;
    uint16_t drmType;
    uint16_t contentType;
    uint16_t skuFlag;
    uint64_t contentFlags;
    uint32_t iroTag;
    uint32_t ekcVersion;
    uint8_t _pad6A[2];
    uint16_t unk3;
    uint16_t unk4;
    uint8_t _pad6E[0x1F2];
    uint8_t digest[0x10];
    uint8_t data[RIF_DATA_SIZE];
    uint8_t signature[0x100];
} Rif;

typedef struct _RsaBuffer
{
    uint8_t* ptr;
    size_t size;
} RsaBuffer;

typedef struct _PfsHeader
{
    uint8_t _pad00[0x370];
    uint8_t cryptSeed[0x10];
    uint8_t _pad380[0x220];
} PfsHeader;

typedef union _KeymgrResponse
{
    struct
    {
        uint32_t type;
        uint8_t key[RIF_MAX_KEY_SIZE];
        uint8_t data[RIF_DIGEST_SIZE + RIF_DATA_SIZE];
    } DecryptRif;
    struct
    {
        uint8_t raw[SIZEOF_RIF];
    } DecryptEntireRif;
} KeymgrResponse;

typedef union _KeymgrRequest
{
    struct
    {
        uint32_t type;
        uint8_t key[RIF_MAX_KEY_SIZE];
        uint8_t data[RIF_DIGEST_SIZE + RIF_DATA_SIZE];
    } DecryptRif;

    struct
    {
        Rif rif;
        uint8_t keyTable[RIF_KEY_TABLE_SIZE];
        uint64_t timestamp;
        int status;
    } DecryptEntireRif;
} KeymgrRequest;

typedef enum _SelfFormat
{
    None,
    Elf,
    Self,
    Count
} SelfFormat;

typedef struct _SelfHeader
{
    uint32_t magic;             /*0x00*/
    uint8_t version;            /*0x04*/
    uint8_t mode;               /*0x05*/
    uint8_t endian;             /*0x06*/
    uint8_t attr;               /*0x07*/
    uint32_t keyType;           /*0x08*/
    uint16_t headerSize;        /*0x0C*/
    uint16_t metaSize;          /*0x0E*/
    uint64_t fileSize;          /*0x10*/
    uint16_t numEntries;        /*0x18*/
    uint16_t flags;             /*0x1A*/
} SelfHeader;

typedef struct _SelfContext
{
    SelfFormat format;          /*0x00*/
    int32_t elfAuthType;        /*0x04*/
    uint32_t totalHeaderSize;   /*0x08*/
    uint32_t unk12;             /*0x0C*/
    void* segment;              /*0x10*/
    uint32_t unk24;             /*0x18*/
    int32_t contextId;          /*0x1C*/
    uint64_t serviceId;         /*0x20*/
    uint64_t unk40;             /*0x28*/
    int32_t bufferId;           /*0x30*/
    uint32_t unk52;             /*0x34*/
    SelfHeader* header;         /*0x38*/
    struct mtx lock;            /*0x40*/
} SelfContext;

typedef struct _SelfAuthInfo
{
    uint64_t paid;              /*0x00*/
    uint64_t caps[4];           /*0x08*/
    uint64_t attrs[4];          /*0x28*/
    uint8_t _0x0040[0x40];
} SelfAuthInfo;

typedef struct _SelfFakeAuthInfo
{
    uint64_t size;
    SelfAuthInfo info;
} SelfFakeAuthInfo;

typedef struct _SelfExInfo
{
    uint64_t paid;
    uint64_t ptype;
    uint64_t appVersion;
    uint64_t firmwareVersion;
    uint8_t digest[0x20];
} SelfExInfo;

typedef struct _SelfEntry
{
    uint64_t props;
    uint64_t offset;
    uint64_t fileSize;
    uint64_t memorySize;
} SelfEntry;

typedef struct _MailboxMessage
{
    int16_t funcId; /* 2 */
    char pad02[2];
    int32_t retVal; /* Return Value */
    uint64_t unk08;
    uint32_t unk16;
    uint32_t unk20;
    uint64_t unk24;
    uint64_t unk32;
    uint64_t unk40;
    uint32_t unk48;
    char unk52[76];
} MailboxMessage;

typedef struct _SblMapListEntry
{
    struct _SblMapListEntry* next;
    struct _SblMapListEntry* prev;
    uint64_t cpuVa;
    uint32_t numPageGroups;
    uint64_t gpuVa;
    void* pageGroups;
    uint32_t numPages;
    uint64_t flags;
    struct proc* proc;
    void* vmPage;
} SblMapListEntry;

#ifdef __cplusplus
}
#endif
