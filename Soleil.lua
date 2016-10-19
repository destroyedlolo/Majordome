-- Sun's

--
-- Topics
--

TMETEO='Meteo/Nonglard'
TSunSet=TMETEO .. '/sunset'
TSunRise=TMETEO .. '/sunrise'

--
-- Actions
--

-- Levé du soleil

function SLeve()
	SelLog.log("Le soleil se lève" )
	SubTasks( Tasks['SLeve'] )
end

function chgSLeve()
	local t = SelShared.get( TSunRise )
	SelLog.log("Levé du soleil = " .. t)

	tmrRemoveEntry(tbl_timers, SLeve)
	tmrAddEntry( tbl_timers, t, SLeve)

	SelShared.PushTask( rethingTimerCron, SelShared.TaskOnceConst("LAST"))
end

Tasks['SLeve']={}
table.insert( Topics, { topic = TSunRise, func=TopicDate2Number, trigger=chgSLeve, trigger_once=true } )


-- Couché du soleil

-- Afin de s'assurer que le vollet se ferment réelement lorsqu'il n'y a plus de soleil
-- nous nous baserons sur la production photovoltaique.

function SCouche()	-- Le soleil se couche effectivement.
	SelLog.log("Le soleil se couche" )
	SubTasks( Tasks['SCouche'] )
end

function SurveillanceCoucheSoleil()
	tmrRemoveEntry(tbl_timers, SurveillanceCoucheSoleil)

	if SelShared.get(TProdElectrique) == nil then	-- Pas de production electrique depuis un moment
		SCouche()
	else
		local dt = os.date("*t")
		local cur = dt.hour + dt.min/100
		local t = DEC2DMS(DMS2DEC(cur) + DMS2DEC(0.01))	-- On relance la fonction dans 2'

		SelLog.log("Le soleil brille toujours : nouveau test à ".. t)
		tmrAddEntry( tbl_timers, t, SurveillanceCoucheSoleil)
	end
	
	SelShared.PushTask( rethingTimerCron, SelShared.TaskOnceConst("LAST"))	-- Recalcul de la prochaine tache
end

function chgSCouche()
	local t = SelShared.get( TSunSet )
	SelLog.log("Début surveillance du couché du soleil = " .. t)

	tmrRemoveEntry(tbl_timers, SurveillanceCoucheSoleil)
	tmrAddEntry( tbl_timers, t, SurveillanceCoucheSoleil)

	SelShared.PushTask( rethingTimerCron, SelShared.TaskOnceConst("LAST"))
end

Tasks['SCouche']={}
table.insert( Topics, { topic = TSunSet, func=TopicDate2Number, trigger=chgSCouche, trigger_once=true } )

