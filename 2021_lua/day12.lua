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

local function print_paths(paths)
	for _,p in ipairs(paths) do
		for _,n in ipairs(p) do
			io.write(n, ",")
		end
		io.write("\n")
	end
end

local function search(caves, can_visit)
	local paths = {}
	local function searchR(curr, dst, path)
		table.insert(path, curr)
		if curr == dst then
			table.insert(paths, ShallowCopyTable(path))
		else
			for _,n in ipairs(caves[curr].conn) do
				if can_visit(caves, path, n) then
					searchR(n, dst, path)
				end
			end
		end
		table.remove(path)
	end
	searchR("start", "end", {})
	return paths
end

local can_visit = {
	part1 = function (caves, path, node)
		if caves[node].big then return true end
		for _,v in ipairs(path) do
			if node == v then return false end
		end
		return true
	end,
	part2 = function (caves, path, node)
		if caves[node].big then return true end
		local cnt = {}
		for _,v in ipairs(path) do
			if not caves[v].big then
				cnt[v] = cnt[v] and cnt[v] + 1 or 1
			end
		end
		if not cnt[node] then return true end
		if node == "start" then return false end
		for _,v in pairs(cnt) do
			if v >= 2 then return false end
		end
		return true
	end
}

local function part(file, can_visit)
	local caves = parse_file(file)
	local paths = search(caves, can_visit)
	return #paths
end

function _M.part1(file)
	return part(file, can_visit.part1)
end

function _M.part2(file)
	return part(file, can_visit.part2)
end

return _M
