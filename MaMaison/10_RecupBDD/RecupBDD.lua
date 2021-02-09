-- Récupération des données historisées dans la BDD
--
-->> listen=DemandeHistorique

-- topic de réponse

local TopicReponse = MajordomeMQTTTopic.find("ReponseHistorique", true)


-- Connexion à la base

local pgmoon = require "pgmoon"
local pg = pgmoon.new( {
	database = "www",
--	user= "toto"
})

assert(pg:connect())


-- Transcodage des températures

local temperature = {
	['TSSPorte'] = 'Porte Garage',
	['TSS'] = 'Garage',
	['TBuanderie'] = 'Buanderie',
	['TCVin'] = 'Cave a vin',
	['TBureau'] = 'Bureau',
	['TChParent'] = 'Chambre Parents',
	['TSalon'] = 'Salon',
	['TGrenierNord'] = 'Grenier Nord',
	['TChJoris'] = 'Chambre Joris',
	['TChOceane'] = 'Chambre Oceane',
	['TAmis'] = 'Chambre Amis',
	['TGrenierSud'] = 'Grenier Sud',
	['TComble'] = 'Comble'
}

SelLog.log('D', "Demande de récupération pour : "..MAJORDOME_PAYLOAD)
if temperature[MAJORDOME_PAYLOAD] then -- temperature standard
	local query = "select extract(epoch from sample_time) as \"when\", value from domestik.probe_hardware where host = 'Maison' and probe = '".. temperature[MAJORDOME_PAYLOAD] .. "' order by sample_time;"
	local topic = TopicReponse:getTopic() .."/".. MAJORDOME_PAYLOAD

--	SelLog.log('D', ">>> ".. query)
--	SelLog.log('D', ">>> ".. topic)

	local res = pg:query(query)
	if res then
		local reponse
		for _,v in ipairs(res) do
			if reponse then 
				reponse = reponse .. "\n" .. v.when .."\t".. v.value
			else
				reponse = v.when .."\t".. v.value
			end
--			print(v.when, v.value)
		end
		SeleMQTT.Publish( topic, reponse )
	else
		SelLog.log('E', query)
	end
end
