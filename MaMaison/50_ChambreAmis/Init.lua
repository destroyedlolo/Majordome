-- Initialise some stuffs for other modules.
-- Notez-bien : it is very very not recommended to use Selene.Sleep()
-- (or any SelScripting methods) in Majordome tasks.
--
-- Here, a delay is needed in LanceSurveillanceChAmis

Selene.exposeAdminAPI()	-- Use is part of Admin API
Selene.Use("SelScripting")	-- Selene.Sleep() part of this module
