function c_test()
	local obj = A()
	obj:show()
	print("get:", obj:get())
	print("set:", obj:set(11))
end

function c_testInt()
	local val = 1314
	return val
end

function c_testStr()
	local str = "hello world"
	return str
end


