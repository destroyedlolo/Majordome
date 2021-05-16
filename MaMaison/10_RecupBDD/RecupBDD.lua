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
-- host / probe

local temperature = {
	['TDehors'] = { host='Maison', probe='Exterieur' },
	['TSSPorte'] = { host='Maison', probe='Porte Garage' },
	['TSS'] = { host='Maison', probe='Garage' },
	['TBuanderie'] = { host='Maison', probe='Buanderie' },
	['TCVin'] = { host='Maison', probe='Cave a vin' },
	['TBureau'] = { host='Maison', probe='Bureau' },
	['TChParent'] = { host='Maison', probe='Chambre Parents' },
	['TSalon'] = { host='Maison', probe='Salon' },
	['TGrenierNord'] = { host='Maison', probe='Grenier Nord' },
	['TChJoris'] = { host='Maison', probe='Chambre Joris' },
	['TChOceane'] = { host='Maison', probe='Chambre Oceane' },
	['TAmis'] = { host='Maison', probe='Chambre Amis' },
	['TGrenierSud'] = { host='Maison', probe='Grenier Sud' },
	['TComble'] = { host='Maison', probe='Comble' },
	['consommation2'] = { host='EDF', probe='consomation_Puissance' },
	['production2'] = { host='EDF', probe='production_Puissance' },
}

local query
local probe,limit = string.match( MAJORDOME_PAYLOAD, "(%w+);(%d+)" )
if not limit then
	if not temperature[MAJORDOME_PAYLOAD] then
		SelLog.log('E', "Demande de récupération pour inconnu : "..MAJORDOME_PAYLOAD)
		return
	end
	probe = MAJORDOME_PAYLOAD
	query = "select extract(epoch from sample_time) as \"when\", value from domestik.probe_hardware where host = '".. temperature[MAJORDOME_PAYLOAD].host .."' and probe = '".. temperature[MAJORDOME_PAYLOAD].probe .. "' order by sample_time;"
else
	if not temperature[probe] then
		SelLog.log('E', "Demande de récupération pour inconnu : "..probe)
		return
	end
	query = "select * from (select extract(epoch from sample_time) as \"when\", sample_time, value from domestik.probe_hardware where host = '".. temperature[probe].host .."' and probe = '".. temperature[probe].probe .. "' order by sample_time desc limit ".. limit ..") as tmp order by \"when\";"
end

SelLog.log('I', "Demande de récupération pour : " .. probe)
local topic = TopicReponse:getTopic() .."/" ..probe

-- SelLog.log('D', ">>> ".. query)
-- SelLog.log('D', ">>> ".. topic)

local res = pg:query(query)
if res then
	local reponse
	for _,v in ipairs(res) do
		if reponse then 
			reponse = reponse .. "\n" .. v.when .."\t".. v.value
		else
			reponse = v.when .."\t".. v.value
		end
--		print(v.when, v.value)
	end
	SeleMQTT.Publish( topic, reponse )
else
	SelLog.log('E', query)
end

