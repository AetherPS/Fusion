#include "stdafx.h"
#include "Settings.h"

#define SETTINGS_DIR "/data/Fusion/Settings.cfg"

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


void Settings::Init()
{
	if (sceSysmoduleLoadModule(SCE_SYSMODULE_JSON2) != SCE_OK) 
	{
		Logger::Error("Failed to load Json2 module.");
		return;
	}

	sce::Json::InitParameter2 initparam;
	MyAllocator allc;
	initparam.setAllocator((sce::Json::MemAllocator*)&allc, 0);
	initparam.setFileBufferSize(512);
	initparam.setSpecialFloatFormatType(sce::Json::kSpecialFloatFormatTypeSymbol);

	sce::Json::Initializer initializer;
	initializer.initialize(&initparam);

	// Create the Settings file if it does not exist already.
	if (!FileSystem::Exists(SETTINGS_DIR))
	{
		Logger::Info("Create Settings.");
		Create();
	}
	
	sce::Json::Value rootval;
	sce::Json::Parser::parse(rootval, SETTINGS_DIR);
	
	EnableFTP = rootval.getValue("EnableFTP").getBoolean();
	
	for (const auto& plugin : rootval.getValue("Plugins").getArray())
	{
		PluginList.push_back(plugin.getString().c_str());
	}
}

void Settings::Create()
{
	sce::Json::Value rootval;
	sce::Json::Object obj;
	
	obj["EnableFTP"] = true;
	
	sce::Json::Array pluginList;
	obj["Plugins"] = pluginList;
	
	// Push everything to the root.
	rootval.set(sce::Json::Value(obj));
	
	// Serialize the json blob to string.
	sce::Json::String jsonBlob;
	if (rootval.serialize(jsonBlob) != 0)
		return;
	
	// Write the string blob to file.
	FileSystem::Write(SETTINGS_DIR, (void*)jsonBlob.c_str(), strlen(jsonBlob.c_str()));
}