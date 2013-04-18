#!/usr/bin/bash

echo "echo 'hello' to /dev/mydev"
echo 'hello' > /dev/mydev
echo "cat /dev/mydev"
cat /dev/mydev
