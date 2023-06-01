--TEST--
Test array_group_pair() function : increasing subsequences
--FILE--
<?php
echo "*** Testing array_group_pair() : increasing subsequences ***\n";

function less_than_or_equal( $a, $b ) {
	return $a <= $b;
}

$arr1 = array(1,2,2,3,1,2,0,4,5,2);

var_dump( array_group_pair($arr1, 'less_than_or_equal') );

echo "Done";
?>
--EXPECT--
*** Testing array_group_pair() : increasing subsequences ***
array(4) {
  [0]=>
  array(4) {
    [0]=>
    int(1)
    [1]=>
    int(2)
    [2]=>
    int(2)
    [3]=>
    int(3)
  }
  [1]=>
  array(2) {
    [0]=>
    int(1)
    [1]=>
    int(2)
  }
  [2]=>
  array(3) {
    [0]=>
    int(0)
    [1]=>
    int(4)
    [2]=>
    int(5)
  }
  [3]=>
  array(1) {
    [0]=>
    int(2)
  }
}
Done
