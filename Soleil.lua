-- Everything related to sun's light

--
-- Couché de soleil
--
Sunset_tasks = { FermeVoletRDC }

function SunSet()
	SubTasks( Sunset_tasks )
	SelLog.log("Le soleil se couche" )
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

print(universal_tostring( tbl_timers ))
end

table.insert( Topics, { topic = METEO .. '/sunset', trigger=hSunset, trigger_once=true } )


--
-- Levé de soleil
--

Sunset_tasks = { OuvreVoletRDC }

function SunRise()
	SubTasks( Sunrise_tasks )
	SelLog.log("Le soleil se lève" )
end

function hSunrise()
	local t = TXT2DMS( SelShared.get(METEO .. '/sunrise') )
	SelLog.log("Levé du soleil = " .. t)

	tmrRemoveEntry(tbl_timers, SunRise )
	if not tbl_timers[t] then
		tbl_timers[t] = { SunRise }
	else
		table.insert( tbl_timers[t], SunRise )
	end

print(universal_tostring( tbl_timers ))
end

table.insert( Topics, { topic = METEO .. '/sunrise', trigger=hSunrise, trigger_once=true } )

-- Action

function FermeVoletRDC()
	Brk:Publish( 'maison/Volet/Salon/Balcon', 'Down' )
	Brk:Publish( 'maison/Volet/Salon/Fenetre', 'Down' )
	Brk:Publish( 'maison/Volet/Salon/Cheminee', 'Down' )
end

function OuvreVoletRDC()
	Brk:Publish( 'maison/Volet/Salon/Balcon', 'Up' )
	Brk:Publish( 'maison/Volet/Salon/Fenetre', 'Up' )
	Brk:Publish( 'maison/Volet/Salon/Cheminee', 'Up' )
end
