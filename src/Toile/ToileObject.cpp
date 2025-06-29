#include "ToileObject.h"
#include "../Config.h"

ToileObject::ToileObject() : visible(true), parentR(NULL), parentP(NULL){
}

bool ToileObject::isVisible(void){
	bool ret = this->getOwnVisibility();
	if(!ret && ::debug && this->isVerbose())
		SelLog->Log('D', "[%s/%s] is not visible", this->getWhereC(), this->getNameC());

	return ret;
}

bool ToileObject::readConfigDirective(std::string &l){
	std::string arg;

#if 0	/* TODO */
	if(!(arg = striKWcmp( l, "-->> Renderer Parent=" )).empty()){
		if(this->parentR || this->parentP){
			SelLog->Log('F', "\t\tA Painting can't have multiple parents");
			exit(EXIT_FAILURE);
		}
	
			// Search the parent renderer
		RendererCollection::iterator renderer;
		if((renderer = config.RendererList.find(arg)) != config.RendererList.end()){
			if(::verbose)
#ifdef DEBUG
				SelLog->Log('C', "\t\tThe Parent is Renderer '%s' (%p)", arg.c_str(), &(renderer->second));
#else
				SelLog->Log('C', "\t\tThe Parent is Renderer '%s'", arg.c_str());
#endif
			this->parentR = renderer->second;
			renderer->second->addPainting( this );

			if(d2)
				fd2 << renderer->second->getFullId() << " <- " << this->getFullId() << ": Renderer Parent { class: llink }" << std::endl;
		} else {
			SelLog->Log('F', "\t\tRenderer '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if( l == "-->> hidden" ){
		if(::verbose)
			SelLog->Log('C', "\t\tHidden");
		this->visible = false;
	} else
		return false;
#endif

	return true;
}
