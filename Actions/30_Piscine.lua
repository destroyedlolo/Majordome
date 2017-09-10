-- Pool pump

function APiscine(
	aname, atpc	-- Pump IO topic
)
	local self = MQTTdata(aname, atpc)

	local function Action()
		if not SelShared.get( ModePiscine.getTopic() ) then -- security only, should not happen
			return
		elseif SelShared.get( ModePiscine.getTopic() ) == 'Forcé' then
			pubLog(self.getName() .. " : Forcé", 'Action')
			Brk:Publish(atpc, 1)
		elseif SelShared.get( ModePiscine.getTopic() ) == 'Arret' then
			pubLog(self.getName() .. " : Arrêté", 'Action')
			Brk:Publish(atpc, 0)
		else -- 'Heure Creuse'
			if not SelShared.get( TarifElectricite.getTopic() ) then
				pubLog(self.getName() .. " : Tarif électricité non renseigné", 'Erreur')
			elseif SelShared.get( TarifElectricite.getTopic() ) == 'HC..' then
				pubLog(self.getName() .. " : Pompe activée", 'Action')
				Brk:Publish(atpc, 1)
			else
				pubLog(self.getName() .. " : Pompe stoppée", 'Action')
				Brk:Publish(atpc, 0)
			end
		end
	end

	ModePiscine.TaskOnceAdd( Action )
	TarifElectricite.TaskOnceAdd( Action )

	return self
end

Piscine = APiscine( 'Piscine', 'maison/IO/Pompe_Piscine' )
