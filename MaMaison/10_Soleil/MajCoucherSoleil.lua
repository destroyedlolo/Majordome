-->> listen=CoucherSoleil

local timer = MajordomeTimer.find("CoucherSoleil")
assert( timer, '"CoucherSoleil" pas trouvé' )

local h,m = string.match(string.gsub( SelShared.Get('CoucherSoleil'), '%.', ':'), "(%d+):(%d+)")

timer:setAtHM( h, m )

SelLog.log('I', "Le soleil se couche à ".. h ..":".. m)
