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
	local pos,count,sum = {},0,0
	local min,max = 10000,0
	for n in file:read("l"):gmatch("%d+") do
		n = tonumber(n)
		table.insert(pos, n)
		if n < min then min = n end
		if n > max then max = n end
		count = count + 1
		sum = sum + n
	end

	local avg,sum2
	avg,sum = math.floor(sum/count),0
	for _,v in ipairs(pos) do
		sum = sum + gauss(math.abs(avg-v))
	end
	avg,sum2 = math.floor(sum/count),0
	for _,v in ipairs(pos) do
		sum2 = sum2 + gauss(math.abs(avg-v))
	end
	return sum < sum2 and sum or sum2
end

return _M
