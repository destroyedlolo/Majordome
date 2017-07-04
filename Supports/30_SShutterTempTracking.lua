-- Class to handle shutters with temperature tracking

function SShutterTempTracking(aname, atpc, atimer, atemp, astart, astop, alimit)
	local self = SShutter(aname, atpc)

	-- local fields
	local timer = atimer			-- Timer to modify
	local probe = atemp				-- temperature's probe
	local HMonitoringStart = astart	-- start of monitoring
	local HMonitoringStop = astop	-- end of monitoring
	local TemperatureLimit = alimit	-- temperature triggering freshness saving

	-- methods
	local TemperatureTracking	-- forward declaration

	local function StopTracking()
		pubLog(self.getName() .. " : Fin de la surveillance de la température", 'Action')
		timer.TaskRemove( LaunchTracking )
		timer.TaskRemove( StopTracking )
		probe.TaskRemove( TemperatureTracking )
	end

	TemperatureTracking = function ()
		if probe.get() > TemperatureLimit then
			pubLog( self.getName() .. " : " .. probe.get() .. ", les volets se ferment", 'Action')
			self.My()
			StopTracking()

			timer.TaskAdd( HMonitoringStop, self.Up )
			pubLog( self.getName() .. " : Les volets s'ouvriront à " .. HMonitoringStop )
		elseif DEBUG then
			SelLog.log( self.getName() .. " : " .. probe.get() .. ' -> trop basse' )
		end
	end

	local function LaunchTracking()
		pubLog(self.getName() .." : Début de la surveillance de la température", 'Action')
		timer.TaskRemove( LaunchTracking )
		probe.TaskAdd( TemperatureTracking )
	end

	function self.LaunchTrackingAt( atime )
		local cur = atimer.Current()

		if not atime then
			atime = HMonitoringStart
		end
		if cur < atime then		-- Before monitoring period
			pubLog( self.getName() .. " : La surveillance commencera à " .. atime )
			timer.TaskAdd( atime, LaunchTracking )

			if SunSet.get() <= HMonitoringStop then
				pubLog( self.getName() .. " : Les volets se fermeront avant la fin de la surveillance" )
			else
				pubLog( self.getName() .. " : Et se terminera à " .. HMonitoringStop )
				timer.TaskAdd( HMonitoringStop, StopTracking )
			end

			probe.TaskRemove( TemperatureTracking )
		elseif cur < HMonitoringStop then	-- during
			LaunchTracking()

			if SunSet.get() <= HMonitoringStop then
				pubLog( self.getName() .. " : Les volets se fermeront avant la fin de la surveillance" )
			else
				pubLog( self.getName() .. " : La surveillance se terminera à " .. HMonitoringStop )
				timer.TaskAdd( HMonitoringStop, StopTracking )
			end
		else								-- after
			StopTracking()
		end
	end

	function self.getTimer()
		return timer
	end

	return self
end

