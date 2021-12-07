local _M = {}

function _M.part1(file)
	local pos = {}
	for n in file:read("l"):gmatch("%d+") do
		table.insert(pos, tonumber(n))
	end
	table.sort(pos)
	local height = pos[#pos//2]
	local sum = 0
	for _,v in ipairs(pos) do
		sum = sum + math.abs(height-v)
	end
	return sum
end

local function gauss(n) return (n*(n+1))//2 end

function _M.part2(file)
	local pos = {}
	local min,max = 10000,0
	for n in file:read("l"):gmatch("%d+") do
		n = tonumber(n)
		table.insert(pos, n)
		if n < min then min = n end
		if n > max then max = n end
	end

	local minFuel = 1000000000000
	for p = min,max do
		local fuel = 0
		for _,n in ipairs(pos) do
			fuel = fuel + gauss(math.abs(p-n))
		end
		if fuel < minFuel then minFuel = fuel end
	end
	return minFuel
end

return _M
