#!/bin/bash
for filename in benchmarks/*.php; do
	echo Executing benchmarks for $filename
	echo --------------------
	php -d extension_dir=modules -d extension=wpboost.so $filename
done
