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
	local board = {}
	local mima = {maxx=0, maxy=0, minx=0, miny=0}

	local enhance = {}
	local line = "xyz"
	while line ~= "" do
		line = file:read("l")
		for c in line:gmatch(".") do
			table.insert(enhance, c == "#")
		end
	end

	local y = 0
	for line in file:lines() do
		local x = 0
		for c in line:gmatch(".") do
			if c == "#" then
				local co = ("%d,%d"):format(x,y)
				board[co] = true
			end
			x = x + 1
		end
		mima.maxx = x-1
		y = y + 1
	end
	mima.maxy = y-1

	return {mima=mima, board=board}, enhance
end

local function enhance_img(b, enhance, default)
	local epsilon = 2
	local function to_bin(x,y)
		local r = {}
		for j=y-1,y+1 do
			for i=x-1,x+1 do
				local v
				if utils.board.in_range(b, i,j) then
					v = utils.board.get(b, i,j)
				else
					v = default
				end
				table.insert(r, v and 1 or 0)
			end
		end
		return tonumber(table.concat(r), 2)
	end
	local changed = false
	local r_b = {board={}, mima=utils.table.copy_deep(b.mima)}
	for y = b.mima.miny-epsilon, b.mima.maxy+epsilon do
		for x = b.mima.minx-epsilon, b.mima.maxx+epsilon do
			local ind = to_bin(x,y)
			changed = changed or utils.board.get(r_b, x,y) ~= enhance[ind-1]
			utils.board.set(r_b, x,y, enhance[ind+1], true)
		end
	end
	return r_b, changed
end

-- Thanks to https://www.juliabloggers.com/advent-of-code-2021-day-20/
-- I noticed that infinity possibly swaps between true and false with the maps
function _M.part1(file)
	-- file = io.open("day20.dat.testing")
	local board,enhance = parse_file(file)
	for i=1,2 do
		-- print()
		-- utils.board.print(board, function(x) return x and "#" or "." end)
		board = enhance_img(board, enhance, i%2 == 0 and enhance[1])
	end
	-- print()
	-- utils.board.print(board, function(x) return x and "#" or "." end)

	return utils.board.count(board, function(x) return x end)
end

function _M.part2(file)
	-- file = io.open("day20.dat.testing")
	local board,enhance = parse_file(file)
	local changed
	for i=1,50 do
		-- print()
		-- utils.board.print(board, function(x) return x and "#" or "." end)
		board,changed = enhance_img(board, enhance, i%2 == 0 and enhance[1])
		if not changed then break end
	end
	-- print()
	-- utils.board.print(board, function(x) return x and "#" or "." end)

	return utils.board.count(board, function(x) return x end)
end

return _M
