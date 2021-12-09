assert(arg[1] and arg[1]:match("^%d%d$"), "Day must be given as first parameter and has to be a two digit number")
assert(not arg[2] or arg[2] == "1" or arg[2] == "2", "Part to execute must be none(both), 1 or 2")

-- global helper functions
function ShallowCopyTable(t)
	local res = {}
	for i,v in pairs(t) do
		res[i] = v
	end
	return res
end
function DeepCopyTable(t)
	if type(t) ~= "table" then return t end
	local res = {}
	for i,v in pairs(t) do
		res[i] = DeepCopyTable(v)
	end
	return res
end
function PrintTable(t, shift)
	local function printTable(t, indent)
		if type(t) ~= "table" then io.write(string.rep(" ", indent), tostring(t), "\n") return end
		for k,v in pairs(t) do
			io.write(string.rep(" ", indent), k, " -> \n")
			printTable(v, indent+(shift or 2))
		end
	end
	printTable(t, 0)
end
function SetInsert(set, ele)
	if set[ele] ~= nil then return nil end
	set[ele] = true
	return ele
end
function SetRemove(set, ele)
	set[ele] = nil
	return ele
end
function SetUnion(a, b)
	local ret = {}
	for k,_ in pairs(a) do
		ret[k] = true
	end
	for k,_ in pairs(b) do
		ret[k] = true
	end
	return ret
end
function SetIntersect(a, b)
	local ret = {}
	for k,_ in pairs(a) do
		if b[k] == true and a[k] == true then
			ret[k] = true
		else
			ret[k] = false
		end
	end
	return ret
end
function SetLen(s)
	local len = 0
	for _,v in pairs(s) do
		if v then len = len + 1 end
	end
	return len
end


local day = require("day"..arg[1])

local file
if not arg[2] or arg[2] == "1" then
	file = io.open("day"..arg[1]..".dat")
	local s = os.clock()
	local r = day.part1(file)
	local e = os.clock()
	print("Part1:", r, e-s)
	file:close()
end

if not arg[2] or arg[2] == "2" then
	file = io.open("day"..arg[1]..".dat")
	local s = os.clock()
	local r = day.part2(file)
	local e = os.clock()
	print("Part2:", r, e-s)
	file:close()
end
