/*
 * Majordome
 * 	An event based Automation System
 *
 * Copyright 2018-24 Laurent Faillie
 *
 * 		Majordome is covered by
 *		Creative Commons Attribution-NonCommercial 3.0 License
 *      (http://creativecommons.org/licenses/by-nc/3.0/) 
 *      Consequently, you're free to use if for personal or non-profit usage,
 *      professional or commercial usage REQUIRES a commercial licence.
 *  
 *      Majordome is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/* Basic modules needed by almost all applications */
#include <Selene/libSelene.h>	/* Modules : the only part hardly linked */
#include <Selene/SeleneCore.h>	/* Selene's core functionalities */
#include <Selene/SelLog.h>		/* Logging : not really mandatory but very useful in most of the cases */
#include <Selene/SelLua.h>
#include <Selene/SelScripting.h>

	/* Here start 'standard' C code */
#include <cstdlib>		/* exit(), ... */
#include <cstring>

#include <dlfcn.h>		/* dlerror(), ... */

int main(int ac, char **av){
		/*
		 * Load Selene's modules
		 */
	uint16_t verfound;

		// Selene's core
	struct SeleneCore *SeleneCore = (struct SeleneCore *)loadModule("SeleneCore", SELENECORE_VERSION, &verfound);
#ifdef DEBUG
	printf("*D* SeleneCore %s : version %u\n", SeleneCore ? "found":"not found", verfound);
#endif
	if(!SeleneCore){	/* Needs to do checks manually as SeleneCore is ... not loaded */
		printf("*F* : can't load SeleneCore ");
		if(verfound)
			printf("(%u instead of expected %u)\n", verfound, SELENECORE_VERSION);
		else {
			char *err = dlerror();
			if(!err)
				puts(" : missing InitModule() or newer SelModule expected");
			else
				printf("(%s)\n", dlerror());
		}

		exit(EXIT_FAILURE);
	}

	struct SelLog *SelLog = (struct SelLog *)SeleneCore->loadModule("SelLog", SELLOG_VERSION, &verfound, 'F');

		/* We still need to do it manually as SeleneCore->loadModule() still can't logging */
	if(!SelLog){
		printf("*F* : can't load SelLog ");
		if(verfound)
			printf("(%u instead of expected %u)\n", verfound, SELLOG_VERSION);
		else {
			char *err = dlerror();
			if(!err)
				puts(" : missing InitModule() or outdated dependency found");
			else
				printf("(%s)\n", dlerror());
		}

		exit(EXIT_FAILURE);
	} else if(!SeleneCore->SelLogInitialised(SelLog)){	// logging initiated
		SelLog->Log('F', "SelLog too old");
		exit(EXIT_FAILURE);
	}

	struct SelLua *SelLua = (struct SelLua *)SeleneCore->loadModule("SelLua", SELLUA_VERSION, &verfound, 'F');
	if(!SelLua)
		exit(EXIT_FAILURE);

	struct SelScripting *SelScripting = (struct SelScripting *)SeleneCore->loadModule("SelScripting", SELSCRIPTING_VERSION, &verfound, 'F');
	if(!SelScripting)
		exit(EXIT_FAILURE);
}
