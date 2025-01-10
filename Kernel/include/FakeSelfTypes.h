#pragma once

typedef enum _SelfFormat
{
    None,
    Elf,
    Self,
    Count
} SelfFormat;

typedef struct _SelfHeader
{
    uint32_t magic;             //0x00
    uint8_t version;            //0x04
    uint8_t mode;               //0x05
    uint8_t endian;             //0x06
    uint8_t attr;               //0x07
    uint32_t keyType;           //0x08
    uint16_t headerSize;        //0x0C
    uint16_t metaSize;          //0x0E
    uint64_t fileSize;          //0x10
    uint16_t numEntries;        //0x18
    uint16_t flags;             //0x1A
} SelfHeader;

typedef struct _SelfContext
{
    SelfFormat format;          //0x00
    int32_t elfAuthType;        //0x04
    uint32_t totalHeaderSize;   //0x08
    uint32_t unk12;             //0x0C
    void* segment;              //0x10
    uint32_t unk24;             //0x18
    int32_t contextId;          //0x1C
    uint64_t serviceId;         //0x20
    uint64_t unk40;             //0x28
    int32_t bufferId;           //0x30
    uint32_t unk52;             //0x34
    _SelfHeader* header;        //0x38
    struct mtx lock;            //0x40
} SelfContext;

typedef struct _SelfAuthInfo
{
    uint64_t paid;              //0x00
    uint64_t caps[4];           //0x08
    uint64_t attrs[4];          //0x28
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
    int16_t funcId; // 2
    char pad02[2];
    int32_t retVal; // Return Value
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
    _SblMapListEntry* next;
    _SblMapListEntry* prev;
    uint64_t cpuVa;
    uint32_t numPageGroups;
    uint64_t gpuVa;
    void* pageGroups;
    uint32_t numPages;
    uint64_t flags;
    struct proc* proc;
    void* vmPage;
} SblMapListEntry;