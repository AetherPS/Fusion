#pragma once

struct dynlib_load_prx_args
{
	const char* prx_path;
	int flags;
	int* handle_out;
	uint64_t unk;
};

class LibraryReplacer
{
public:
	static void Init();
	static void Term();

private:
	static int sys_dynlib_load_prxHook(thread* td, dynlib_load_prx_args* args);
};
