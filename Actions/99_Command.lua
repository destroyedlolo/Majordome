-- Controle Majordom
local function AMajordomCtrl()
	local self = {}

	-- private fields
	local cmds = {}

	local function help()
		local res = 'Known commands'
		for c,v in pairs( cmds ) do
			res = res .. '\n\t' .. c .. '\t' .. v.desc
		end
		SelLog.log( res )
	end

	local function dump_timer()
		SelLog.log( '*d* ' .. Timer.listentries() )
	end

	local function det_seas()
		Saison.DetermineSaison()
	end

	cmds.help = { func=help, desc='List known commands' }
	cmds.dump_timer = { func=dump_timer, desc="Dump timer's entries" }
	cmds.determine_season = { func=det_seas, desc="Launch season determination" }

	local function ExecCmd()
		local cmd = SelShared.get( 'Majordome/'.. CLIENTID )
		if cmds[ cmd ] then
			cmds[ cmd ].func()
		else
			SelLog.log( '*E* Unknown command : "'.. cmd ..'"')
		end
	end

	-- initialiser
	table.insert( Topics,
		{
			topic = 'Majordome/'.. CLIENTID,
			trigger = ExecCmd,
			trigger_once=true
		}
	)
	SelLog.log( '*I* Listening on topic : "'..  'Majordome/'.. CLIENTID .. '"')

	return self
end
Cmd = AMajordomCtrl()
