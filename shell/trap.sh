#!/bin/bash

loop() {
	trap "echo 'How dare you!'" INT	
}
loop
trap "echo 'You hit CTRL-C'" INT
trap "echo 'You try to kill me!'" TERM

while true; do
	sleep 600
done
