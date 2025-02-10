-- Fermeture du volet du bureau
-->> when=FermetureVoletBureau
-->> waitfor=CoucherSoleilEffectif
--->> whenDone=CoucherSoleil
--
-->> need_timer=FermetureVoletBureau

--
-- Récupération des objets cibles
--

local CmdVoletBureau = MajordomeMQTTTopic.find("CmdVoletBureau", true)
local mode = SelSharedVar.Get("Mode") or "Manuel"
local saison = SelSharedVar.Get("Saison") or "Intersaison"

--
-- C'est parti
--

if mode == "Manuel" then
	SelLog.Log('I', "Mode \"Manuel\" : 'Fermeture' du volet du bureau ignoré")
else
	if MAJORDOME_TRACKER == "CoucherSoleil" and saison == "Hiver" then
		SelLog.Log('D', "Fermeture du volet du bureau ignoré (déja fermé car 'hiver')")
	else
		SelLog.Log('A', "Fermeture du volet du bureau")
		CmdVoletBureau:Publish("Down")
	end
end
