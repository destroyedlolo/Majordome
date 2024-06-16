-- Ouverture des volets du salon le matin
-->> when=LeverSoleil
-->> require_topic=Mode
-->> require_topic=Saison

if Mode:getVal() == "Manuel" then
	SelLog.Log('I', "Mode \"Manuel\" : 'Ouverture' du volet du salon ignoré")
else
	SelLog.Log('A', "Ouverture du volet du salon/Fenetre")
	local CmdVoletSalonFenetre = MajordomeMQTTTopic.find("CmdVoletSalonFenetre", true)
	CmdVoletSalonFenetre:Publish("Up")

	if Saison:getVal() == "Ete" then
		SelLog.Log('I', "C'est l'été : on laisse les volets du salon comme ils sont")
	else
		SelLog.Log('A', "Ouverture du volet du salon")

		local CmdVoletSalonBalcon = MajordomeMQTTTopic.find("CmdVoletSalonBalcon", true)
		local CmdVoletSalonCheminee = MajordomeMQTTTopic.find("CmdVoletSalonCheminee", true)

		CmdVoletSalonBalcon:Publish("Up")
		CmdVoletSalonCheminee:Publish("Up")
		CmdVoletSalonFenetre:Publish("Up")
	end
end
