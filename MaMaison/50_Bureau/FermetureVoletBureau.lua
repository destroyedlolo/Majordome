-- Ouverture du volet du bureau
-->> when=FermetureVoletBureau
-->> when=CoucherSoleil

--
-- Récupération des objets cibles
--

local CmdVoletBureau = MajordomeMQTTTopic.find("CmdVoletBureau", true)
local mode = SelShared.Get("Mode") or "Manuel"
local saison = SelShared.Get("Saison") or "Intersaison"
local FermetureVoletBureau = MajordomeTimer.find("FermetureVoletBureau", true)

--
-- C'est parti
--

if mode == "Manuel" then
	SelLog.log('I', "Mode \"Manuel\" : 'Fermeture' du volet du bureau ignoré")
else
	if MAJORDOME_TIMER == "CoucherSoleil" and saison == "Hiver" then
		SelLog.log('D', "'Fermeture' du volet du bureau ignoré (déja fermé car 'hiver'")
	else
		SelLog.log('A', "'Fermeture' du volet du bureau")
		CmdVoletBureau:Publish("Down")
	end
end
