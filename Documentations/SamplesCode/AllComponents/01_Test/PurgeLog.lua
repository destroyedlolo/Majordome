-- Delete older logs
-->> when=MidnightOrAtLaunch

require 'lfs'

local RETENTION = 30 -- Number of days to keep

local nbre_del, nbre_kept = 0,0
RETENTION = RETENTION*86400

for file in lfs.dir("/tmp") do
	if file:sub(1, MAJORDOME_ClientID:len()) == MAJORDOME_ClientID then
		local filef = "/tmp/" .. file
		local t = lfs.attributes( filef, 'modification' )
		if t < os.time()-RETENTION then
			nbre_del = nbre_del + 1
			os.remove(filef)
		else
			nbre_kept = nbre_kept + 1
		end
	end
end

SelLog.Log('I', nbre_del .. " log file" .. ((nbre_del > 1) and 's' or '') .. " deleted / ".. nbre_kept .." kept")
