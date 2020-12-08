-- Mise à jour de la consigne du mode
-->> listen=ModeForce

local Mode = MajordomeMQTTTopic.find("Mode", true)
local ModeForce = MajordomeMQTTTopic.find("ModeForce", true)

if ModeForce:getVal() == 'Auto' then
	SelLog.log('M', "Mode repassé à 'automatique'")
	-- le mode actif sera mis à jour automatiquement
elseif ModeForce:getVal() == 'Manuel' or ModeForce:getVal() == 'Absent' 
or ModeForce:getVal() == 'Vacances' or ModeForce:getVal() == 'Travail' then
	SelLog.log('M', "Mode forcé à '".. ModeForce:getVal() .."'")
	Mode:Publish( ModeForce:getVal() );
else
	SelLog.log('E', "Mode forcé non reconnu : '".. ModeForce:getVal() .."'")
	ModeForce:Publish("Manuel")
end
