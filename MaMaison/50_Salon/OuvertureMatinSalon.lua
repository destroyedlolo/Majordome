-- Ouverture des volets du salon le matin
-->> when=LeverSoleil

local mode = SelShared.Get("Mode") or "Manuel"
local saison = SelShared.Get("Saison") or "Intersaison"

if mode == "Manuel" then
	SelLog.log('I', "Mode \"Manuel\" : 'Ouverture' du volet du salon ignoré")
else
	if saison == "Ete" then
		SelLog.log('A', "C'est l'été : on laisse les volets du salon comme ils sont")
	else
		SelLog.log('A', "'Ouverture' du volet du salon")

		local CmdVoletSalonBalcon = MajordomeMQTTTopic.find("CmdVoletSalonBalcon", true)
		local CmdVoletSalonCheminee = MajordomeMQTTTopic.find("CmdVoletSalonCheminee", true)
		local CmdVoletSalonFenetre = MajordomeMQTTTopic.find("CmdVoletSalonFenetre", true)

		CmdVoletSalonBalcon:Publish("Up")
		CmdVoletSalonCheminee:Publish("Up")
		CmdVoletSalonFenetre:Publish("Up")
	end
end
