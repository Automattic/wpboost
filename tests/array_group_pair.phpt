--TEST--
Test array_group_pair() function : basic functionality
--FILE--
<?php
echo "*** Testing array_group_pair() : basic functionality ***\n";

function less_than( $a, $b ) {
	return $a < $b;
}

function equal( $a, $b ) {
	return $a == $b;
}

function equal_obj( $a, $b ) {
	return $a->name == $b->name;
}

$arr1 = array(1, 2, 3);

echo "-- With an integer array for < --\n";
var_dump( array_group_pair($arr1, 'less_than') );

echo "-- With an integer array for == --\n";
var_dump( array_group_pair($arr1, 'equal') );

echo "-- With an empty array for == --\n";
var_dump( array_group_pair(array(), 'equal') );

echo "-- With a singleton integer array for == --\n";
var_dump( array_group_pair(array(1), 'equal') );

$obj1 = (object)array('id'=>3,'name'=>'foo');
$obj2 = (object)array('id'=>4,'name'=>'foo');
$obj3 = (object)array('id'=>5,'name'=>'baz');

echo "-- With an array of objects for == --\n";
var_dump( array_group_pair(array($obj1, $obj2, $obj3), 'equal_obj') );

echo "Done";
?>
--EXPECT--
*** Testing array_group_pair() : basic functionality ***
-- With an integer array for < --
array(1) {
  [0]=>
  array(3) {
    [0]=>
    int(1)
    [1]=>
    int(2)
    [2]=>
    int(3)
  }
}
-- With an integer array for == --
array(3) {
  [0]=>
  array(1) {
    [0]=>
    int(1)
  }
  [1]=>
  array(1) {
    [0]=>
    int(2)
  }
  [2]=>
  array(1) {
    [0]=>
    int(3)
  }
}
-- With an empty array for == --
array(0) {
}
-- With a singleton integer array for == --
array(1) {
  [0]=>
  array(1) {
    [0]=>
    int(1)
  }
}
-- With an array of objects for == --
array(2) {
  [0]=>
  array(2) {
    [0]=>
    object(stdClass)#1 (2) {
      ["id"]=>
      int(3)
      ["name"]=>
      string(3) "foo"
    }
    [1]=>
    object(stdClass)#2 (2) {
      ["id"]=>
      int(4)
      ["name"]=>
      string(3) "foo"
    }
  }
  [1]=>
  array(1) {
    [0]=>
    object(stdClass)#3 (2) {
      ["id"]=>
      int(5)
      ["name"]=>
      string(3) "baz"
    }
  }
}
Done
