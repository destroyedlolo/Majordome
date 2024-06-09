SelLog.Log( 'I', "Init : I'm on way to set interesting global stuffs" )

MQTTBroker:Publish( MAJORDOME_ClientID .. "/Test", "Successful publish by " .. SELENE_SCRIPT_NAME .. " from " .. SELENE_SCRIPT_DIR)
