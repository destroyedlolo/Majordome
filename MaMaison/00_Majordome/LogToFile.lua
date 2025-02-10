-- Log to file.
-- As relaunched every day at midnight, it's doing also log rotation
--
-->> when=MidnightOrAtLaunch

Selene.exposeAdminAPI()	-- configure is part of admin API

local _,err = SelLog.configure('/tmp/'.. MAJORDOME_ClientID ..'.log_' .. os.date('%Y%m%d'), true)

if err then
	SelLog.Log("*E* Log file creation : ".. err)
end
