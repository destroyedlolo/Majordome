-- Reset du répéteur WiFi
--
-->> when=11h
-->> when=CdGRepWiFi
-->> once

local commande = MajordomeMQTTTopic.find("CmdResetRepWiFi", true)

SelLog.Log('A', "Arrêt du répéteur WiFi")
commande:Publish("1")

Selene.Sleep( 15 )

SelLog.Log('A', "Démarrage du répéteur WiFi")
commande:Publish("0")

