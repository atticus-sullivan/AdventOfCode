local _M = {}

local function parse_file(file)
	local ret,cnt = {},0
	for line in file:lines() do
		local display = {fst={}, snd={}}
		local state = 1
		for signal in line:gmatch("[abcdefg|]+") do
			if signal == "|" then state = state+1
			elseif state == 1 then
				table.insert(display.fst, signal)
			elseif state == 2 then
				table.insert(display.fst, signal)
				if #signal == 2  or #signal == 4 or #signal == 3 or #signal == 7 then
					cnt = cnt+1
				end
			else
				error(string.format("Error while parsing line %s signal %s state %d", line, signal, state))
			end
		end
		table.insert(ret, display)
	end
	return ret,cnt
end

function _M.part1(file)
	local cnt = 0
	for line in file:lines() do
		local state = 1
		for signal in line:gmatch("[abcdefg|]+") do
			if signal == "|" then
				state = state+1
			elseif state == 2 then
				if #signal == 2  or #signal == 4 or #signal == 3 or #signal == 7 then
					cnt = cnt+1
				end
			elseif state ~= 1 then
				error(string.format("Error while parsing line %s signal %s state %d", line, signal, state))
			end
		end
	end
	return cnt
end

local function permutations()
	local function _perm(input, output, p)
		if input[1] == nil then coroutine.yield(p) return end
		local i = table.remove(input, 1)
		for o=1,#output do
			local o2 = table.remove(output, o)
			p[i] = o2
			_perm(input, output, p)
			table.insert(output, o, o2)
		end
		table.insert(input, 1, i)
	end
	return coroutine.wrap(function() _perm({"a", "b", "c", "d", "e", "f", "g"}, {"a", "b", "c", "d", "e", "f", "g"}, {}) end)
end

function _M.part2(file)
	local digits = {
		[0] = {a=true  ,b=true  ,c=true  ,d=false ,e=true  ,f=true  ,g=true },
		[1] = {a=false ,b=false ,c=true  ,d=false ,e=false ,f=true  ,g=false},
		[2] = {a=true  ,b=false ,c=true  ,d=true  ,e=true  ,f=false ,g=true },
		[3] = {a=true  ,b=false ,c=true  ,d=true  ,e=false ,f=true  ,g=true },
		[4] = {a=false ,b=true  ,c=true  ,d=true  ,e=false ,f=true  ,g=false},
		[5] = {a=true  ,b=true  ,c=false ,d=true  ,e=false ,f=true  ,g=true },
		[6] = {a=true  ,b=true  ,c=false ,d=true  ,e=true  ,f=true  ,g=true },
		[7] = {a=true  ,b=false ,c=true  ,d=false ,e=false ,f=true  ,g=false},
		[8] = {a=true  ,b=true  ,c=true  ,d=true  ,e=true  ,f=true  ,g=true },
		[9] = {a=true  ,b=true  ,c=true  ,d=true  ,e=false ,f=true  ,g=true },
	}
	local function segsToDigit(segs)
		for i,d in pairs(digits) do
			local failed = false
			for k,v in pairs(d) do
				-- if i == 5 then print(i, k, d[k], v, segs[k]) end
				if not((v == true and segs[k] == true) or (v == false and segs[k] == nil)) then
					failed = true
					break
				end
			end
			if not failed then return i end
		end
		return nil
	end
	local function permResolution(perm, x)
		local ret = {}
		for y in x:gmatch("[a-g]") do
			ret[perm[y]] = true
		end
		return ret
	end
	local function checkPerm(perm, display)
		for _,d in ipairs(display.fst) do
			local real = {}
			for c in d:gmatch("[a-g]") do
				real[perm[c]] = true
			end
			local tmp = segsToDigit(real)
			if tmp then
			else
				return nil
			end
		end
		return perm
	end

	file = io.open("day08.dat")
	local ret = {}
	for line in file:lines() do
		local display,state = {fst={}, snd={}},1
		for signal in line:gmatch("[abcdefg|]+") do
			if signal == "|" then state = state+1
			elseif state == 1 then
				table.insert(display.fst, signal)
			elseif state == 2 then
				table.insert(display.snd, signal)
			else
				error(string.format("Error while parsing line %s signal %s state %d", line, signal, state))
			end
		end
		table.insert(ret, display)
	end

	local sum = 0
	for _,display in ipairs(ret) do
		for perm in permutations() do
			local tmp = checkPerm(perm, display)
			if tmp then
				local s = ""
				for _,x in ipairs(display.snd) do
					local y = permResolution(perm, x)
					s = s .. tostring(segsToDigit(y))
				end
				sum = sum + tonumber(s)
				break
			end
		end
	end
	return sum
