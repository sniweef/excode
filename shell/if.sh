#!/bin/bash
test_if() {
	if cd a; then 
		echo hello world!
	elif cd program; then 
			echo hello
	else
		echo hello2
	fi
}
test_if 
