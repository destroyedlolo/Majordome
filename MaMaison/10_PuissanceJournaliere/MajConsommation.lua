-- Reception des données de compteurs de consommation
-->> listen=TeleInfoConsoCntHC
-->> listen=TeleInfoConsoCntHP

if MAJORDOME_TOPIC_NAME == "TeleInfoConsoCntHC" then
	if not SelSharedVar.Get("CONSOCNTHC") then	-- Première valeur du jour
		SelSharedVar.Set("CONSOCNTHC", SelSharedVar.Get("TeleInfoConsoCntHC") )
		SelLog.Log('T', "Init du compteur de conso HC : ".. SelSharedVar.Get("TeleInfoConsoCntHC"))
	end
else -- Heure plaine
	if not SelSharedVar.Get("CONSOCNTHP") then	-- Première valeur du jour
		SelSharedVar.Set("CONSOCNTHP", SelSharedVar.Get("TeleInfoConsoCntHP") )
		SelLog.Log('T', "Init du compteur de conso HP : ".. SelSharedVar.Get("TeleInfoConsoCntHP"))
	end
end

local pwr = 0
if SelSharedVar.Get("CONSOCNTHP") and SelSharedVar.Get("TeleInfoConsoCntHP") then
	pwr = pwr + SelSharedVar.Get("TeleInfoConsoCntHP") - SelSharedVar.Get("CONSOCNTHP")
end

if SelSharedVar.Get("CONSOCNTHC") and SelSharedVar.Get("TeleInfoConsoCntHC") then
	pwr = pwr + SelSharedVar.Get("TeleInfoConsoCntHC") - SelSharedVar.Get("CONSOCNTHC")
end

SelLog.Log('T', "Puissance consommée : ".. pwr)

local PubConsoJournee = MajordomeMQTTTopic.find("PubConsoJournee", true)
PubConsoJournee:Publish(pwr)
