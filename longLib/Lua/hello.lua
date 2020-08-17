local luaLib = require("luaLib")

local aT = os.clock()
luaLib.sayHello()
local a = luaLib.averageFunc(2, 4, 5, 6)
print(a)
local bT = os.clock()
print(bT - aT)
