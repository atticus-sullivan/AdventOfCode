---@version 5.4

---@class File
local File= {}

function File:close() end

function File:flush() end

---@vararg string "n" -> number; "a" -> whole file; "l" -> next line (no
---EOL); "L" -> next line (with EOL); number -> up to arg bytes
--- (Default: "l")
function File:lines(...) end

---@vararg string "n" -> number; "a" -> whole file; "l" -> next line (no
---EOL); "L" -> next line (with EOL); number -> up to arg bytes
function File:read(...) end

---param whence string "set"; "cur"; "end" (Default: "cur")
---param offset number (Default: 0)
---return number position in file beasued in bytes on success
function File:seek(whence, offset) end

---param mode string "no"; "full"; "line"
---param size number buffer size (Default is appropriate)
function File:setvbuf(mode, size) end

--vargs string|number write args to file
function File:write(...) end

---@type File
local x = {}
x:

---param File file
local function foo(file)
	file:
end
