#include "common.h"
#include "sidecar.h"

void ascii()
{
	klog("#######################################################################################################################\n");
	klog("\n");
	klog("FFFFFFFFFFFFFFFFFFFFFF                                     iiii                                      333333333333333   \n");
	klog("F::::::::::::::::::::F                                    i::::i                                    3:::::::::::::::33 \n");
	klog("F::::::::::::::::::::F                                     iiii                                     3::::::33333::::::3\n");
	klog("FF::::::FFFFFFFFF::::F                                                                              3333333     3:::::3\n");
	klog("  F:::::F       FFFFFFuuuuuu    uuuuuu      ssssssssss   iiiiiii    ooooooooooo   nnnn  nnnnnnnn                3:::::3\n");
	klog("  F:::::F             u::::u    u::::u    ss::::::::::s  i:::::i  oo:::::::::::oo n:::nn::::::::nn              3:::::3\n");
	klog("  F::::::FFFFFFFFFF   u::::u    u::::u  ss:::::::::::::s  i::::i o:::::::::::::::on::::::::::::::nn     33333333:::::3 \n");
	klog("  F:::::::::::::::F   u::::u    u::::u  s::::::ssss:::::s i::::i o:::::ooooo:::::onn:::::::::::::::n    3:::::::::::3  \n");
	klog("  F:::::::::::::::F   u::::u    u::::u   s:::::s  ssssss  i::::i o::::o     o::::o  n:::::nnnn:::::n    33333333:::::3 \n");
	klog("  F::::::FFFFFFFFFF   u::::u    u::::u     s::::::s       i::::i o::::o     o::::o  n::::n    n::::n            3:::::3\n");
	klog("  F:::::F             u::::u    u::::u        s::::::s    i::::i o::::o     o::::o  n::::n    n::::n            3:::::3\n");
	klog("  F:::::F             u:::::uuuu:::::u  ssssss   s:::::s  i::::i o::::o     o::::o  n::::n    n::::n            3:::::3\n");
	klog("FF:::::::FF           u:::::::::::::::uus:::::ssss::::::si::::::io:::::ooooo:::::o  n::::n    n::::n3333333     3:::::3\n");
	klog("F::::::::FF            u:::::::::::::::us::::::::::::::s i::::::io:::::::::::::::o  n::::n    n::::n3::::::33333::::::3\n");
	klog("F::::::::FF             uu::::::::uu:::u s:::::::::::ss  i::::::i oo:::::::::::oo   n::::n    n::::n3:::::::::::::::33 \n");
	klog("FFFFFFFFFFF               uuuuuuuu  uuuu  sssssssssss    iiiiiiii   ooooooooooo     nnnnnn    nnnnnn 333333333333333   \n");
	klog("\n");
	klog("#######################################################################################################################\n");
}

int _main(void)
{
	ResolveDynlib();
	ascii();

	klog("Installing Kernel ELF\n");
	LoadKernel();

	// TODO: Add 7z extraction code.
	// TODO: Unpack and load the Daemon here.
	//		 I have added a driver definition for resolving symbols from embeded libs in one PRX.
	//		 We can use that to load the Daemon from here using Lnc.

	klog("All Done\n");

	return 0;
}