-->> RunAtStartup
-->> need_renderer=LCD
-->> need_painting=SubSurface
-->> need_painting=SubSurface2
-->> need_painting=SubSurface3


print("---------------")
print("  Subsurfaces")
print("---------------")

print(SubSurface:getContainer(), 
	SubSurface:getName(),
	"Enable: ".. tostring(SubSurface:isEnabled()),
	"Visible: ".. tostring(SubSurface:isVisible())
)
SubSurface:Dump()

print(SubSurface2:getContainer(), 
	SubSurface2:getName(),
	"Enable: ".. tostring(SubSurface2:isEnabled()),
	"Visible: ".. tostring(SubSurface2:isVisible())
)
SubSurface2:Dump()

print(SubSurface3:getContainer(), 
	SubSurface3:getName(),
	"Enable: ".. tostring(SubSurface3:isEnabled()),
	"Visible: ".. tostring(SubSurface3:isVisible())
)
SubSurface3:Dump()

print("-------------")
print("  Renderer")
print("-------------")

print(LCD:getContainer(), 
	LCD:getName(),
	"Enable: ".. tostring(LCD:isEnabled()),
	"Visible: ".. tostring(LCD:isVisible())
)

LCD:Dump()	-- not exposed to the renderer

print("---------------------------------")
print("  Now the surface3 is visible")
print("---------------------------------")

SubSurface3:setVisibility(true)

LCD:Dump()	-- not exposed to the renderer
