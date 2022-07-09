local _M      = {}
local _Packet = {}

local hex2bin = {
	["0"] = {0,0,0,0},
	["1"] = {0,0,0,1},
	["2"] = {0,0,1,0},
	["3"] = {0,0,1,1},
	["4"] = {0,1,0,0},
	["5"] = {0,1,0,1},
	["6"] = {0,1,1,0},
	["7"] = {0,1,1,1},
	["8"] = {1,0,0,0},
	["9"] = {1,0,0,1},
	["A"] = {1,0,1,0},
	["B"] = {1,0,1,1},
	["C"] = {1,1,0,0},
	["D"] = {1,1,0,1},
	["E"] = {1,1,1,0},
	["F"] = {1,1,1,1},
}
local type2foo = {
	[0] = {"sum",      0,     function (acc, x) return true, acc + x end},
	[1] = {"prod",     1,     function (acc, x) return true, acc * x end},
	[2] = {"min",      10e20, function (acc, x) return true, math.min(acc, x) end},
	[3] = {"max",      0,     function (acc, x) return true, math.max(acc, x) end},
	[4] = {"literal",  0,     function (  _, _) error("content is a literal") end},
	[5] = {"gt",      -1,     function (acc, x) if acc == -1 then return true, x else return false, acc >  x and 1 or 0 end end},
	[6] = {"lt",      -1,     function (acc, x) if acc == -1 then return true, x else return false, acc <  x and 1 or 0 end end},
	[7] = {"eq",      -1,     function (acc, x) if acc == -1 then return true, x else return false, acc == x and 1 or 0 end end},
}



local function co_fail(v) if not table.remove(v,1) then print(table.unpack(v)) return true else return false end end
local function iter(file, n)
	local state = {}
	local function insHex(h)
		if h ~= "\n" then
			table.move(hex2bin[h], 1, 4, #state+1, state)
			return true
		else
			return nil
		end
	end
	while true do
		while #state < n do
			if not insHex(file:read(1)) then break end
		end
		if #state < n then
			return table.unpack(state, 1, n)
		end
		local n_ = coroutine.yield(table.unpack(state, 1, n))
		state = {table.unpack(state, n+1)}
		n = n_
	end
end

function _Packet.raw_val(co, p)
	local val = {}
	local v = {coroutine.resume(co, 5)}
	p.length = p.length + 5
	if not table.remove(v, 1) then return nil end -- check coroutine state
	while v[1] == 1 do
		table.remove(v, 1)
		table.move(v, 1, #v, #val+1, val)
		v = {coroutine.resume(co, 5)}
		p.length = p.length + 5
		if not table.remove(v, 1) then return nil end -- check coroutine state
	end
	table.remove(v, 1)
	table.move(v, 1, #v, #val+1, val)

	p.value = tonumber(table.concat(val), 2)
end

function _Packet.parse_operator(co, p)
	p.mode = {coroutine.resume(co, 1)}
	p.length = p.length + 1
	if co_fail(p.mode) then return nil end
	p.mode = p.mode[1] -- only one bit

	if p.mode == 0 then
		-- next 15 bits are total length
		p.sublength = {coroutine.resume(co, 15)}
		p.length = p.length + 15
		if co_fail(p.sublength) then return nil end
		p.sublength = tonumber(table.concat(p.sublength), 2)

		local s = 0
		while s < p.sublength do
			local ps = _Packet.parse(co)
			s = s + ps.length
			p.length = p.length + ps.length
			table.insert(p.sub, ps)
		end
	else
		-- next 11 bits are number of packets
		p.num = {coroutine.resume(co, 11)}
		p.length = p.length + 11
		if co_fail(p.num) then return nil end
		p.num = tonumber(table.concat(p.num), 2)

		for _ = 1,p.num do
			local ps = _Packet.parse(co)
			p.length = p.length + ps.length
			table.insert(p.sub, ps)
		end
	end
end

function _Packet.parse(co)
	local p = {version=-1, type=-1, value=-1, mode=-1, sublen=-1, num=-1, sub={}, length=0}

	p.version = {coroutine.resume(co, 3)}
	p.length = p.length + 3
	if co_fail(p.version) then return nil end
	p.version = tonumber(table.concat(p.version), 2)

	p.type = {coroutine.resume(co, 3)}
	p.length = p.length + 3
	if co_fail(p.type) then return nil end
	p.type = tonumber(table.concat(p.type), 2)

	if p.type == 4 then
		-- raw value
		_Packet.raw_val(co, p)
	else
		-- operator
		_Packet.parse_operator(co, p)
	end
	return p
end

function _Packet.print(p, indent, t)
	indent = indent or 0
	t = t or {}
	t.t = t.t or 0
	t.t = t.t + p.version
	print(string.format("%sVersion:     %d", string.rep(" ",indent), p.version))
	print(string.format("%sType ID:     %d (%s)",    string.rep(" ",indent), p.type, type2foo[p.type][1]))
	print(string.format("%sPkgLength:   %d bits",    string.rep(" ",indent), p.length))
	if p.type == 4 then
		print(string.format("%sValue:       %d",   string.rep(" ",indent), p.value))
	else
		print(string.format("%smode:        %d %s",   string.rep(" ",indent), p.mode, p.mode == 0 and "-> bits" or "-> subpackets"))
		if p.mode == 0 then
			print(string.format("%sLength:      %d bits",   string.rep(" ",indent), p.sublength))
		else
			print(string.format("%sLength:      %d subpackets",   string.rep(" ",indent), p.num))
		end
		local first = true
		for _,v in ipairs(p.sub) do
			if not first then
				print()
			else
				first = false
			end
			_Packet.print(v, indent+4, t)
		end
	end
end

function _Packet.add_version(p)
	local r = p.version
	for _,v in ipairs(p.sub) do
		r = r + _Packet.add_version(v)
	end
	return r
end

function _Packet.eval(p)
	if p.type == 4 then return p.value end

	local acc = type2foo[p.type][2]
	local run = true
	for _,v in ipairs(p.sub) do
		run,acc = type2foo[p.type][3](acc, _Packet.eval(v))
		if not run then break end
	end
	return acc
end

function _M.part1(file)
	file = io.open("./day16.dat")
	local co = coroutine.create(function(n) iter(file, n) end)
	local p = _Packet.parse(co)
	-- packet.print(p)
	return _Packet.add_version(p)
end

function _M.part2(file)
	file = io.open("./day16.dat")
	local co = coroutine.create(function(n) iter(file, n) end)
	local p = _Packet.parse(co)
	-- _Packet.print(p)
	return _Packet.eval(p)
end

return _M
