/* Archiving data
 */
#ifndef ARCHIVING_H
#define ARCHIVING_H

#include "mpgConnection.h"
#include "Purge.h"
#include "../Object.h"
#include "../Handler.h"
#include "../ObjCollection.h"
#include "../StringVector.h"
#include "../Event.h"

class Archiving : virtual public Purge {
protected:
	virtual void readConfigDirective( std::string &l );

	std::string SourceName;
	std::string SourceField;
	std::string Aggregation;

		/* Archiving kind */
	enum _kind {
		MINMAX,	// Min, Max and average
		SUM,	// Sumarize
		DELTA,	// Delta between the first and the last value
		MMA2	// From MMA to MMA
	} kind;

	StringVector keys;

	bool internalExec(void);
public:
	Archiving(const std::string &fch, std::string &where);

	/* Accessors */
	bool hasSource(void){ return !this->SourceName.empty(); };

	virtual std::string getTri(){ return "ARC_"; }
};

typedef ObjCollection<Archiving *> ArchivingCollection;
#endif
