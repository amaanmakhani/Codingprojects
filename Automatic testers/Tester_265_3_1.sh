for num in {0..19}
do
	#run test 0-19, compare the mtf files, use the output from cmp to echo the status
	#\e[31(32) allows the output color to change
	if [ $num -ge 10 ]; then
		python3 decode.py use_to_test/test$num.mtf
		diff use_to_test/test$num.txt use_to_test/txt_solutions/test$num.txt>output.txt
		if [ $? -ne 0 ]; then
			echo -e "\e[31mTest $num failed\e[0m";
		else
			echo -e "\e[32mTest $num passed\e[0m";
		fi
	else
		python3 decode.py use_to_test/test0$num.mtf
		diff use_to_test/test0$num.txt use_to_test/txt_solutions/test0$num.txt
		if [ $? -ne 0 ]; then
			echo -e "\e[31mTest 0$num failed\e[0m";
		else
			echo -e "\e[32mTest 0$num passed\e[0m";
		fi
    fi
done

#test invalid case
python3 decode.py use_to_test/test10.txt>output.txt
if [ $? -eq 1 ]; then
	echo -e "\e[32mInvalid test passed\e[0m";
else
	echo -e "\e[31mInvalid test failed\e[0m";
fi
