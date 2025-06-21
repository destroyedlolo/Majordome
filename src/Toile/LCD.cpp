/* LCD Renderer
 * 	Embodies a low-level physical LCD textual device.
 */

#include "LCD.h"

#include <Selene/SelPlug-in/SelLCD/SelLCD.h>
#include <Selene/SelPlug-in/SelLCD/SelLCDScreen.h>

LCD::LCD( const std::string &fch, std::string &where, lua_State *L ) : Object(fch, where), LuaExec(fch, where), bus_number(1), address(0x27), twolines(false), y11(false){
	this->loadConfigurationFile(fch, where, L);

	if(d2)
		fd2 << this->getFullId() << ".class: LCD" << std::endl;
}

void LCD::readConfigDirective( std::string &l ){
	std::string arg;

	if(!(arg = striKWcmp( l, "-->> I2CBus=" )).empty()){
		this->bus_number = std::stoi(arg, nullptr, 0);
		if(::verbose)
			SelLog->Log('C', "\t\tBus number '0x%02x'", this->bus_number);
	} else if(!(arg = striKWcmp( l, "-->> I2CAddress=" )).empty()){
		this->address = std::stoi(arg, nullptr, 0);
		if(::verbose)
			SelLog->Log('C', "\t\tI2C address '0x%02x'", this->address);
	} else if(l == "-->> MultiLines"){
		this->twolines = true;
		if(::verbose)
			SelLog->Log('C', "\t\tThis screen has more than a line");
	} else if(l == "-->> 11px"){
		this->y11 = true;
		if(::verbose)
			SelLog->Log('C', "\t\tCharacters are 11 pixels hight");
	} else
		this->Renderer::readConfigDirective(l);
}

bool LCD::exec(){	/* From LuaExec::execSync() */
	if(debug)
		SelLog->Log('D', "LCD::exec()");

	if(!this->canRun())
		return false;

	lua_State *L = this->createLuaState();
	if(!L)
		return false;

	threadEnvironment(L);
	if(!this->feedbyNeeded(L)){
		lua_close(L);
		return false;
	}

	enum boolRetCode rc;
	this->execSync(L, &rc);	// no need to check the return, rc will be updated
	lua_close(L);

	if(rc == boolRetCode::RCfalse)	// Creation rejected
		return false;

	uint16_t verfound;
	struct SelLCD *SelLCD = (struct SelLCD *)SeleneCore->loadModule("SelLCD", SELLCD_VERSION, &verfound, 'F');
	if(!SelLCD)
		return false;

		/* Can be made by a simple cast,
		 * but these lines make the code more understandable
		 */
	SelLCDScreen *lcd = new SelLCDScreen;
	this->surface = &(lcd->primary.obj);

	if(!SelLCD->Init(lcd, this->bus_number, this->address, this->twolines, this->y11)){
		SelLog->Log('E', "Can't initialise the screen '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), lua_tostring(L, -1));
		return false;
	}

	SelLCD->Clear(lcd);
	SelLCD->Backlight(lcd, true);	// Backlight on
	SelLCD->DisplayCtl(lcd, true, false, false);	// On but no cursor

	if(debug)
		SelLog->Log('D', "LCD::exec() - Succeeded");

	return true;
}
