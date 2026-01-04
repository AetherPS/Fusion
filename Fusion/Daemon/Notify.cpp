#include "stdafx.h"
#include "Notify.h"

void NotifyCustom(const char* icon, const char* fmt, ...)
{
	OrbisNotificationRequest Buffer;

	va_list args;
	va_start(args, fmt);
	vsprintf(Buffer.message, fmt, args);
	va_end(args);

	Buffer.type = OrbisNotificationRequestType::NotificationRequest;
	Buffer.unk3 = 0;
	Buffer.useIconImageUri = 1;
	Buffer.targetId = -1;
	strcpy(Buffer.iconUri, icon);

	sceKernelSendNotificationRequest(0, &Buffer, 3120, 0);
}

void Notify(const char* fmt, ...)
{
	char buffer[1024];

	va_list args;
	va_start(args, fmt);
	vsprintf(buffer, fmt, args);
	va_end(args);

	NotifyCustom("cxml://psnotification/tex_icon_system", buffer);
}