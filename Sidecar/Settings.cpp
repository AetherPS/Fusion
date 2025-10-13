#include "stdafx.h"
#include "Settings.h"
//#include <json2.h>

#define SETTINGS_DIR "/data/Fusion/Settings.cfg"

void Settings::Init()
{
	//sceKernelLoadStartModule("/system/common/lib/libSceJson2.sprx", 0, 0, 0, 0, 0);
	// 
	//// Create the Settings file if it does not exist already.
	//if (!FileSystem::Exists(SETTINGS_DIR))
	//{
	//	Logger::Info("Create Settings.");
	//	Create();
	//}
	//
	//sce::Json::Value rootval;
	//sce::Json::Parser::parse(rootval, SETTINGS_DIR);
	//
	//EnableFTP = rootval.getValue("EnableFTP").getBoolean();
	//
	//for (const auto& plugin : rootval.getValue("Plugins").getArray())
	//{
	//	PluginList.push_back(plugin.getString().c_str());
	//}
}

void Settings::Create()
{
	//sce::Json::Value rootval;
	//sce::Json::Object obj;
	//
	//obj["EnableFTP"] = true;
	//
	//sce::Json::Array pluginList;
	//obj["Plugins"] = pluginList;
	//
	//// Push everything to the root.
	//rootval.set(sce::Json::Value(obj));
	//
	//// Serialize the json blob to string.
	//sce::Json::String jsonBlob;
	//if (rootval.serialize(jsonBlob) != 0)
	//	return;
	//
	//// Write the string blob to file.
	//WriteFile(SETTINGS_DIR, (void*)jsonBlob.c_str(), strlen(jsonBlob.c_str()));
}