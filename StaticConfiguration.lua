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
TExterieur='maison/Temperature/Dehors'
METEO='Meteo/Nonglard'

-- Default configuration
SelShared.set( MODE, 'Manuel' )
SelShared.set( MODEFORCE, 'Auto' )
SelShared.set( SAISON, 'Intersaison' )
SelShared.set( HLEVER, '8:00' )

