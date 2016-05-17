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
MODEDEMAIN='Majordome/Mode/Demain'
SAISON='Majordome/Saison'
HLEVER='Majordome/HLever'
TExterieur='maison/Temperature/Dehors'

-- Default configuration
SelShared.set( MODE, 'Manuel' )
SelShared.set( SAISON, 'Intersaison' )
SelShared.set( HLEVER, '8:00' )

