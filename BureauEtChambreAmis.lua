-- Office and guest room

--
-- Topics
--

local TBuro='maison/Volet/Buro'
local TChAmis='maison/Volet/chAmis'

--
-- Actions
--

function OuvreBuro()
	VoletOuvre(TBuro, 'Bureau')
end

function FermeBuro()
	VoletFerme(TBuro, 'Bureau')
end

function MyBuro()
	VoletMy(TBuro, 'Bureau')
end

function OuvreChAmis()
	VoletOuvre(TChAmis, 'Chambre Amis')
end

function FermeChAmis()
	VoletFerme(TChAmis, 'Chambre Amis')
end

function MyChAmis()
	VoletMy(TChAmis, 'Chambre Amis')
end

--
-- Triggers
--

function determinePlanningRdC()
	SelLog.log("Détermination du planning pour le rez de chaussée")

	if SelShared.get( SAISON ) == 'Hiver' then
	else
		-- Ménage
		tmrRemoveEntry(tbl_timers, OuvreBuro)
		tmrRemoveEntry(tbl_timers, FermeBuro)
		tmrRemoveEntry(tbl_timers, MyBuro)
		tmrRemoveEntry(tbl_timers, OuvreChAmis)
		tmrRemoveEntry(tbl_timers, FermeChAmis)
		tmrRemoveEntry(tbl_timers, MyChAmis)

		if SelShared.get(MODE) == 'Travail' then
			h = DEC2DMS(DMS2DEC(SelShared.get( HLEVE )) - DMS2DEC(0.15))
			tmrAddEntry( tbl_timers, h, OuvreBuro)
			tmrAddEntry( tbl_timers, h, OuvreChAmis)
		end
	end

	SelShared.PushTask( rethingTimerCron, SelShared.TaskOnceConst("LAST"))
end
