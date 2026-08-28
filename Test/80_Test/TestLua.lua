-->> RunAtStartup
-->> need_renderer=LCD
-->> need_painting=SubSurface


print("-------------")
print("  Subsurface")
print("-------------")

print(SubSurface:getContainer(), 
	SubSurface:getName(),
	"Enable: ".. tostring(SubSurface:isEnabled()),
	"Visible: ".. tostring(SubSurface:isVisible())
)

SubSurface:Dump()

print("-------------")
print("  Renderer")
print("-------------")

print(LCD:getContainer(), 
	LCD:getName(),
	"Enable: ".. tostring(LCD:isEnabled()),
	"Visible: ".. tostring(LCD:isVisible())
)

LCD:Dump()	-- not exposed to the renderer

--[[
print("---------------------------------")
print("  Now the surface is visible")
print("---------------------------------")

SubSurface:setVisibility(true)

LCD:Dump()	-- not exposed to the renderer
--]]
