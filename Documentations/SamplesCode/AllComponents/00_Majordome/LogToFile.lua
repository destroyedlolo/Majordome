-- Log to file.
-- As relaunched every day at midnight, it's doing also log rotation
--
-->> when=MidnightOrAtLaunch

local _,err = SelLog.init('/tmp/'.. MAJORDOME_ClientID ..'.log_' .. os.date('%Y%m%d'), false)

if err then
	SelLog.Log("*E* Log file creation : ".. err)
end
