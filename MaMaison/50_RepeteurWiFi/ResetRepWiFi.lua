-- Reset du répéteur WiFi
--
-->> until=11h
-->> until=CdGRepWiFi
-->> once

local commande = MajordomeMQTTTopic.find("CommandeResetRepWiFi")
assert( commande, 'Topic "CommandeResetRepWiFi" pas trouvé' )

SelLog.log('A', "Arrêt du répéteur WiFi")
commande:Publish("1")

Selene.Sleep( 15 )

SelLog.log('A', "Démarrage du répéteur WiFi")
commande:Publish("0")

