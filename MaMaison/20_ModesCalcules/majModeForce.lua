-- Mise à jour de la consigne du mode
-->> listen=ModeForce
--
-->> need_topic=Mode
-->> need_topic=ModeForce

if ModeForce:getVal() == 'Auto' then
	SelLog.Log('M', "Mode repassé à 'automatique'")
	-- le mode actif sera mis à jour automatiquement
elseif ModeForce:getVal() == 'Manuel' or ModeForce:getVal() == 'Absent' 
or ModeForce:getVal() == 'Vacances' or ModeForce:getVal() == 'Travail' then
	SelLog.Log('M', "Mode forcé à '".. ModeForce:getVal() .."'")
	Mode:Publish( ModeForce:getVal() );
else
	SelLog.Log('E', "Mode forcé non reconnu : '".. ModeForce:getVal() .."'")
	ModeForce:Publish("Manuel")
end
