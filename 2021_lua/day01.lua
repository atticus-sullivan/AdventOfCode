local function part1(file)
	local ret = 0
	local n1,n2 = file:read("n"),file:read("n")
	while n2 do
		if n1 < n2 then ret = ret+1 end
		n1 = n2
		n2 = file:read("n")
	end
	file:close()
	return ret
end

---param File file
local function part2(file)
	local ret = 0
	local lastSum = nil
	local sums = {0, 0, 0}
	local n = file:read("n")
	sums[1] = n
	n = file:read("n")
	sums[2] = n
	sums[1] = sums[1] + n
	n = file:read("n")
	while n do
		sums[1] = sums[1] + n
		sums[2] = sums[2] + n
		sums[3] = sums[3] + n

		-- sum[1] is finished
		if lastSum and lastSum < sums[1] then ret = ret+1 end
		lastSum = sums[1]
		table.remove(sums, 1)
		sums[3] = 0
		n = file:read("n")
	end
	file:close()
	return ret
end

print("Part1:" .. part1(io.open("./day01.dat", "r")))
print("Part2:" .. part2(io.open("./day01.dat", "r")))
