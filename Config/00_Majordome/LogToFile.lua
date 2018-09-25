-- Initialise log to file will every day log rotation
-->> until=MidnightOrAtLaunch
local _,err = SelLog.init('/tmp/'.. MAJORDOME_ClientID ..'.log_' .. os.date('%Y%m%d'), false)

if err then
	SelLog.log("*E* Log file creation : ".. err)
end
