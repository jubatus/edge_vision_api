from app import edge_vision_api

if __name__ == '__main__':
    edge_vision_api.debug = True
    edge_vision_api.run(debug=True, use_reloader=False)
