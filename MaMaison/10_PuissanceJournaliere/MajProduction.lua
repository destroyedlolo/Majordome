-- Reception des données de compteurs de consommation
-->> listen=TeleInfoProdCnt

if not SelShared.Get("PRODCNT") then	-- Première valeur du jour
	SelShared.Set("PRODCNT", SelShared.Get("TeleInfoProdCnt") )
	SelLog.log('T', "Init du compteur de conso HC : ".. SelShared.Get("TeleInfoProdCnt"))
end

local pwr = 0

if SelShared.Get("PRODCNT") and SelShared.Get("TeleInfoProdCnt") then
	pwr = pwr + SelShared.Get("TeleInfoProdCnt") - SelShared.Get("PRODCNT")
end

SelLog.log('T', "Puissance produite : ".. pwr)

local PubProdJournee = MajordomeMQTTTopic.find("PubProdJournee", true)
PubProdJournee:Publish(pwr)
