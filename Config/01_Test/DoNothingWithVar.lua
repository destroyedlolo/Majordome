-- This is an example of a task script using sharedvar
-- Have a look on DoNothing.lua to get explanation on header's content
--
-->> listen=NoStations

print("Variable content :", SelShared.Get("NoStations") )

SelShared.dump()
