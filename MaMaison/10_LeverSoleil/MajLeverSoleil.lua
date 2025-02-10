-->> listen=LeverSoleil
-->> need_timer=LeverSoleil

local h,m = string.match(string.gsub( SelSharedVar.Get('LeverSoleil'), '%.', ':' ), "(%d+):(%d+)")

SelLog.Log('I', "Le soleil se leve à ".. h ..":".. m)

local saison = SelSharedVar.Get("Saison") or "Hiver"
if saison == "Hiver" then
	LeverSoleil:setAtHM( h, m+30 )
	h,m = LeverSoleil:getAtHM()
	SelLog.Log('I', "Comme c'est l'hiver l'information \"Lever Soleil\" est repoussée à ".. h ..":".. m)
else
	LeverSoleil:setAtHM( h, m )
end

