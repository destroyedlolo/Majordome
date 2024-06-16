-- Fermeture des volets en fin de journée
-->> waitfor=CoucherSoleilEffectif
--->> whenDone=CoucherSoleil
--
-->> require_topic=Mode
-->> require_topic=Saison

if Mode:getVal() == "Manuel" then
	SelLog.Log('I', "Mode \"Manuel\" : 'Fermeture' du volet du salon ignoré")
else
	local action
	if Saison:getVal() == "Ete" then
		SelLog.Log('A', "C'est l'été : les volets du salon se ferment en \"My\"")
		action = "My"
	else
		SelLog.Log('A', "Fermeture du volet du salon")
		action = "Down"
	end

	local CmdVoletSalonBalcon = MajordomeMQTTTopic.find("CmdVoletSalonBalcon", true)
	local CmdVoletSalonCheminee = MajordomeMQTTTopic.find("CmdVoletSalonCheminee", true)
	local CmdVoletSalonFenetre = MajordomeMQTTTopic.find("CmdVoletSalonFenetre", true)
	CmdVoletSalonBalcon:Publish(action)
	CmdVoletSalonCheminee:Publish(action)
	CmdVoletSalonFenetre:Publish(action)
end
