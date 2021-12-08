local _M = {}

function _M.printTable(t)
	for k,v in pairs(t) do
		print(k,v)
	end
end

function _M.getSession(key, path)
	local i = 0
	for line in io.lines(path..".sess") do
		i = i + 1
		if i == key then
			return line
		end
	end
	error("key number " .. tonumber(key) .. " not found in .sess")
end

function _M.copyTable(t)
	local res = {}
	for i,v in ipairs(t) do
		table.insert(res, i, v)
	end
	return res
end

function _M.printStrings(t)
	if type(t) ~= "table" then print(t) return end
	for _,v in ipairs(t) do
		_M.printStrings(v)
	end
	print()
end

function _M.find(node, comp, acc, path)
	table.insert(path, node)
	if comp(node) then
		acc[node] = _M.copyTable(path)
	end
	for _,child in ipairs(node) do
		if type(child) == "table" then
			_M.find(child, comp, acc, path)
		end
	end
	table.remove(path)
end

return _M
