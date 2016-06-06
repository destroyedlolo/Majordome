-- Everything related to sun's light

--
-- Couché de soleil
--
Sunset_tasks = { }

function SunSet()
	SelLog.log("Le soleil se couche" )
	SubTasks( Sunset_tasks )
end

function hSunset()
	local t = TXT2DMS( SelShared.get(METEO .. '/sunset') )
	SelLog.log("Couché du soleil = " .. t)

	tmrRemoveEntry(tbl_timers, SunSet )
	if not tbl_timers[t] then
		tbl_timers[t] = { SunSet }
	else
		table.insert( tbl_timers[t], SunSet )
	end

	tmrRethink( timerCron, tbl_timers );
end

table.insert( Topics, { topic = METEO .. '/sunset', trigger=hSunset, trigger_once=true } )


--
-- Levé de soleil
--

Sunrise_tasks = { OuvreVoletSalon }

function SunRise()
	SubTasks( Sunrise_tasks )
	SelLog.log("Le soleil se lève" )
end

function hSunrise()
	local t = TXT2DMS( SelShared.get(METEO .. '/sunrise') )
	SelLog.log("Levé du soleil = " .. t)

	tmrAddEntry( tbl_timers, t, SunRise )
	tmrRethink( timerCron, tbl_timers );
end

table.insert( Topics, { topic = METEO .. '/sunrise', trigger=hSunrise, trigger_once=true } )

