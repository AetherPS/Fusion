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

	UNK_1 = 100,
	TrcCheckNotificationRequest = 101,
	NpDebugNotificationRequest = 102,
	UNK_2 = 102,
};

typedef struct
{
	enum SceNotificationRequestType type;
	int reqId;
	int priority;
	int msgId;
	int targetId;
	int userId;
	int unk1;
	int unk2;
	int appId;
	int errorNum;
	int unk3;
	unsigned char useIconImageUri;
	char message[1024];
	char iconUri[1024];
	char unk[1024];
} SceNotificationRequest;

int isprint(int ch);
void hexdump(void* ptr, int buflen, bool showAddress);
void SendNotificationRequest(bool bank, SceNotificationRequest* req, size_t size, bool ReadWrite);
void NotifyCustom(char* icon, const char* fmt, ...);
void MemcpyTextSeg(void* dst, void* src, size_t len);
void* AllocateForMap(vm_map_t map, vm_ooffset_t offset, vm_offset_t size, vm_prot_t prot, vm_prot_t max);
void* KmemAllocAt(vm_map_t map, vm_ooffset_t offset, vm_offset_t size);
char* FileNameFromPath(char* path);
bool GetSandboxPath(thread* td, char* sandboxPath);
int fileExists(char* filename);
void UnHideDriver(char* deviceName);