for num in {11..19}
do
	#run test 11-19, compare the mtf files, use the output from cmp to echo message
	#\e[31(32) and the -e option allows it to chnage colours of the output
	python3 encode.py use_to_test/test$num.txt
	cmp use_to_test/test$num.mtf use_to_test/mtf_solutions/test$num.mtf
	if [ $? -ne 0 ]; then
		echo -e "\e[31mTest $num failed\e[0m";
	else
		echo -e "\e[32mTest $num passed\e[0m";
	fi
done
