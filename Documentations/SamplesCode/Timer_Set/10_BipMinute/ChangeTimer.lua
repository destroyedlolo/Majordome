-- Display the time
--
-->> when=minute
-->> need_timer=minute

local t = os.date('*t')
local delay = 60-t.sec

minute:setEvery(delay)

SelLog.Log( os.date("%X") )
SelLog.Log( "Seconds to wait :".. delay)

