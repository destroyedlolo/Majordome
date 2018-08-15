-- This is an example of a task script using sharedvar
-- Have a look on DoNothing.lua to get explanation on header's content
--
-->> listen=NoStations

print("Variable initial content :", SelShared.Get("NoStations") )
SelShared.dump()

Selene.Sleep( 2 )

print("Variable final content :", SelShared.Get("NoStations") )
SelShared.dump()
