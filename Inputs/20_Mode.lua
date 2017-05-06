-- Handles wakeup mode
--

-- Calendar determined modes
ModeAujourdHui = MQTTinputDefLog('Mode AujourdHui', 'Majordome/Mode/AujourdHui', nil, 'Manuel', true)
ModeDemain = MQTTinputDefLog('Mode Demain', 'Majordome/Mode/Demain', nil, 'Manuel', true)

-- Forced modes
ModeForce = MQTTinputDefLog('Mode Force', 'Majordome/Mode/Force', nil, 'Auto', true)
ModeFEnfants = MQTTinputDefLog('Mode Force Enfants', 'Majordome/Mode/Force/Enfants', nil, 'Auto', true)
ModeFParents = MQTTinputDefLog('Mode Force Parents', 'Majordome/Mode/Force/Parents', nil, 'Auto', true)
ModeFOceane = MQTTinputDefLog('Mode Force Océane', 'Majordome/Mode/Force/Enfants/Oceane', nil, 'Auto', true)
ModeFJoris = MQTTinputDefLog('Mode Force Joris', 'Majordome/Mode/Force/Enfants/Joris', nil, 'Auto', true)

-- Actual mode
Mode = MQTTinputDefLog('Mode', 'Majordome/Mode', nil, 'Manuel')

--
-- Actions
--

local function updModeAujourdhui()
-- Update actual mode against computed one
	if ModeForce.get() == 'Auto' then
		SelLog.log("Mode calculé")
		Brk:Publish( Mode.getTopic(), ModeAujourdHui.get() )
	end
end
ModeAujourdHui.TaskAdd(updModeAujourdhui)

local function updModeDemain()
-- Tomorrow mode
	if ModeForce.get() ~= 'Auto' then
		SelLog.log('"Mode demain" forcé à ' .. ModeForce.get())
		ModeDemain.set( ModeForce.get() )
	end
end
ModeDemain.TaskAdd(updModeDemain)

local function updModeForce()
-- Forced mode update
	if ModeForce.get() == 'Auto' then
		SelLog.log('Mode repassé à automatique')
		updModeAujourdhui()	-- Rethink actuel mode
	elseif ModeForce.get() == 'Manuel' or ModeForce.get() == 'Absent' 
	or ModeForce.get() == 'Vacances' or ModeForce.get() == 'Travail' then
		SelLog.log('Mode forcé')
		Brk:Publish( Mode.getTopic(), ModeForce.get() )
		-- 'Mode Demain' will be updated by updModeDemain()
	else
		SelLog.log("Mode forcé '".. ModeForce.get() .."' non reconnu")
		Brk:Publish( Mode.getTopic(), 'Manuel' )
		ModeForce.set( 'Manuel' )
		-- 'Mode Demain' will be updated by updModeDemain()
	end
end
ModeForce.TaskAdd(updModeForce)

-- Modes depend on forced one
ModeForce.TaskAdd(updModeAujourdhui)
ModeForce.TaskAdd(updModeDemain)

