make
for num in {0..19}
do
	#run test 0-19, compare the mtf files, use the output from cmp to echo message
	#\e[31(32) and the -e option allows it to change colors of the output
	if [ $num -ge 10 ];
	then
		./mtf2text2 use_to_test/test$num.mtf
		diff use_to_test/test$num.txt use_to_test/txt_solutions/test$num.txt>output.txt
		if [ $? -ne 0 ]; then
			echo -e "\e[31mTest $num failed\e[0m";
		else
			echo -e "\e[32mTest $num passed\e[0m";
		fi
	else
		./mtf2text2 use_to_test/test0$num.mtf
		diff use_to_test/test0$num.txt use_to_test/txt_solutions/test0$num.txt
		if [ $? -ne 0 ]; then
			echo -e "\e[31mTest 0$num failed\e[0m";
		else
			echo -e "\e[32mTest 0$num passed\e[0m";
		fi
fi
done
