#include "ToileObject.h"
#include "../Config.h"

ToileObject::ToileObject() : visible(true), parent(NULL){
}

bool ToileObject::isVisible(void){
	bool ret = this->getOwnVisibility();
	if(!ret && ::debug && this->isVerbose())
		SelLog->Log('D', "[%s/%s] is not visible", this->getWhereC(), this->getNameC());

	return ret;
}

bool ToileObject::readConfigDirective(std::string &l){
	std::string arg;

	if(!(arg = striKWcmp( l, "-->> ApplyOn Renderer=" )).empty()){
		if(this->parent){
			SelLog->Log('F', "\t\tA Toile object can have only a single parent");
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

			this->parent = dynamic_cast<ToileContainer *>(renderer->second);

			if(this->getTri() == Decoration::trigramme()){
				puts("**** Decoration");
				renderer->second->addDecoration( static_cast<Decoration *>(this) );
			} else {
				puts("**** Painting");
#if 0	/* ToDo painting */
			renderer->second->addPainting( this );
#endif
			}

			if(d2)
				fd2 << renderer->second->getFullId() << " <- " << this->getFullId() << ": ApplyOn Renderer { class: llink }" << std::endl;
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

	return true;
}
