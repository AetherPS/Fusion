#include "Common.h"
#include "Fusion.h"

KernelAddrs g_KernelAddrs;
uint64_t KernelBase;

extern "C" int _main(uint64_t kernelBase, KernelAddrs* addrs)
{
	KernelBase = kernelBase;
	InitResolver(addrs);
	memcpy(&g_KernelAddrs, addrs, sizeof(KernelAddrs));

	InitFusion();
	return 0;
}