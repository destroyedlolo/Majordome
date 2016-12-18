-- Solar production

local function ITeleInfo(aname, atpc)
	local function rcvTProd(t, v)
		v = tonumber(v)
		if v ~= 0 then	-- Reset the TTL only if there is some production
			Topic2NumberTTL( t,v, 60 )	-- after 1 minutes, we estimate the production is over
		end
		return true
	end

	local self = MQTTinputs( aname, atpc, rcvTProd )

	return self
end

TProdElectrique = ITeleInfo('TeleInfoProd', 'TeleInfo/Production/values/PAPP')
