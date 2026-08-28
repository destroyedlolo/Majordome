-->> desc=Define custom chars
-->> RunAtStartup
-->> need_renderer=LCD

-- Define custom characters
-- char 0-2 are already used by the clock

-- 3 : Photovoltaic
LCD:SetChar(3, {
	"     ",
	"  XXx",
	" xxx ",
	"xxx  ",
	"  xx ",
	" xx  ",
	"x    ",
	"     "
})

-- 4 : Consumption
LCD:SetChar(4, {
	"     ",
	" x x ",
	"xxxxx",
	"xxxxx",
	" xxx ",
	"  x  ",
	"  x  ",
	"  x  "
})

-- 5 : powersaving
LCD:SetChar(5, {
	" xXX ",
	"x   X",
	"XXX  ",
	"X    ",
	"XXX  ",
	"x   X",
	" xXx ",
	"     "
})

-- 6 : Volt-Ampere
LCD:SetChar(6, {
	"x   x",
	"x  x ",
	"x x  ",
	"xx  x",
	"x  xx",
	"  x x",
	" xxxx",
	"x   x"
})

-- 7: Text
LCD:SetChar(7, {
	"xxx  ",
	" x   ",
	" x   ",
	"     ",
	"  xx ",
	" xxxx",
	" x   ",
	"  xx "
})
