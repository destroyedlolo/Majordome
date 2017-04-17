-- Consignes sur les heures de lever / coucher

HLeve = MQTTinputDefLog('Heure lever', 'Majordome/HLever', TopicDate2Number, 8.05, true )
HLeveOceane = MQTTinputDefLog('Heure lever Océane', 'Majordome/HLever/Oceane', TopicDate2Number, 7.00, true )
HLeveJoris = MQTTinputDefLog('Heure lever Joris', 'Majordome/HLever/Joris', TopicDate2Number, 7.45, true )
HCouche = MQTTinputDefLog('Heure coucher', 'Majordome/HCoucher', TopicDate2Number, 21.30, true )
