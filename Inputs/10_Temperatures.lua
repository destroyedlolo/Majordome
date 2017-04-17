-- Température à suivre

TExterieur = MQTTinput('TExterieur', 'maison/Temperature/Dehors', Topic2Number)
TSalon = MQTTinput('TSalon', 'maison/Temperature/Salon', Topic2Number)
TBureau = MQTTinput('TBureau', 'maison/Temperature/Bureau', Topic2Number)

TChOceane = MQTTinput('TChOceane', 'maison/Temperature/Chambre Oceane', Topic2Number)
TChJoris = MQTTinput('TChJoris', 'maison/Temperature/Chambre Joris', Topic2Number)
TChParents = MQTTinput('TChParents', 'maison/Temperature/Chambre Parents', Topic2Number)
