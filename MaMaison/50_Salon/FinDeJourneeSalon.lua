-- Fermeture des volets en fin de journée
-->> when=CoucherSoleil

local mode = SelShared.Get("Mode") or "Manuel"
local saison = SelShared.Get("Saison") or "Intersaison"

if mode == "Manuel" then
	SelLog.log('I', "Mode \"Manuel\" : 'Fermeture' du volet du salon ignoré")
else
	local action
	if saison == "Ete" then
		SelLog.log('A', "C'est l'été : les volets se ferment en \"My\"")
		action = "My"
	else
		SelLog.log('A', "'Fermeture' du volet du salon")
		action = "Down"
	end

	local CmdVoletSalonBalcon = MajordomeMQTTTopic.find("CmdVoletSalonBalcon", true)
	local CmdVoletSalonCheminee = MajordomeMQTTTopic.find("CmdVoletSalonCheminee", true)
	local CmdVoletSalonFenetre = MajordomeMQTTTopic.find("CmdVoletSalonFenetre", true)
	CmdVoletSalonBalcon:Publish(action)
	CmdVoletSalonCheminee:Publish(action)
	CmdVoletSalonFenetre:Publish(action)
end
