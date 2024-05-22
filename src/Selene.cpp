/* Selene.c
 *
 *	Interface with Selene
 *
 *	Copyright 2018-24 Laurent Faillie
 *
 */

#include "Selene.h"

struct SeleneCore *SeleneCore;
struct SelLog *SelLog;
struct SelLua *SelLua;
struct SelMQTT *SelMQTT;

	/* Here start 'standard' C code */
#include <cstdlib>		/* exit(), ... */
#include <cstring>

#include <dlfcn.h>		/* dlerror(), ... */

void initSelene(void){
	uint16_t verfound;

		/*
		 * Load Selene's modules
		 */
		// Selene's core
	 SeleneCore = (struct SeleneCore *)loadModule("SeleneCore", SELENECORE_VERSION, &verfound);
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

	SelLog = (struct SelLog *)SeleneCore->loadModule("SelLog", SELLOG_VERSION, &verfound, 'F');

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

	SelLua = (struct SelLua *)SeleneCore->loadModule("SelLua", SELLUA_VERSION, &verfound, 'F');
	if(!SelLua)
		exit(EXIT_FAILURE);

	SelMQTT = (struct SelMQTT *)SeleneCore->loadModule("SelMQTT", SELMQTT_VERSION, &verfound, 'F');
	if(!SelMQTT)
		exit(EXIT_FAILURE);
}
