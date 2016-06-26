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

function SCouche()
	SelLog.log("Le soleil se couche" )
	SubTasks( Tasks['SCouche'] )
end

function chgSCouche()
	local t = SelShared.get( TSunSet )
	SelLog.log("Couché du soleil = " .. t)

	tmrRemoveEntry(tbl_timers, SCouche)
	tmrAddEntry( tbl_timers, t, SCouche)

	SelShared.PushTask( rethingTimerCron, SelShared.TaskOnceConst("LAST"))
end

Tasks['SCouche']={}
table.insert( Topics, { topic = TSunSet, func=TopicDate2Number, trigger=chgSCouche, trigger_once=true } )

