for num in {0..10}
do
#run test 0-11, compare the mtf files, use the output from cmp to echo message
#\e[31(32) and the -e option allows it to change colors of the output
  if [ $num -eq 10 ];
  then
    python3 text2mtf.py use_to_test/test10.txt
    cmp use_to_test/test10.mtf use_to_test/mtf_solutions/test10.mtf
    if [ $? -ne 0 ]; then
      echo -e "\e[31mTest $num failed\e[0m";
    else
      echo -e "\e[32mTest $num passed\e[0m";
    fi
  else
    python3 text2mtf.py use_to_test/test0$num.txt
    cmp use_to_test/test0$num.mtf use_to_test/mtf_solutions/test0$num.mtf
    if [ $? -ne 0 ]; then
      echo -e "\e[31mTest 0$num failed\e[0m";
    else
      echo -e "\e[32mTest 0$num passed\e[0m";
    fi
fi
done
