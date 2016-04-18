# -*- coding: utf-8 -*-
from __future__ import print_function

from flask import Blueprint, request, jsonify, Response, url_for
import os
from uuid import uuid4
import jubatus
from jubatus.common import Datum

import hashlib
import subprocess

api = Blueprint('v0', __name__, url_prefix="/v0")
ALLOWED_EXTENSIONS = set(['png', 'jpg', 'jpeg'])

host = "127.0.0.1"
port = 9199
name = "vision"

juba = subprocess.Popen("jubaclassifier -f jubaconfig.json -p {port}".format(port=port).split(" "))

@api.route("/predict", methods=["post"])
def predict():
    assert(request.method == 'POST')

    handler = request.files['file']
    ext = os.path.splitext(handler.filename)
    filename = str(uuid4()) + ext[-1]
    handler.save(os.path.join("./tmp", filename))

    client = jubatus.Classifier(host, port, name)
    with open(os.path.join("./tmp", filename)) as f:
        ret = client.classify([Datum({"image": f.read()})])
        print(ret)

    return jsonify({
            "status": "ok",
            "result": {
                "front": 1.0,
                "back": 0.2
            }
    })
