-- Check if pump consumption is bellow 600w during at least 30 seconds
--
-->> listen=PumpConsumption

local col = assert( SelAverageCollection.Find("Pump") )

col:Push(MAJORDOME_PAYLOAD)

col:dump()
