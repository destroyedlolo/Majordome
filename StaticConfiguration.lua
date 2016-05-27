-- Static "hardcoded" configuration
--
-- 11/05/2016 - First version
--

-- Internal
VERBOSE=true

BROKERHOST='tcp://bPI.chez.moi:1883'
CLIENTID='Majordome-dev'

-- Topics to look for
MODE='Majordome/Mode'
MODEAUJOURDHUI='Majordome/Mode/AujourdHui'
MODEDEMAIN='Majordome/Mode/Demain'
MODEFORCE='Majordome/Mode/Force'
SAISON='Majordome/Saison'
HLEVER='Majordome/HLever'
HCOUCHER='Majordome/HCoucher'
TExterieur='maison/Temperature/Dehors'
TSalon='maison/Temperature/Salon'
METEO='Meteo/Nonglard'

-- Default configuration
SelShared.set( MODE, 'Manuel' )
SelShared.set( MODEFORCE, 'Auto' )
SelShared.set( SAISON, 'Intersaison' )
SelShared.set( HLEVER, 8.00 )	-- Ne doit pas être < a 1:00
SelShared.set( HCOUCHER, 21.10 )	-- pour les enfants

