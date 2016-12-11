-- Generic class to handle input coming from MQTT

function MQTTinputs(aname, atpc, afunc, aonce)
	local self = {}

	-- Private fields
	local name = aname
	local tasks = {}

	-- methods
	function self.received()
		print( SelShared.get( name ) )
	end

	-- initialiser
print(self.received)
	table.insert( Topics, { topic=atpc, func=afunc, trigger=self.received, trigger_once=aonce } )

	return self
end

--
-- Helpers
--

function Topic2Number( t, v )
	SelShared.set(t, tonumber(v))
	return true
end

function Topic2NumberTTL( t, v, to )
-- to : ttl de la variable, en seconde
	SelShared.set(t, tonumber(v), to)
	return true
end

function TopicDate2Number( t, v )
	SelShared.set(t, TXT2DMS(v))
	return true
end

