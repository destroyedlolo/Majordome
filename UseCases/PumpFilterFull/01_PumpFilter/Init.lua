-- Load Séléné's module
--

Selene.exposeAdminAPI()				-- We're needing administration API.
Selene.Use("SelAverageCollection")	-- Load collection module
Selene.LetsGo()						-- Late dependencies solving

	-- Create a collection of 30 entries, 1 average covering 30 samples 
SelAverageCollection.Create("Pump", 5,1,5)
