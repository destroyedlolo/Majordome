-- Consignes sur les heures de lever / coucher

HLever = MQTTinputDefLog('Heure lever', 'Majordome/HLever', TopicDate2Number, 8.05, 'N' )
HLeverOceane = MQTTinputDefLog('Heure lever Océane', 'Majordome/HLever/Oceane', TopicDate2Number, 7.00, 'N' )
HLeverJoris = MQTTinputDefLog('Heure lever Joris', 'Majordome/HLever/Joris', TopicDate2Number, 7.45, 'N' )
HCoucher = MQTTinputDefLog('Heure coucher', 'Majordome/HCoucher', TopicDate2Number, 21.30, 'N' )
