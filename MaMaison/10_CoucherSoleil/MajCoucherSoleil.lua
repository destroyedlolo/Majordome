-- Mise à jour du timer de la consigne "ConsigneCoucherSoleil" en fonction
-- des infos reçues de la météo.
--
-->> listen=CoucherSoleil
-->> listen=Saison

if not SelShared.Get('CoucherSoleil') then
	return
end

local timer = MajordomeTimer.find("ConsigneCoucherSoleil", true)

local h,m = string.match(string.gsub( SelShared.Get('CoucherSoleil'), '%.', ':'), "(%d+):(%d+)")

SelLog.log('I', "Le soleil se couche à ".. h ..":".. m)

local saison = SelShared.Get("Saison") or "Hiver"
if saison == "Hiver" then
	timer:setAtHM( h, m-15 )
	h,m = timer:getAtHM()
	SelLog.log('I', "Début de la surveillance solaire à ".. h ..":".. m)
else
	timer:setAtHM( h, m )
end

local trace = MajordomeMQTTTopic.find("TraceSuiviCoucherSoleil", true)
trace:Publish("D;" .. h ..":".. m, true)

local rdv = MajordomeRendezVous.find("CoucherSoleilChange", true)
rdv:Launch()
