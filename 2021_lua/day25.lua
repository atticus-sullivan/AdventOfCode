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
	local mima = {}
	local y = 0
	for line in file:lines() do
		local x = 0
		for c in line:gmatch(".") do
			local co = ("%d,%d"):format(x,y)
			if c ~= "." then
				board[co] = c
			end
			x = x + 1
		end
		mima.maxx = x-1
		y = y + 1
	end
	mima.maxy = y-1
	mima.minx,mima.miny = 0,0
	return {board=board, mima=mima}
end

local function move_dir(b, sym, x_diff,y_diff)
	local to_move = {}
	for co,e in pairs(b.board) do
		if e == sym then
			local x,y = co:match("(%-?%d+),(%-?%d+)")
			x,y = (tonumber(x)+x_diff)%(b.mima.maxx+1), (tonumber(y)+y_diff)%(b.mima.maxy+1)
			local co_new = ("%d,%d"):format(x,y)
			if not b.board[co_new] then
				to_move[co] = co_new
			end
		end
	end
	local r = false
	for co,co_new in pairs(to_move) do
		r = true
		b.board[co] = nil
		b.board[co_new] = sym
	end
	return r
end

function _M.part1(file)
	-- file = io.open("day25.dat.testing2")
	local board = parse_file(file)
	local i = 0
	-- utils.board.print(board, function(x) return x or "." end, {bool=false, pre=("Step %d"):format(i), pred=function(...) return true end})
	-- print()
	local running = true
	while running do
		running = move_dir(board, ">", 1,0)
		running = move_dir(board, "v", 0,1) or running
		i = i + 1
	end
	return i
end

function _M.part2(_)
	-- parse_file(file)
	-- print(file)
end

return _M
