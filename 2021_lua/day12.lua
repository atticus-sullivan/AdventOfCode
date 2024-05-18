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

local function parse_file(file)
	local caves = {}
	local function add_edge(s,e)
		if not caves[s] then caves[s] = s:match("[A-Z]+") and {big=true, conn={}} or {big=false, conn={}} end
		if not caves[e] then caves[e] = e:match("[A-Z]+") and {big=true, conn={}} or {big=false, conn={}} end
		table.insert(caves[s].conn, e)
		table.insert(caves[e].conn, s)
	end
	for line in file:lines() do
		add_edge(line:match("([a-zA-Z]+)%-([a-zA-Z]+)"))
	end
	return caves
end

local function print_path(path)
	io.write("{")
	for n,c in pairs(path) do
		if c > 0 then
			io.write(n, "->", c, " , ")
		end
	end
	io.write("}\n")
end

local function search(caves, can_visit)
	local paths = 0
	local function searchR(curr, dst, path)
		path[curr] = path[curr] and path[curr] + 1 or 1
		if curr == dst then
			paths = paths + 1
		else
			for _,n in ipairs(caves[curr].conn) do
				if can_visit(caves, path, n) then
					searchR(n, dst, path)
				end
			end
		end
		path[curr] = path[curr] - 1
	end
	searchR("start", "end", {})
	return paths
end

local can_visit = {
	part1 = function (caves, path, node)
		if caves[node].big then return true end
		return not path[node] or path[node] == 0
	end,
	part2 = function (caves, path, node)
		if caves[node].big then return true end
		local cnt = path

		if not cnt[node] or cnt[node] == 0 then return true end
		if node == "start" then return false end
		for k,v in pairs(cnt) do
			if v >= 2 and not caves[k].big then return false end
		end
		return true
	end
}

local function part(file, can_visit)
	local caves = parse_file(file)
	local paths = search(caves, can_visit)
	return paths
end

function _M.part1(file)
	return part(file, can_visit.part1)
end

function _M.part2(file)
	return part(file, can_visit.part2)
end

return _M
