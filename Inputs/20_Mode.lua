-- Handles wakeup mode
--

-- Calendar determined modes
ModeAujourdHui = MQTTinputDefLog('Mode AujourdHui', 'Majordome/Mode/AujourdHui', nil, 'Manuel')
ModeDemain = MQTTinputDefLog('Mode Demain', 'Majordome/Mode/Demain', nil, 'Manuel')

-- Forced modes
ModeForce = MQTTinputDefLog('Mode Force', 'Majordome/Mode/Force', nil, 'Auto')
ModeEnfants = MQTTinputDefLog('Mode Force Enfants', 'Majordome/Mode/Enfants', nil, 'Auto')

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
		ModeAujourdHui.set('Manuel')
		-- 'Mode' will be updated by updModeAujourdhui()
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

