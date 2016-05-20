-- Everything related to sun's light

-- Action

function FermeVoletRDC()
	SelLog.log("Fermeture des volets du Salon")

	Brk:Publish( 'maison/Volet/Salon/Balcon', 'Down' )
	Brk:Publish( 'maison/Volet/Salon/Fenetre', 'Down' )
	Brk:Publish( 'maison/Volet/Salon/Cheminee', 'Down' )
end

function OuvreVoletRDC()
	SelLog.log("Overture des volets du Salon")

	if SelShared.get( SAISON ) == 'Ete' then
		Brk:Publish( 'maison/Volet/Salon/Fenetre', 'Up' )
		Brk:Publish( 'maison/Volet/Salon/Balcon', 'My' )
		Brk:Publish( 'maison/Volet/Salon/Cheminee', 'My' )
	elseif SelShared.get( SAISON ) == 'Intersaison' then
		Brk:Publish( 'maison/Volet/Salon/Fenetre', 'Up' )
		Brk:Publish( 'maison/Volet/Salon/Balcon', 'Up' )
		Brk:Publish( 'maison/Volet/Salon/Cheminee', 'Up' )
	end
end

--
-- Couché de soleil
--
Sunset_tasks = { FermeVoletRDC }

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

	tmrRething( timerCron, tbl_timers );
end

table.insert( Topics, { topic = METEO .. '/sunset', trigger=hSunset, trigger_once=true } )


--
-- Levé de soleil
--

Sunrise_tasks = { OuvreVoletRDC }

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

	tmrRething( timerCron, tbl_timers );
end

table.insert( Topics, { topic = METEO .. '/sunrise', trigger=hSunrise, trigger_once=true } )

