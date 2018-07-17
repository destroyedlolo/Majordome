/* sortdir.c
 * 	read and sort the content of a directory
 */

#include <dirent.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "sortdir.h"

struct direntry {
	struct direntry *next;
	char *file;
};

static bool igndot(const char *fch){
	return(*fch != '.');
}

static int compare (void const *a, void const *b){
	return strcmp (*(const char **)a, *(const char **)b);
}

char **sortdir( const char *where, unsigned int *nbre, bool accept_func(const char *) ){
	struct direntry *content = NULL;
	DIR *dir;
	struct dirent *p;
	char **result;

	if(!accept_func)
		accept_func = igndot;

	*nbre = 0;
	if( !(dir = opendir(where)) )
		return NULL;

	while((p = readdir(dir))){
		if( accept_func( p->d_name ) ){
			struct direntry *e = malloc( sizeof(struct direntry) );
			assert( e );
			e->next = content;
			assert( (e->file = strdup( p->d_name )) );
			content = e;
			(*nbre)++;
		}
	}

	closedir(dir);

	assert( (result = calloc( sizeof(char *), *nbre )) );
	for(unsigned int i=0; i<*nbre; i++){
		struct direntry *e = content;
		content = content->next;
		result[i] = e->file;
		free(e);
	}
	qsort(result, *nbre, sizeof(char *), compare);

	return result;
}

void freedir( char **dirtable, unsigned int nbre ){
	if(!dirtable)	/* Nothing to free */
		return;

	for(unsigned int i=0; i<nbre; i++){
		if( dirtable[i] ) free( dirtable[i] );
	}
	
	free( dirtable );
}
