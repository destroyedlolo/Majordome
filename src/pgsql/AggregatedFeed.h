/* Add a data to a table from a *MinMax object
 */
#ifndef AGGREGATEDFEED_H
#define AGGREGATEDFEED_H

#include "Feed.h"
#include "../MinMax.h"
#include "../NamedMinMax.h"

class AggregatedFeed : virtual public Feed {
	MinMax *minmax;
	NamedMinMax *nminmax;

		// Which data to store
	enum _which {
		MIN, MAX, AVG, SUM
	} figure;

	virtual void feedState(lua_State *L);

		/* Executable
		 *
		 * CAUTION : if the data is rejected, the MinMax is not cleared
		 */
	virtual bool execAsync(lua_State *L);	// Overloading to handle data acceptation 

protected:
	void readConfigDirective( std::string &l );

public:
	AggregatedFeed(const std::string &, std::string &, lua_State *);

	virtual std::string getTri(){ return AggregatedFeed::trigramme(); }
	static std::string trigramme(){ return "AFD_"; }
};

typedef ObjCollection<AggregatedFeed *> AggregatedFeedCollection;
#endif
