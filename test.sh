#!/bin/zsh

for test_case in \
  test_assignment_basic1.txt \
  test_assignment_basic2.txt \
  test_assignment_variables_no_init.txt \
  test_assignment_variables1.txt \
  test_assignment_variables2_operators.txt \
  test_control_i_if_f_i_if_f1.txt \
  test_control_i_if_f_i_if_f2.txt \
  test_control_i_if_f1.txt \
  test_control_i_if_f2.txt \
  test_control_i_if_f3.txt \
  test_control_i_if_f4.txt \
  test_control_i_if_f5.txt \
  test_control_i_if_fif1.txt \
  test_control_i_if_fif2.txt \
  test_control_if1.txt \
  test_control_if2.txt \
  test_control_if3_list.txt \
  test_control_if3_list2.txt \
  test_control_if4_list.txt \
  test_control_ifif1.txt \
  test_control_ifif2.txt \
  test_control_ifif3.txt \
  test_control_ifif4.txt \
  test_control_while1.txt \
  test_control_while2.txt \
  test_control_while3_fibonacci.txt \
  test_control_while4.txt \
  test_control_while5.txt \
  test_control_whilewhile_if1.txt \
  test_control_whilewhile6.txt \
  test_control_whilewhile7.txt \
  test_control_whilewhile8.txt \
  test_control_whilewhile9.txt
do
  echo "Running test case: $test_case"
  ./compiler < CSE340_Project3_Provided_Test_Cases/$test_case > output.txt
  if diff -bB output.txt CSE340_Project3_Provided_Test_Cases/$test_case.expected >/dev/null
  then
    echo "PASSED"
  else
    echo "FAILED"
  fi
done
