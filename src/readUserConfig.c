/* Read user configuration directories */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

#include <libSelene.h>

#include "Components.h"
#include "sortdir.h"

static bool onlydir( const char *fch ){
	if(*fch == '.')
		return false;

	struct stat buff;
	if( stat( fch, &buff )<0 ){
		publishLog('E', "%s : %s", fch, strerror(errno));
		return false;
	}

	return( !!(buff.st_mode & S_IFDIR) );
}

static bool configfiles( const char *fch ){
	if( *fch == '.' )
		return false;
	
	const char *ext = fileextention( fch );
	if( !strcmp(ext,".lua") )
		return true;
	
	return false;
}

void readUserConfig( const char *dir, lua_State *L ){
	char cwd[PATH_MAX];		/* Keep current working directory */
	assert( getcwd(cwd, PATH_MAX) );
	if( chdir(dir) ){
		publishLog('F', "%s : %s", dir, strerror(errno));
		exit( EXIT_FAILURE );
	}

	unsigned int nbredir;
	char **userconfdir;
	if(!(userconfdir = sortdir( ".", &nbredir, onlydir ))){
		publishLog('F', "%s : %s", dir, strerror(errno));
		exit( EXIT_FAILURE );
	}

	for(unsigned int i=0; i<nbredir; i++){
		publishLog('L', "Loading '%s'", userconfdir[i]);
		if( chdir(userconfdir[i]) ){
			publishLog('F', "%s : %s", userconfdir[i], strerror(errno));
			exit( EXIT_FAILURE );
		}

		char **userconfdircontent;
		unsigned int nbrefch;
		if(!(userconfdircontent = sortdir( ".", &nbrefch, configfiles ))){
			publishLog('F', "%s : %s", userconfdir[i], strerror(errno));
			exit( EXIT_FAILURE );
		}

		for(unsigned int j=0; j<nbrefch; j++){
			if( !strcmp(userconfdircontent[j], "Init.lua") ){
				int err = luaL_loadfile(L, userconfdircontent[j]) || lua_pcall(L, 0, 0, 0);
				if(err){
					publishLog('F', "%s : %s", userconfdircontent[j], lua_tostring(L, -1));
					lua_pop(L, 1);  /* pop error message from the stack */
					exit(EXIT_FAILURE);
				}
			}
else
	printf("*d* ignoring %s\n", userconfdircontent[j] );
		}

		freedir( userconfdircontent, nbrefch );
		assert( !chdir("..") );
	}

	freedir( userconfdir, nbredir );

	if( chdir(cwd) ){	/* Back to the previous working directory */
		publishLog('F', "%s : %s", dir, strerror(errno));
		exit( EXIT_FAILURE );
	}
}
