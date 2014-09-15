function c_test()
	--c_xxx()
	--print(hw)
	print(A)
	local pack = A()
	--print("-------->>>", getmetatable(A).show())
	pack:show()
end

function c_testInt()
	local val = 1314
	return val
end

function c_testStr()
	local str = "hello world"
	return str
end