end

local function set_len(a)
	local cnt = 0
	for _,_ in pairs(a) do cnt = cnt+1 end
	return cnt
end

local function solve(display)
	local function clear(signals)
		local changed = false
		repeat
			for i,_ in pairs(signals) do
				local one = nil
				for o,_ in pairs(signals[i]) do
					if signals[i][o] and one == nil then
						one = o
					elseif signals[i][o] and one == true then
						one = nil
						break
					end
				end
				if one then
					-- in row:i only one o is set -> clear column o apart from i
					for i2,_ in pairs(signals) do
						if i2 ~= i then
							if signals[i2][one] == false then
								changed = true
								signals[i2][one] = false
							end
						end
					end
				end
			end
			-- TODO do the same for row clearing if only one entry in column is set
		until not changed
	end
	local function inter(signals, si, set)
		for s in si:gmatch("[abcdefg]") do
			if set_len(signals[s]) > 1 then
				signals[s] = SetIntersect(signals[s], set)
			end
		end
	end
	local function SetContainsOne(set, one)
		local ret = nil
		for x,_ in pairs(set) do
			if one[x] ~= nil then
				if ret == nil then
					ret = x
				else
					return nil
				end
			end
			return ret
		end
	end
	-- input -> möglicher output
	local signals = {
		a={a=true, b=true, c=true, d=true, e=true, f=true, g=true},
		b={a=true, b=true, c=true, d=true, e=true, f=true, g=true},
		c={a=true, b=true, c=true, d=true, e=true, f=true, g=true},
		d={a=true, b=true, c=true, d=true, e=true, f=true, g=true},
		e={a=true, b=true, c=true, d=true, e=true, f=true, g=true},
		f={a=true, b=true, c=true, d=true, e=true, f=true, g=true},
		g={a=true, b=true, c=true, d=true, e=true, f=true, g=true},
	}
	local eight = nil
	-- step 1
	for _,si in ipairs(display) do
		if #si == 2 then
			inter(signals, si, {c=true,f=true})
		elseif #si == 3 then
			inter(signals, si, {a=true,c=true,f=true})
		elseif #si == 4 then
			inter(signals, si, {b=true,c=true,d=true,f=true})
		elseif #si == 7 then
			eight = si
		end
	end
	-- step 2
	for _,si in ipairs(display) do
		if #si == 6 then
			for x in eight:gmatch("[a-g]") do
				if not si:match(x) then
					local y = SetContainsOne(signals[x], {e=9,c=6,d=0})
					-- y cannot appear in signals[si] -> loop over si and unset y for each
					-- as a result only for one entry y will be set -> make a vertical clear
					-- or even better make horizontal/vertical clear as long as something changes
				end
			end
		end
	end
	PrintTable(signals)
	os.exit()
end

-- was supposed to be the pretty solution with logic, but has gotten pretty dirty
function _M.part2_sol(file)
	file = io.open("day08.dat.testing")
	local ret,cnt = {},0
	for line in file:lines() do
		local display,state = {},1
		for signal in line:gmatch("[abcdefg|]+") do
			if signal == "|" then state = state+1
			elseif state == 1 then
				table.insert(display, signal)
			elseif state == 2 then
				local map = solve(display)
				if #signal == 2  or #signal == 4 or #signal == 3 or #signal == 7 then
					-- TODO resolve four digits
					cnt = cnt+1
				end
			else
				error(string.format("Error while parsing line %s signal %s state %d", line, signal, state))
			end
		end
		table.insert(ret, display)
	end
	return cnt
end

 --[[
{
	{a -> {a,b,c,d,e,f,g}
	{b -> {a,b,c,d,e,f,g}
	{c -> {a,b,c,d,e,f,g}
	{d -> {a,b,c,d,e,f,g}
	{e -> {a,b,c,d,e,f,g}
	{f -> {a,b,c,d,e,f,g}
}
]]

return _M
