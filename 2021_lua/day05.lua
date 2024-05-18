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

local _M = {}

local function parseFile(file, pred)
	local ret = {}
	for line in file:lines() do
		local x1,y1 , x2,y2 = line:match("(%d+),(%d+) %-> (%d+),(%d+)")
		if pred(x1,y1 , x2,y2) then table.insert(ret, {x1=x1,y1=y1 , x2=x2,y2=y2}) end
	end
	return ret
end

local function sign(a)  return a<0 and -1 or 1 end

local part1 = {
	pred = function (x1,y1 , x2,y2) return x1 == x2 or y1 == y2 end,
	tick = function(board, v)
		if v.x1 == v.x2 then
			for i = v.y1,v.y2,sign(v.y2-v.y1) do
				local index = string.format("%d|%d", v.x1,i)
				if not board[index] then board[index] = 0 end
				board[index] = board[index] + 1
			end
		else
			for i = v.x1,v.x2,sign(v.x2-v.x1) do
				local index = string.format("%d|%d", i,v.y1)
				if not board[index] then board[index] = 0 end
				board[index] = board[index] + 1
			end
		end
	end
}

local part2 = {
	pred = function (x1,y1 , x2,y2) local b = (x1 == x2 or y1 == y2 or math.abs(x1-x2) == math.abs(y1-y2)) if not b then print("dropping") end return b end,
	tick = function(board, v)
		local x,y
		if v.x1 == v.x2 then
			x = function(i) return v.x1 end
		else
			x = function(i) return sign(v.x2 - v.x1)*i + v.x1 end
		end
		if v.y1 == v.y2 then
			y = function(i) return v.y1 end
		else
			y = function(i) return sign(v.y2 - v.y1)*i + v.y1 end
		end
		for i = 0,math.max(math.abs(v.x1-v.x2),math.abs(v.y1-v.y2)) do
			local index = string.format("%d|%d", x(i), y(i))
			if not board[index] then board[index] = 0 end
			board[index] = board[index] + 1
		end
	end
}

local function part(file, pred, tick)
	local board = {}
	local maxX,maxY = 0,0
	local vents = parseFile(file, pred)
	for _,v in pairs(vents) do
		if tonumber(v.x1) > maxX then maxX = tonumber(v.x1) end if tonumber(v.x2) > maxX then maxX = tonumber(v.x2) end
		if tonumber(v.y1) > maxY then maxY = tonumber(v.y1) end if tonumber(v.y2) > maxY then maxY = tonumber(v.y2) end
		tick(board, v)
	end
	local ret = 0
	for _,f in pairs(board) do
		if f >= 2 then ret = ret+1 end
	end
	return ret
end

function _M.part1(file) return part(file, part1.pred, part1.tick) end
function _M.part2(file) return part(file, part2.pred, part2.tick) end

return _M
