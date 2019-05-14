-->> listen=LeverSoleil

local timer = MajordomeTimer.find("LeverSoleil", true)

local h,m = string.match(string.gsub( SelShared.Get('LeverSoleil'), '%.', ':'), "(%d+):(%d+)")

SelLog.log('I', "Le soleil se leve à ".. h ..":".. m)

local saison = SelShared.Get("Saison") or "Hiver"
if saison == "Hiver" then
	timer:setAtHM( h, m+30 )
	h,m = timer:getAtHM()
	SelLog.log('I', "Comme c'est l'ouvert l'information \"Lever Soleil\" est repoussée à ".. h ..":".. m)
else
	timer:setAtHM( h, m )
end

