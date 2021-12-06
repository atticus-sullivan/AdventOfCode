local _M = {}

local part1 = {}
function part1.forward(pos, x) pos.hor   = pos.hor   + x end
function part1.up(pos, x)      pos.depth = pos.depth - x end
function part1.down(pos, x)    part1.up(pos, -x) end

local part2 = {}
function part2.forward(pos, x)
	pos.hor = pos.hor + x
	pos.depth = pos.depth + pos.aim*x
end
function part2.up(pos, x)
	pos.aim = pos.aim - x
end
function part2.down(pos, x)
	part2.up(pos, -x)
end


local function part(file, funcSet)
	local pos = {hor=0, depth=0, aim=0}
	local n
	for line in file:lines() do
		n = line:match("%d+")
		if line:match("^forward")  then funcSet.forward(pos, n)
		elseif line:match("^down") then funcSet.down(pos, n)
		elseif line:match("^up")   then funcSet.up(pos, n)
		else
			error("Failed parsing line ", line)
		end
		-- print(line)
		-- print("hor", pos.hor, "depth", pos.depth, "aim", pos.aim, "\n")
	end
	return pos.hor * pos.depth
end

function _M.part1(file) return part(file, part1) end
function _M.part2(file) return part(file, part2) end

return _M
