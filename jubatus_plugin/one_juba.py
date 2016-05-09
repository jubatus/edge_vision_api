# -*- coding: utf-8 -*-
from __future__ import unicode_literals
from __future__ import print_function

host = '127.0.0.1'
port = 9199
name = 'test'

import sys, os
import json
import random

import jubatus
from jubatus.common import Datum

def gen_datum(filename):
    with open(filename, "rb") as f:
        binary = f.read()
        d = Datum()
        d.add_binary("img", binary)
    return d

client = jubatus.Classifier("127.0.0.1", 9199, "test")
print(sys.argv)

client.train([("label", gen_datum(sys.argv[1]))])
