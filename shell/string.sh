#!/bin/bash
var=string_operator
echo ${var:1}
echo ${var:0:1}
echo ${var:1:-1}
echo ${var:-1:3}
echo ${@:3:1}
