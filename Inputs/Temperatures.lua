-- Home temperatures to follow

TExterieur = MQTTinputs('TExterieur', 'maison/Temperature/Dehors', Topic2Number)
TSalon = MQTTinputs('TSalon', 'maison/Temperature/Salon', Topic2Number)
TBureau = MQTTinputs('TBureau', 'maison/Temperature/Bureau', Topic2Number)

TChOceane = MQTTinputs('TChOceane', 'maison/Temperature/Chambre Oceane', Topic2Number)
TChJoris = MQTTinputs('TChJoris', 'maison/Temperature/Chambre Joris', Topic2Number)
TChParents = MQTTinputs('TChParents', 'maison/Temperature/Chambre Parents', Topic2Number)
