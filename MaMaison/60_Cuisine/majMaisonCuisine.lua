-- Activation des composants de la stratégie de régulation de la température
-- en fonction de la saison.
--
-->> listen=Saison
-->> require_topic=Saison
-->> need_timer=FermetureTemperatureEteCuisine
-->> need_tracker=SurveillanceCuisine

-- désactivation de tous les composants

FermetureTemperatureEteCuisine:Disable()
SurveillanceCuisine:Disable()

-- Activation en fonction de la surveillance

if Saison:getVal() == "Ete" then
	FermetureTemperatureEteCuisine:Enable()
elseif Saison:getVal() == "Intersaison" then
	SurveillanceCuisine:Enable()
end
