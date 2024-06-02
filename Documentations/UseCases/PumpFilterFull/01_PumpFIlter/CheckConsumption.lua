-- Check if pump consumption is bellow 600w during at least 30 seconds
--
-->> listen=PumpConsumption

local col = assert( SelAverageCollection.Find("Pump") )

col:Push(MAJORDOME_PAYLOAD)

	-- Check if the collection is full
local isz, asz = col:GetSize()
local ihm, ahm = col:HowMany()

if ahm >= asz then
	local min, max = col:MinMaxA()

	if min < 600 then
		SelLog.Log("E", "Pump is full (".. min .."W)")
	end
end
