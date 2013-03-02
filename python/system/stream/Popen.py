#!/usr/bin/env python3

from subprocess import Popen, PIPE, call

import os
p1 = os.popen('python3 pipefile.py')
print(p1.readline())
p1.close()
#X = call('python3 pipefile.py')
#pipe = Popen('python3 hello-out.py', stdout=PIPE)
#pipe.coummunicate()[0]
