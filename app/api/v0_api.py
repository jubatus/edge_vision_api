# -*- coding: utf-8 -*-
from __future__ import print_function

from flask import Blueprint, request, jsonify, Response, url_for
import os
from uuid import uuid4
import jubatus
from jubatus.common import Datum

import hashlib
import subprocess

ALLOWED_EXTENSIONS = set(['png', 'jpg', 'jpeg'])

from skimage.feature import hog
from skimage import data, color, exposure
from skimage.io import imread

host = "127.0.0.1"
port = 9199
name = "vision"

juba = subprocess.Popen("jubaclassifier -f jubaconfig.json -p {port}".format(port=port).split(" "))

api = Blueprint('v0', __name__, url_prefix="/v0")

@api.route("/predict", methods=["post"])
def predict():
    assert(request.method == 'POST')

    handler = request.files['file']
    ext = os.path.splitext(handler.filename)
    filename = str(uuid4()) + ext[-1]
    filepath = os.path.join("./tmp", filename)
    handler.save(filepath)

    client = jubatus.Classifier(host, port, name)
    image = color.rgb2gray(imread(filepath))
    fd, hog_image = hog(image, orientations=8, pixels_per_cell=(16, 16),
                        cells_per_block=(1, 1), visualise=True)
    fd_list = fd.tolist()
    print(fd_list)

    d = {}
    for i, hist in enumerate(fd_list):
        d[str(i)] = hist

    return jsonify({
            "status": "ok",
            "result": client.classify([Datum(d)])[0]
    })
