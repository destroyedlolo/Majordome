-->> listen=LeverSoleil

local timer = MajordomeTimer.find("LeverSoleil", true)

local h,m = string.match(string.gsub( SelShared.Get('LeverSoleil'), '%.', ':'), "(%d+):(%d+)")

timer:setAtHM( h, m )

SelLog.log('I', "Le soleil se leve à ".. h ..":".. m)
