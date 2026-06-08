-->> RunAtStartup
-->> need_renderer=LCD
-->> need_painting=SubSurface

print(SubSurface:getContainer(), SubSurface:getName(), SubSurface:isEnabled() )
print(SubSurface:isVisible())

SubSurface:Clear()
SubSurface:Dump()

print("----------")
print("  Renderer")
print("----------")

print(LCD:getContainer(), LCD:getName(), LCD:isEnabled() )
print(LCD:isVisible())

LCD:Clear()
SubSurface:Dump()	-- not exposed to the renderer
