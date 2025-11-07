#include "stdafx.h"
#include "Misc.h"

class MyAllocator : public sce::Json::MemAllocator
{
public:
	MyAllocator() {};
	~MyAllocator() {};
	virtual void* allocate(size_t size, void* user_data)
	{
		void* p = malloc(size);
		return p;
	}

	virtual void deallocate(void* ptr, void* user_data)
	{
		free(ptr);
	}
	virtual void notifyError(int32_t error, size_t size, void* userData)
	{
		switch (error) {
		case SCE_JSON_ERROR_NOMEM:
			printf("allocate Fail. size = %ld\n", size);
			abort();
		default:
			printf("unknown[%#x]\n", error);
			break;
		}
	}
};

bool LoadModules()
{
	auto res = sceSysmoduleLoadModuleInternal(SCE_SYSMODULE_INTERNAL_SYSTEM_SERVICE);
	if (res != SCE_OK)
	{
		Logger::Error("%s: Failed to load SCE_SYSMODULE_INTERNAL_SYSTEM_SERVICE (%llX)\n", __FUNCTION__, res);
		return false;
	}

	res = sceSysmoduleLoadModule(SCE_SYSMODULE_JSON2);
	if (res != SCE_OK)
	{
		Logger::Error("%s: Failed to load SCE_SYSMODULE_JSON2 (%llX)\n", __FUNCTION__, res);
		return false;
	}

	res = sceLncUtilInitialize();
	if (res != SCE_OK)
	{
		Logger::Error("%s: sceLncUtilInitialize failed (%llX)\n", __FUNCTION__, res);
		return false;
	}

	sce::Json::InitParameter2 initparam;
	MyAllocator allc;
	initparam.setAllocator((sce::Json::MemAllocator*)&allc, 0);
	initparam.setFileBufferSize(512);
	initparam.setSpecialFloatFormatType(sce::Json::kSpecialFloatFormatTypeSymbol);

	sce::Json::Initializer initializer;
	if (initializer.initialize(&initparam) != SCE_OK)
	{
		Logger::Error("%s: sce::Json::Initializer::initialize failed\n", __FUNCTION__);
		return false;
	}

	Logger::Success("%s: Success!\n", __FUNCTION__);
	return true;
}


void ExitGraceful()
{
	SceAppInfo info;
	sceKernelGetAppInfo(getpid(), &info);

	sceLncUtilKillApp(info.AppId);
}

