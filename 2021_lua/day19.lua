-- Advent Of Code 2021
-- Copyright (C) 2024  Lukas Heindl
-- 
-- This program is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
-- 
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
-- 
-- You should have received a copy of the GNU General Public License
-- along with this program.  If not, see <http://www.gnu.org/licenses/>.

local utils  = require("aocutils")
local linalg = require("linAlg")

local _M = {}
local threshold = 12

Beacon = {}
function Beacon:new(o) -- key/value passing
	if getmetatable(o) == Beacon then
		local o_ = {}
		for i,v in ipairs(o) do o_[i] = v end
		o_.dist = o.dist
		o = o_
	end
	o = o or {dist={}}
	o.dist = o.dist or {}
	setmetatable(o, self) -- use the same metatable for all objects
	self.__index = self -- lookup in class if key is not found
	return o
end
function Beacon:set_distance(b)
	assert(getmetatable(b) == Beacon, "distance only works with a second beacon")
	local d = 0
	assert(#b == #self, "dimensions must match to set the distance")
	for i=1,#b do d = d + (self[i]-b[i])^2 end
	self.dist[d] = (self.dist[d] or 1) + 1
	b.dist[d]    = (b.dist[d]    or 1) + 1
end
function Beacon:common_dist(b)
	assert(getmetatable(b) == Beacon, "distance only works with a second beacon")
	local r = 0
	for d,v in pairs(self.dist) do
		if b.dist[d] then
			r = r + math.min(v, b.dist[d])
		end
	end
	return r
end
function Beacon:translate(h, o)
	local c,n = {}, #h
	for i=1,n do
		for j = 1, n do
			c[i] = (c[i] or 0) + h[i][j]*self[j]
		end
		c[i] = c[i] + (o and o[i] or 0)
	end
	for i,v in ipairs(c) do self[i] = v end
end
function Beacon:print()
	io.write(table.concat(self, ","), "\t")
	for d,i in pairs(self.dist) do
		io.write(", ", d, "|", i)
	end
	io.write("\n")
end
function Beacon:tovector()
	return utils.table.copy_shallow(self)
end
function Beacon:cs()
	return table.concat(self, ",")
end

Scanner = {}
function Scanner:new(o) -- key/value passing
	if getmetatable(o) == Scanner then
		o = {num=o.num, beacons=o.beacons}
	end
	o = o or {num=nil, beacons={}}
	setmetatable(o, self) -- use the same metatable for all objects
	self.__index = self -- lookup in class if key is not found
	o:fill_dist()
	return o
end
function Scanner:fill_dist()
	for i=1,#self.beacons do
		for j=i+1,#self.beacons do
			self.beacons[i]:set_distance(self.beacons[j])
		end
	end
end
function Scanner:common_dist(o)
	local r1,r2 = 0,{}
	for _,b1 in pairs(self.beacons) do
		local found = 0
		for _,b2 in pairs(o) do
			local r = b1:common_dist(b2)
			-- print(r)
			if r >= (threshold-1)*2 then
				r1 = r1+1
				table.insert(r2, {b1, b2})
				if found > 0 then print("WARNING: multiple matching distances") end
				found = found + found
				-- break -- hopefully only one matches
			end
		end
	end
	return r1,r2
end
function Scanner:translate(h, o)
	for _,b in ipairs(self.beacons) do
		b:translate(h, o)
	end
	self.pos = o
end
function Scanner:print()
	io.write("num: ", self.num, "\n")
	for _,b in ipairs(self.beacons) do
		b:print()
	end
end

local function parse_file(file)
	local r = {}
	while true do
		local line = file:read("l")
		if not line then break end
		local num = line:match("%-%-%- scanner (%d+) %-%-%-")
		num = tonumber(num)

		line = file:read("l")
		local beacons = {}
		while line and line ~= "" do
			local b = {}
			for x in line:gmatch("%-?%d+") do
				table.insert(b, x)
			end
			table.insert(beacons, Beacon:new(b))
			line = file:read("l")
		end
		table.insert(r, Scanner:new{num=num, beacons=beacons})
	end
	return r
end

local function match_beacons(pair, n)
	local h,a,o = {}, {}, {}
	for i = 1, n do
		h[i] = {}
		a[i] = {}
	end
	a[n+1] = {}

	for i=1,n do
		for j = 1, n+1 do
			-- insert pair.first
			a[j][i] = pair[j][1][i]
		end
	end
	for i=1,n+1 do
		a[i][n+1] = 1
	end
	local l,u = linalg.solver.lu_factorization(a)

	local b,res = {},{}

	for j = 1, n do
		for i = 1, n+1 do
			-- insert pair.second
			b[i] = pair[i][2][j]
		end
		res = linalg.solver.solve_lu(nil, b, l,u)
		for i = 1, n do
			h[j][i] = res[i]
		end
		o[j] = res[n+1]
	end

	-- round to integers
	for i=1,#h do
		for j=1,#h do
			h[i][j] = math.floor(h[i][j] + 0.5)
		end
		o[i] = math.floor(o[i] + 0.5)
	end
	return h,o
end

Map = {}
function Map:new(o) -- key/value passing
	if getmetatable(o) == Map then
		o = {beacons=o.beacons}
	end
	o = o or {beacons={}}
	setmetatable(o, self) -- use the same metatable for all objects
	self.__index = self -- lookup in class if key is not found
	return o
end
function Map:add_beacons(beacon, avoid_recalc)
	for _,b in ipairs(beacon) do
		if not self.beacons[b:cs()] then
			if not avoid_recalc then
				b.dist = {}
				for _,b2 in pairs(self.beacons) do
					b:set_distance(b2)
				end
			end
			self.beacons[b:cs()] = b
		end
	end
end
function Map:cnt()
	return utils.set.len(self.beacons)
end
function Map:print()
	for _,b2 in pairs(self.beacons) do
		b2:print()
	end
end

local function manhatten(v,w)
	local r = 0
	for i,_ in ipairs(v) do
		r = r + math.abs(v[i] - w[i])
	end
	return r
end

function _M.part1(file)
	-- file = io.open("./day19.dat.testing")
	local open = parse_file(file)
	local map = Map:new()
	local t = table.remove(open, 1)
	t.pos = {0,0,0}
	local solved  = {t}
	-- print(t.num)
	map:add_beacons(t.beacons, true)
		-- print()
		-- beacons_print(beacons)
		-- print()
	while #open > 0 do
		local switch = {}
		for i,o in pairs(open) do
			local d,pair = o:common_dist(map.beacons)
			-- print(d)
			-- o:print()
			if d >= threshold then
				local h,off = match_beacons(pair, 3)
				-- print(o.num)
				-- o:print()
				-- print()
				o:translate(h, off)
				-- linalg.vector.print(off)
				-- linalg.matrix.print(h, "h")
				-- print()
				-- o:print()
				table.insert(switch, i)
				map:add_beacons(o.beacons)
			end
		end
		table.sort(switch, function(a,b) return a > b end)
		for _,s in ipairs(switch) do
			table.insert(solved, open[s])
			table.remove(open, s)
		end
		-- print()
		-- beacons_print(beacons)
		-- print()
	end
	local max = 0
	for _,s1 in ipairs(solved) do
		for _,s2 in ipairs(solved) do
			local m = manhatten(s1.pos, s2.pos)
			if m > max then max=m end
		end
	end
	assert(#open == 0, "still something open")
	return map:cnt(), max
end

return _M
