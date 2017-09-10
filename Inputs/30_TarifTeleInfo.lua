-- Retrieve Electricity rate from TéléInfo
local json = require "dkjson"

local function ITarifEDF(restpc, atpc)
-- restpc : Internal topic containing the current rate
-- atpc : root of the topic to listen to

	local tinit	-- forward definition
	local function initTarif()	-- Determine current rate based on historical summary
		local v = SelShared.get(atpc .. 'summary')
		local res, p, err = json.decode( v )
		if not res then 
			SelLog.log("*E* Can't decode ".. atpc .. 'summary')
			return
		end

		pubLog("Tarif récupéré")
		Brk:Publish( restpc, res.PTEC, true )
		tinit.TaskRemove( initTarif ) -- It has to be called only at init
	end

		-- /!\ Can't used func here as string comparison will fails (thread)
	tinit = MQTTinput( 'initTarif', atpc .. 'summary')
	tinit.TaskAdd(initTarif)

	local function rcvTI()
		Brk:Publish( restpc, SelShared.get(atpc .. 'values/PTEC'), true )
	end
	local self = MQTTinput( 'TInfoTarif', atpc .. 'values/PTEC')
	self.TaskAdd( rcvTI )

	return self
end

TarifEDF = ITarifEDF('Majordome/TarifEDF', 'TeleInfo/Consommation/')

