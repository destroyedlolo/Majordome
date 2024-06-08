-- This is an example of a task script using SelSharedVar
-- Have a look on TimerClient.lua to get explanation on header's content
--
-->> listen=NoStations

print("Variable initial content :", SelSharedVar.Get("NoStations") )
SelSharedVar.dump()
