SelLog.log( 'I', "I'm on way to set interesting global stuffs" )

SelLog.init("/tmp/".. MAJORDOME_ClientID ..".log", true)
SelLog.log("Starting to log to file as well")

SeleMQTT.Publish( MAJORDOME_ClientID .. "/Test", "Successful publish" )
