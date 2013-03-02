#!/bin/bash

declare -a arrays
declare -f function_name
declare -F display_function_names
declare -i integer
declare -r read_only
declare -x export_variable

integer=2
declare -i integer2=3
integer=integer*integer2
echo $integer
