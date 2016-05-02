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

dirs = filter(lambda x: x[:6] == "label_", os.listdir("."))

data_list = []
for dirname in dirs:
    label = dirname[6:]
    jpgs = filter(lambda x: x[-5:] == ".jpeg", os.listdir(dirname))
    for jpg in jpgs:
        data_list.append([
            label,
            "{d}/{j}".format(d=dirname, j=jpg)
        ])
random.shuffle(data_list)

client = jubatus.Classifier("127.0.0.1", 9199, "test")

def gen_datum(filename):
    with open(filename, "rb") as f:
        binary = f.read()
        d = Datum()
        d.add_binary("img", binary)
    return d

k = 10

def train(entry):
    train_data = (entry[0], gen_datum(entry[1]))
    client.train([train_data])

def test(entry):
    res = client.classify([gen_datum(entry[1])])[0]
    res.sort(key=lambda x: x.score, reverse=True)
    #print("{a} predicted {r}".format(a=entry[0], r=res[0].label))
    return res[0].label

gok = 0
gng = 0
result = []

for i in range(k):
    client.clear()
    test_from = (len(data_list) * i / k)
    test_to   = (len(data_list) * (i + 1) / k)

    for j, entry in enumerate(data_list):
        if j <= test_from or test_to <= j:
            train(entry)
            print(".", end="")
            sys.stdout.flush()

    ok = 0
    ng = 0
    for j, entry in enumerate(data_list):
        if test_from < j and j < test_to:
            ret = test(entry)
            if ret == entry[0]:
                ok += 1
                print("o", end="")
            else:
                ng += 1
                print("x", end="")
            result.append({
                "filename": entry[1],
                "expect": entry[0],
                "answer": ret
            })
            sys.stdout.flush()
    print("")
    print("OK: {o}  NG: {n} of {t} data".format(o=ok, n=ng, t=ok+ng))
    gok += ok
    gng += ng

print("total OK: {o}  NG: {n} of {t} data".format(o=gok, n=gng, t=gok+gng))
print("accuracy {a} %".format(a=float(gok) * 100 / float(gok + gng)))

with open("detection_result.json", "w") as f:
    json.dump(result, f)
