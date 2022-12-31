import zmq

context = zmq.Context(1)
pull = context.socket(zmq.PULL)
endpoint = "tcp://192.168.2.244:5550"
pull.connect(endpoint)
while True:
    data = pull.recv_json()
    print(data)
