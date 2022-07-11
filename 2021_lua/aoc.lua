assert(arg[1] and arg[1]:match("^%d%d$"), "Day must be given as first parameter and has to be a two digit number")
assert(not arg[2] or arg[2] == "1" or arg[2] == "2" or arg[2] == "0", "Part to execute must be 0(both), 1 or 2")

local day = require("day"..arg[1])

graphic = arg[3] == "g"

local file
if not arg[2] or arg[2] == "1" or arg[2] == "0" then
	file = io.open("day"..arg[1]..".dat")
	local s = os.clock()
	local r = day.part1(file)
	local e = os.clock()
	print("Part1:", r, e-s)
	file:close()
end

if not arg[2] or arg[2] == "2" or arg[2] == "0" then
	file = io.open("day"..arg[1]..".dat")
	local s = os.clock()
	local r = day.part2(file)
	local e = os.clock()
	print("Part2:", r, e-s)
	file:close()
end
