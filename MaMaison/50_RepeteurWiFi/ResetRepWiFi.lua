-- Reset du répéteur WiFi
--
-->> when=11h
-->> when=CdGRepWiFi
-->> once

local commande = MajordomeMQTTTopic.find("CommandeResetRepWiFi", true)

SelLog.log('A', "Arrêt du répéteur WiFi")
commande:Publish("1")

Selene.Sleep( 15 )

SelLog.log('A', "Démarrage du répéteur WiFi")
commande:Publish("0")

