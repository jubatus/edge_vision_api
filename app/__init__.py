# -*- coding: utf-8 -*-

import logging

logging.basicConfig()
log = logging.getLogger()  # root logger
log.setLevel(logging.DEBUG)

from flask import Flask

edge_vision_api = Flask(__name__)

# version 0
from api.v0_api import api as v0api
edge_vision_api.register_blueprint(v0api)
