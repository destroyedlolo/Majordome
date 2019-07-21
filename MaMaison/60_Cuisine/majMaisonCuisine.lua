-- Activation des composants de la stratégie de régulation de la température
-- en fonction de la saison.
--
-->> listen=Saison

if not not SelShared.Get("Saison") then	-- ne devrait jamais arriver
	return
end

--
-- Récupération des composants
--

local FermetureTemperatureEteCuisine = MajordomeTimer.find("FermetureTemperatureEteCuisine", true)
local SurveillanceCuisine = MajordomeTracker.find("SurveillanceCuisine", true)

-- désactivation de tous les composants

FermetureTemperatureEteCuisine:Disable()
SurveillanceCuisine:Disable()

-- Activation en fonction de la surveillance

if SelShared.Get("Saison") == "Ete" then
	FermetureTemperatureEteCuisine:Enable()
elseif SelShared.Get("Saison") == "Intersaison" then
	SurveillanceCuisine:Enable()
end
