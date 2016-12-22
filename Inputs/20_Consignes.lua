-- Consigns

HLeve = MQTTinputDefLog('Heure levé', 'Majordome/HLever', TopicDate2Number, 8.00 )
HLeveOceane = MQTTinputDefLog('Heure levé Océane', 'Majordome/HLever/Oceane', TopicDate2Number, 7.00 )
HLeveJoris = MQTTinputDefLog('Heure levé Joris', 'Majordome/HLever/Joris', TopicDate2Number, 7.45 )
HCouche = MQTTinputDefLog('Heure couché', 'Majordome/HCoucher', TopicDate2Number, 21.30 )
