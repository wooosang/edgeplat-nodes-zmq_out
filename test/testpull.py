from PyQt5 import QtCore
import json,zmq

context = zmq.Context(1)
pull = context.socket(zmq.PULL)
endpoint = "tcp://192.168.2.244:5034"
pull.connect(endpoint)
while True:
    data = pull.recv()
    print(len(data))
    buf = QtCore.QByteArray.fromRawData(data)
    ds = QtCore.QDataStream(buf)
    msg_context = json.loads(ds.readQString())
    print(msg_context)
