/*
 * LoggerMain.cpp
 *
 *  Created on: 22 дек. 2017 г.
 *      Author:
 */
#include <iostream>
#include <wtypes.h>
#include <winbase.h>

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD     fdwReason, LPVOID    lpvReserved) {
	BOOL res2 = TRUE;
	switch(fdwReason) {
		case DLL_PROCESS_ATTACH:
			std::cerr<<"Logger have been started successfully\n";
			break;
		case DLL_PROCESS_DETACH:

			std::cerr<<"Logger have been stopped successfully\n";
			break;
	}
	return res2;
}

