SelLog.log( 'I', "I'm on way to set interesting global stuffs" )

SeleMQTT.Publish( MAJORDOME_ClientID .. "/Test", "Successful publish" )

print("Running " .. SELENE_SCRIPT_NAME .. " from " .. SELENE_SCRIPT_DIR);
