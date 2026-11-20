-- Slave task to be launched
--
-->> listen=Tasmota_State
-->> need_topic=CPULoad_Template
-->> need_topic=Memory_Template

local cjson = require("cjson")

local host = MAJORDOME_TOPIC:match('^.-/([^/]+)')

local data = cjson.decode(MAJORDOME_PAYLOAD)

SelLog.Log('R', "h: " .. host .. " up: " .. data.Uptime .. " mem: ".. data.Heap .." load: ".. data.LoadAvg)

local cpuload = CPULoad_Template:getTopic():gsub('%%Machine%%', host)
MQTTBroker:Publish(cpuload, data.LoadAvg/100.0)

local memory = Memory_Template:getTopic():gsub('%%Machine%%', host)
MQTTBroker:Publish(memory, (80-data.Heap)/80*100)
