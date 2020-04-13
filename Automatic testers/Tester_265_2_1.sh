for num in {0..10}
do
  #run test 0-11, compare the mtf files, use the output from cmp to echo the status
  #\e[31(32) allows the output color to change
  if [ $num -eq 10 ]; then
    python3 mtf2text.py use_to_test/test10.mtf
    diff use_to_test/test10.txt use_to_test/txt_solutions/test10.txt
    if [ $? -ne 0 ]; then
      echo -e "\e[31mTest $num failed\e[0m";
    else
      echo -e "\e[32mTest $num passed\e[0m";
    fi
  else
    python3 mtf2text.py use_to_test/test0$num.mtf
    diff use_to_test/test0$num.txt use_to_test/txt_solutions/test0$num.txt
    if [ $? -ne 0 ]; then
      echo -e "\e[31mTest 0$num failed\e[0m";
    else
      echo -e "\e[32mTest 0$num passed\e[0m";
    fi
  fi
done

#test invalid case
python3 mtf2text.py use_to_test/test10.txt
if [ $? -eq 1 ]; then
  echo -e "\e[32mInvalid test passed\e[0m";
else
  echo -e "\e[31mInvalid test failed\e[0m";
fi
