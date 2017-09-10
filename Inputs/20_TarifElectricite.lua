-- An internal topic to keep the value of the current electricity rate
-- This object allows others objects to be notified in case of change

local function ITarifElectricite( name, topic )
	local self = MQTTinput( name, topic )

	return self
end

TarifElectricite = ITarifElectricite( 'TarifElectricite', 'Majordome/TarifElectricite' )

