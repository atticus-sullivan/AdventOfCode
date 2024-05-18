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

local utils = require"aocutils"

local _M = {}

local function parse_file(file)
	file:read("l") -- skip border
	local board = {}
	local y = 0
	for line in file:lines() do
		local x = 0
		for c in line:gmatch(".") do
			if c ~= "#" and c ~= " " then
				local co = ("%d,%d"):format(x,y)
				board[co] = c
			end
			x = x + 1
		end
		y = y + 1
	end
	return {board=board}
end

local config = {
	["A"] = {sr=3, weight=1},
	["B"] = {sr=5, weight=10},
	["C"] = {sr=7, weight=100},
	["D"] = {sr=9, weight=1000},
}

local function serialize(b)
	local r = {{},{},{},{}}
	for co,a in pairs(b.board) do
		if a ~= "." then
			table.insert(r[(config[a].sr-1)/2], co)
			table.sort(r[(config[a].sr-1)/2])
		end
	end
	return utils.table.serialize(r)
end

local function _moves(b)
	local function _move(x_init,y_init, x_inc,y_inc)
		local x,y = x_inc, y_inc
		while true do
			local c = ("%d,%d"):format(x_init+x, y_init+y)
			if b.board[c] ~= "." then break end
			coroutine.yield(x,y)
			x = x + x_inc
			y = y + y_inc
		end
	end
	local function move(x_init,y_init, x_inc,y_inc)
		return coroutine.wrap(function() _move(x_init,y_init, x_inc,y_inc) end)
	end
	local function on_entrance(x)
		for _,c in pairs(config) do
			if c.sr == x then return true end
		end
		return false
	end
	local function yield_move(ox,oy, nx,ny, active)
		local oc = ("%d,%d"):format(ox, oy)
		local nc = ("%d,%d"):format(nx, ny)
		local b2 = utils.table.copy_deep(b)
		b2.key = nil

		b2.board[oc] = "."
		b2.board[nc] = active
		coroutine.yield(b2, (math.abs(ox-nx)+math.abs(oy-ny)) * config[active].weight)
		return b2
	end
	local function sr_valid(active)
		local x = config[active].sr
		local y = 1
		while true do
			local c = ("%d,%d"):format(x,y)
			if not b.board[c] then
				break
			end
			if b.board[c] ~= "." and b.board[c] ~= active then return false end
			y = y + 1
		end
		return true
	end

	for co,active in pairs(b.board) do
		if b.board[co] ~= "." then
			assert(config[active], "unknown char")
			local a_x,a_y = co:match("(%-?%d+),(%-?%d+)")
			a_x,a_y = tonumber(a_x), tonumber(a_y)
			if a_y == 0 and sr_valid(active) then
				-- currently in hallway
				local sr_succ = a_x == config[active].sr
				if not sr_succ then
					for x,_ in move(a_x,a_y, 1, 0) do
						if a_x+x == config[active].sr then
							sr_succ = true
							break
						end
					end
				end
				if not sr_succ then
					for x,_ in move(a_x,a_y, -1, 0) do
						if a_x+x == config[active].sr then
							sr_succ = true
							break
						end
					end
				end
				if sr_succ then
					local y_last = 0
					for _,y in move(config[active].sr, a_y, 0, 1) do
						y_last = y
					end
					if y_last > 0 then
						local b2 = yield_move(a_x,a_y, config[active].sr,a_y+y_last, active)
					end
				end
			elseif true then
			-- elseif config[active].sr ~= a_x then
				-- currently in wrong sideroom
				-- try to go to hallway
				local hw_succ = false
				for _,y in move(a_x,a_y, 0,-1) do
					if a_y+y == 0 then
						hw_succ = true
						break
					end
				end
				if hw_succ then
					-- check hallway positions
					-- go to right/left
					for _,inc in ipairs{-1,1} do
						for x,_ in move(a_x,0, inc,0) do
							-- check if on SR entrance
							if not on_entrance(a_x+x) then
								local b2 = yield_move(a_x,a_y, a_x+x,0, active)
							end
						end
					end
				end
			end
		end
	end
end
local function moves(b)
	return coroutine.wrap(function() _moves(b) end)
end

local function pr(b, y)
	y = y or 2
	b.mima = {minx=0, maxx=12, miny=0, maxy=y}
	utils.board.print(b, function(x) return x or "#" end)
end

local function dijkstra(init, idFunction, neighbours)
	local lut = {}
	local pq = {q={}, min=1000000000}
	utils.pq.insert(pq, init, 0)
	lut[idFunction(init)] = init

	while not utils.pq.empty(pq) do
		local u = utils.pq.delmin(pq)
		-- u will nver change from now on

		for b2,cost in neighbours(u) do
			-- path from u to b2 exists with cost
			b2 = lut[idFunction(b2)] or b2
			if not b2.key or b2.key > u.key + cost then
				if not b2.key then
					utils.pq.insert(pq, b2, u.key+cost)
					lut[idFunction(b2)] = b2
				else
					utils.pq.decKey(pq, b2, u.key+cost)
				end
			end
		end
	end
	return lut
end

function _M.part1(file)
	local src = parse_file(file)
	local dst = utils.table.copy_deep(src)
	for char,c in pairs(config) do
		dst.board[("%d,%d"):format(c.sr, 1)] = char
		dst.board[("%d,%d"):format(c.sr, 2)] = char
	end

	-- pr(src)
	-- print("\n")

	local lut = dijkstra(src, serialize,moves)
	return lut[serialize(dst)].key
end

function _M.part2(file)
	file = io.open("day23B.dat")
	local src = parse_file(file)
	local dst = utils.table.copy_deep(src)
	for char,c in pairs(config) do
		dst.board[("%d,%d"):format(c.sr, 1)] = char
		dst.board[("%d,%d"):format(c.sr, 2)] = char
		dst.board[("%d,%d"):format(c.sr, 3)] = char
		dst.board[("%d,%d"):format(c.sr, 4)] = char
	end

	-- pr(src, 4)
	-- print("\n")

	local lut = dijkstra(src, serialize,moves)
	return lut[serialize(dst)].key
end

-- TODO optimization: cap the maximum cost in dijkstra

return _M
