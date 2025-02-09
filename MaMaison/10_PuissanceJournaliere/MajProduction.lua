-- Reception des données de compteurs de consommation
-->> listen=TeleInfoProdCnt

if not SelSharedVar.Get("PRODCNT") then	-- Première valeur du jour
	SelSharedVar.Set("PRODCNT", SelSharedVar.Get("TeleInfoProdCnt") )
	SelLog.Log('T', "Init du compteur de conso HC : ".. SelSharedVar.Get("TeleInfoProdCnt"))
end

local pwr = 0

if SelSharedVar.Get("PRODCNT") and SelSharedVar.Get("TeleInfoProdCnt") then
	pwr = pwr + SelSharedVar.Get("TeleInfoProdCnt") - SelSharedVar.Get("PRODCNT")
end

SelLog.Log('T', "Puissance produite : ".. pwr)

local PubProdJournee = MajordomeMQTTTopic.find("PubProdJournee", true)
PubProdJournee:Publish(pwr)
