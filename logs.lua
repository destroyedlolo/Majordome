-- This script handles logging

function rotatelog( VERSION )
	local msg
	if SelLog.status() or not VERSION then
		msg = "Rotation des logs"
	else
		msg = "\n\tLancement de Majordome v".. VERSION .."; Selene v".. SELENE_VERSION
	end

	if VERBOSE then
		local _,err = SelLog.init('/tmp/Majordome.log_' .. os.date('%Y%m%d'), false)
		if err then
			print("*E* Log creation", err)
		end
	end
	local _,err = SelLog.log(msg)
	if err then
		print("*E* Log creation", err)
	end
end

function pubLog( msg, cat )
	SelLog.log(msg)

	local topic = CLIENTID ..'/Log'
	if cat then
		topic = topic .. '/' .. cat
	end
	msg = os.date('[%H:%M:%S] ') .. msg
	Brk:Publish( topic, msg )
end
