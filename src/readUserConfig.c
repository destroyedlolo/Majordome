/* Read user configuration directories */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

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

void readUserConfig( const char *dir ){
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
printf("*d* %u entries\n", nbredir);
for(unsigned int i=0; i<nbredir; i++) puts( userconfdir[i] );

	freedir( userconfdir, nbredir );

	if( chdir(cwd) ){	/* Back to the previous working directory */
		publishLog('F', "%s : %s", dir, strerror(errno));
		exit( EXIT_FAILURE );
	}
}
