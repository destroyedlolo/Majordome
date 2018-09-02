-- Initialise log to file will every day log rotation
-->> waitfor=MidnightOrAtLaunch
local _,err = SelLog.init('/tmp/'.. MAJORDOME_ClientID ..'.log_' .. os.date('%Y%m%d'), true)
-- *AF* remetre false comme dernier parametre.

if err then
	SelLog.log("*E* Log file creation : ".. err)
end
