-- Class to handle shutters with temperature survey

function SShutterTempSurvey(aname, atpc, atimer, atemp, astart, astop, alimit)
	local self = SShutter(aname, atpc)

	-- local fields
	local timer = atimer			-- Timer to modify
	local probe = atemp				-- temperature's probe
	local HMonitoringStart = astart	-- start of monitoring
	local HMonitoringStop = astop	-- end of monitoring
	local TemperatureLimit = alimit	-- temperature triggering freshness saving

	-- methods
	local function surveyTemperature()
		if probe.get() > TemperatureLimit then
			SelLog.log( self.getName() .. " : " .. probe.get() .. ", les volets se ferment")
			self.My()
			StopSurvey()

			timer.TaskAdd( HMonitoringStop, self.Up )
			SelLog.log( self.getName() .. " : Les volets s'ouvriront à " .. HMonitoringStop )
		end
	end

	local function LaunchSurvey()
		SelLog.log("Début de la surveillance de la température pour " .. self.getName() )
		timer.TaskRemove( LaunchSurvey )
		probe.TaskAdd( surveyTemperature )
	end

	local function StopSurvey()
		SelLog.log("Fin de la surveillance de la température pour " .. self.getName() )
		timer.TaskRemove( LaunchSurvey )
		probe.TaskRemove( surveyTemperature )
	end

	function self.LaunchSurveyAt( time )
		local cur = atimer.Current()

		if cur < HMonitoringStart then		-- Before monitoring period
			SelLog.log( self.getName() .. " : La surveillance commencera à " .. HMonitoringStart )
			timer.TaskAdd( HMonitoringStart, LaunchSurvey )

			SelLog.log( "Et se terminera à " .. HMonitoringStop )
			timer.TaskAdd( HMonitoringStart, StopSurvey )

			probe.TaskRemove( surveyTemperature )
		elseif cur < HMonitoringStop then	-- during
			LaunchSurvey()

			SelLog.log( self.getName() .. " : La surveillance se terminera à " .. HMonitoringStop )
			timer.TaskAdd( HMonitoringStart, StopSurvey )

			probe.TaskRemove( surveyTemperature )
		else								-- after
			StopSurvey()
		end
	end

	return self
end

