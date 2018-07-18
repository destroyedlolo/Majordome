/* sortdir.h
 * 	read and sort the content of a directory
 */
#ifndef SORTDIR_H
#define SORTDIR_H

#include <stdbool.h>

/* list and sort a directory content 
 * -> directory : directory to list
 * -> how_many : number of entry
 * -> accept_function() : acceptance function (true if accepted)
 *  	If NULL, ignore item starting by '.' as well as .md files
 * <- table of content or NULL in case of error (see errno)
 */
extern char **sortdir( const char *directory, unsigned int *how_many, bool accept_func(const char *) );

/* Free result of a sortdir() call
 * -> dirtable : result to free
 * -> how_many : number of entries in the table
 */
extern void freedir( char **dirtable, unsigned int how_many );

/* Return the extention of a given file
 * -> filename
 * <- pointer to the last '.' or NULL if not found
 */
extern const char *fileextention( const char *fch );

#endif
