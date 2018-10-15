-- Ferme la chambre d'amis si le mode est "Absent"
-->> listen=Mode

if SelShared.Get("Mode") == 'Absent' then
	local VoletChAmis = MajordomeMQTTTopic.find("CmdVoletChAmis", true)

	VoletChAmis:Publish("Down")
	SelLog.log('I', "Le volet de la chambre d'amis se ferme")
end
