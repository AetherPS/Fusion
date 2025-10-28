#pragma once

enum SceNotificationRequestType
{
	NotificationRequest = 0,
	SystemNotification = 1,
	SystemNotificationWithUserId = 2,
	SystemNotificationWithDeviceId = 3,
	SystemNotificationWithDeviceIdRelatedToUser = 4,
	SystemNotificationWithText = 5,
	SystemNotificationWithTextRelatedToUser = 6,
	SystemNotificationWithErrorCode = 7,
	SystemNotificationWithAppId = 8,
	SystemNotificationWithAppName = 9,
	SystemNotificationWithAppInfo = 9,
	SystemNotificationWithAppNameRelatedToUser = 10,
	SystemNotificationWithParams = 11,
	SendSystemNotificationWithUserName = 12,
	SystemNotificationWithUserNameInfo = 13,
	SendAddressingSystemNotification = 14,
	AddressingSystemNotificationWithDeviceId = 15,
	AddressingSystemNotificationWithUserName = 16,
	AddressingSystemNotificationWithUserId = 17,

	Debug = 100,
	TrcCheckNotificationRequest = 101,
	NpDebugNotificationRequest = 102,
	WebDebug = 102,
	UNK_103 = 103,
};

#pragma pack(push, 1)
struct SceNotificationRequest
{
	enum SceNotificationRequestType Type;   // 0x00
	uint32_t ReqId;            				// 0x04
	uint32_t Priority;						// 0x08
	uint32_t MsgId;            				// 0x0C
	uint32_t TargetId;						// 0x10
	uint32_t UserId;						// 0x14
	uint32_t unk_18;						// 0x18
	uint32_t unk_1C;						// 0x1C
	uint32_t AppId;            				// 0x20
	uint32_t ErrorNumber;					// 0x24
	uint32_t Attribute;						// 0x28
	uint8_t  HasIcon;						// 0x2C
	union
	{
		struct
		{
			char Message[0x400];			// 0x2D
			char IconImageUri[0x800];		// 0x42D
			char padding[0x3];
		};

		struct
		{
			char unk1[180];					// 0x2D
			char unk2[180];					// 0xE1
			char unk3[180];					// 0x195

		};
	};
};
#pragma pack(pop)
static_assert(sizeof(SceNotificationRequest) == 0xC30, "Size of SceNotificationRequest is not 0xC30");

int isprint(int ch);
void hexdump(void* ptr, int buflen, bool showAddress);
void SendNotificationRequest(bool bank, SceNotificationRequest* req, size_t size, bool ReadWrite);
void NotifyCustom(char* icon, const char* fmt, ...);
void MemcpyTextSeg(void* dst, void* src, size_t len);
void* AllocateForMap(vm_map_t map, vm_ooffset_t offset, vm_offset_t size, vm_prot_t prot, vm_prot_t max);
void* KmemAllocAt(vm_map_t map, vm_ooffset_t offset, vm_offset_t size);
char* FileNameFromPath(char* path);
bool GetSandboxPath(thread* td, char* sandboxPath);
void UnHideDriver(char* deviceName);
bool DoesFileExist(const char* path);
int MkDir(const char* path, int mode);
void MakeTempDmemConfig();
int GetTempDmemConfig();
int dlsym(proc* p, int handle, char* symbol, char* library, unsigned int flags, void* addr);