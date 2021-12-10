local _M = {}

function _M.part1(file)
	local function searchList(list, char)
		for i,v in ipairs(list) do
			if v == char then return i end
		end
		return nil
	end
	local closing = {")", "]", "}", ">"}
	local opening = {"(", "[", "{", "<"}
	local score   = {[")"]=3, ["]"]=57, ["}"]=1197, [">"]=25137}
	local stack
	local sum = 0
	for line in file:lines() do
		stack = {}
		for x in line:gmatch(".") do
			local  c = searchList(closing,x)
			if     c and stack[#stack] == opening[c] then table.remove(stack)
			elseif c and stack[#stack] ~= opening[c] then sum = sum + score[x] break
			elseif searchList(opening,x)                  then table.insert(stack, x)
			end
		end
	end
	return sum
end

function _M.part2(file)
	local function searchList(list, char)
		for i,v in ipairs(list) do
			if v == char then return i end
		end
		return nil
	end
	local closing = {")", "]", "}", ">"}
	local opening = {"(", "[", "{", "<"}
	local score   = {["("]=1, ["["]=2, ["{"]=3, ["<"]=4}
	local stack,failed
	local sums = {}
	for line in file:lines() do
		stack,failed = {},false
		for x in line:gmatch(".") do
			local  c = searchList(closing,x)
			if     c and stack[#stack] == opening[c] then table.remove(stack)
			elseif c and stack[#stack] ~= opening[c] then failed = true break
			elseif searchList(opening,x)        then table.insert(stack, x)
			end
		end
		if not failed then
			local sum = 0
			for i=#stack,1,-1 do
				sum = 5*sum + score[stack[i]]
			end
			table.insert(sums,sum)
		end
	end
	table.sort(sums)
	return sums[#sums//2+1]
end

return _M
