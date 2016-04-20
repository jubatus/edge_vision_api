# -*- coding: utf-8 -*-
from __future__ import print_function

from uuid import uuid4
import jubatus
from jubatus.common import Datum
import os
import random



import subprocess

from skimage.feature import hog
from skimage import data, color, exposure
from skimage.io import imread

host = "127.0.0.1"
port = 9199
name = "vision"

juba = subprocess.Popen("jubaclassifier -f ../jubaconfig.json -p {port}".format(port=port).split(" "))

def train(label, path):
    client = jubatus.Classifier(host, port, name)
    image = color.rgb2gray(imread(path))
    fd, hog_image = hog(image, orientations=8, pixels_per_cell=(16, 16),
                        cells_per_block=(1, 1), visualise=True)
    fd_list = fd.tolist()

    d = {}
    for i, hist in enumerate(fd_list):
        d[str(i)] = hist

    print("{l} => {p}".format(l=label, p=path))
    return

    juba.train([(label, Datum(d))])

label_dirs = filter(lambda x: x[:6] == "label_", os.listdir("."))

supervised_data = []
for dirname in label_dirs:
    label = dirname[6: ]
    images = filter(lambda x: x[-3:] == "jpg", os.listdir(dirname + "/"))
    supervised_data.extend(map(lambda x: (label, dirname + "/" + x), images))

random.shuffle(supervised_data)
for data in supervised_data:
    train(data[0], data[1])
