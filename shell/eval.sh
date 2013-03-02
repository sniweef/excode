#!/bin/bash

listpage="ls | more"
$listpage
#eval $listpage

eval sort -nr \$1 ${2:+"| head -\$2"} 
#\是为了避免出现一些始料未及的事情(当变量含有像>\等特殊字符时)
