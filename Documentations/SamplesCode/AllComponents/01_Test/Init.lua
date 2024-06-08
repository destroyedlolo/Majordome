-- Initialise some stuffs for other modules.
-- Notez-bien : it is very very not recommended to use Selene.Sleep()
-- (or any SelScripting methods) in Majordome tasks.

Selene.exposeAdminAPI()	-- Use is part of Admin API
Selene.Use("SelScripting")	-- Selene.Sleep() part of this module

-- Test code

SelLog.Log( 'I', "Init : I'm on way to set interesting global stuffs" )

MQTTBroker:Publish( MAJORDOME_ClientID .. "/Test", "Successful publish by " .. SELENE_SCRIPT_NAME .. " from " .. SELENE_SCRIPT_DIR)
