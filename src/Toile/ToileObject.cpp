#include "ToileObject.h"
#include "../Config.h"

ToileObject::ToileObject() : visible(true), parent(NULL), surface(NULL){
}

bool ToileObject::isVisible(void){
	bool ret = this->getOwnVisibility();

		/* recursivity */
	for(auto p = this->getParent(); ret && p; p = p->getParent())
		ret &= this->getOwnVisibility();

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

			this->parent = dynamic_cast<ToileObject *>(renderer->second);

			if(this->getTri() == Decoration::trigramme())	/* Decorations are on separate list */
				renderer->second->addDecoration( static_cast<Decoration *>(this) );
			else
				renderer->second->addChild( this );

			if(d2)
				fd2 << renderer->second->getFullId() << " <- " << this->getFullId() << ": ApplyOn Renderer { class: llink }" << std::endl;
		} else {
			SelLog->Log('F', "\t\tRenderer '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!(arg = striKWcmp( l, "-->> ApplyOn Painting=" )).empty()){
		if(this->parent){
			SelLog->Log('F', "\t\tA Toile object can have only a single parent");
			exit(EXIT_FAILURE);
		}

			// Search the parent painting
		PaintingCollection::iterator paint;
		if((paint = config.PaintingList.find(arg)) != config.PaintingList.end()){
			if(::verbose)
#ifdef DEBUG
				SelLog->Log('C', "\t\tThe Parent is Painting '%s' (%p)", arg.c_str(), &(paint->second));
#else
				SelLog->Log('C', "\t\tThe Parent is Painting '%s'", arg.c_str());
#endif

			this->parent = dynamic_cast<ToileObject *>(paint->second);

			if(this->getTri() == Decoration::trigramme())	/* Decorations are on separate list */
				paint->second->addDecoration( static_cast<Decoration *>(this) );
			else
				paint->second->addChild( this );

			if(d2)
				fd2 << paint->second->getFullId() << " <- " << this->getFullId() << ": ApplyOn Painting { class: llink }" << std::endl;
		} else {
			SelLog->Log('F', "\t\tPainting '%s' is not (yet ?) defined", arg.c_str());
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

void ToileObject::assertSanity(void){
		/* Ensure a parent is defined */
	if(!this->getParent()){
		SelLog->Log('F', "[\"%s\"] No parent defined", this->getNameC());
		exit(EXIT_FAILURE);
	}
}
