-- Reception des données de compteurs de consommation
-->> listen=TeleInfoConsoCntHC
-->> listen=TeleInfoConsoCntHP

if MAJORDOME_TOPIC_NAME == "TeleInfoConsoCntHC" then
	if not SelShared.Get("CONSOCNTHC") then	-- Première valeur du jour
		SelShared.Set("CONSOCNTHC", SelShared.Get("TeleInfoConsoCntHC") )
		SelLog.log('T', "Init du compteur de conso HC : ".. SelShared.Get("TeleInfoConsoCntHC"))
	end
else -- Heure plaine
	if not SelShared.Get("CONSOCNTHP") then	-- Première valeur du jour
		SelShared.Set("CONSOCNTHP", SelShared.Get("TeleInfoConsoCntHP") )
		SelLog.log('T', "Init du compteur de conso HP : ".. SelShared.Get("TeleInfoConsoCntHP"))
	end
end

local pwr = 0
if SelShared.Get("CONSOCNTHP") and SelShared.Get("TeleInfoConsoCntHP") then
	pwr = pwr + SelShared.Get("TeleInfoConsoCntHP") - SelShared.Get("CONSOCNTHP")
end

if SelShared.Get("CONSOCNTHC") and SelShared.Get("TeleInfoConsoCntHC") then
	pwr = pwr + SelShared.Get("TeleInfoConsoCntHC") - SelShared.Get("CONSOCNTHC")
end

SelLog.log('T', "Puissance consommée : ".. pwr)

local PubConsoJournee = MajordomeMQTTTopic.find("PubConsoJournee", true)
PubConsoJournee:Publish(pwr)
