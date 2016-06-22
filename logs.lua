-- This script handles logging

function rotatelog( VERSION )
	local msg
	if SelLog.status() then
		msg = "Rotation des logs"
	else
		msg = "\n\tLancement de Majordome v".. VERSION .."; Selene v".. SELENE_VERSION
	end

	if VERBOSE then
		local _,err = SelLog.init('/tmp/Majordome.log_' .. os.date('%Y%m%d'))
		if err then
			print("*E* Log creation", err)
		end
	end
	local _,err = SelLog.log(msg)
	if err then
		print("*E* Log creation", err)
	end
end

