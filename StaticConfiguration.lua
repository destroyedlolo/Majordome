-- Static "hardcoded" configuration
--
-- 11/05/2016 - First version
--

-- Internal
VERBOSE=true

-- BROKERHOST='tcp://bPI.chez.moi:1883'
BROKERHOST='tcp://localhost:1883'
CLIENTID='Majordome-dev'

-- Topics to look for
MODE='Majordome/Mode'
MODEAUJOURDHUI='Majordome/Mode/AujourdHui'
MODEDEMAIN='Majordome/Mode/Demain'
MODEFORCE='Majordome/Mode/Force'
SAISON='Majordome/Saison'
HLEVE='Majordome/HLever'
HCOUCHE='Majordome/HCoucher'
TExterieur='maison/Temperature/Dehors'
TSalon='maison/Temperature/Salon'
TChOceane='maison/Temperature/Chambre Oceane'
METEO='Meteo/Nonglard'

-- Default configuration
SelShared.set( MODE, 'Manuel' )
SelShared.set( MODEFORCE, 'Auto' )
SelShared.set( SAISON, 'Intersaison' )
SelShared.set( HLEVE, 8.00 )	-- Ne doit pas être < a 1:00
SelShared.set( HCOUCHE, 21.10 )	-- pour les enfants

