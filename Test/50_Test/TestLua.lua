-->> RunAtStartup
-->> need_renderer=LCD
-->> need_painting=SubSurface


print("-------------")
print("  Subsurface")
print("-------------")

print(SubSurface:getContainer(), SubSurface:getName(), SubSurface:isEnabled() )
print(SubSurface:isVisible())

SubSurface:Dump()

print("-------------")
print("  Renderer")
print("-------------")

print(LCD:getContainer(), LCD:getName(), LCD:isEnabled() )
print(LCD:isVisible())

LCD:Dump()	-- not exposed to the renderer

print("---------------------------------")
print("  Now the surface is visible")
print("---------------------------------")

SubSurface:setVisibility(true)

LCD:Dump()	-- not exposed to the renderer

