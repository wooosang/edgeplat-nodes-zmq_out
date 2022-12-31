import json, sys, time, zmq, os, re
import logging
from datetime import datetime

from PyQt5.QtCore import QDataStream, QByteArray, QIODevice

index=0

def send_files(socks, path, cameraip):
    for filepath,dirnames,filenames in os.walk(path):
        for filename in filenames:
            send(socks, cameraip, filename)
            time.sleep(0.5)

def send(socks, cameraip, filename):

    global index
    exp = '(\d)-(\d+)-(\d+).jpg'
    pattern = re.compile(exp)
    if not filename.endswith("jpg"):
        return
    match = pattern.search(filename)
    if not match:
        logging.error("{} not valid!".format(filename))
        return
    face = match.group(1)
#    print(match.group(2))
    idx = match.group(3)
    data = QByteArray()
    stream = QDataStream(data, QIODevice.WriteOnly)
    body = dict()
    index = index + 1
    body['idx']=int(idx)
    body['face'] = face
    body['ip'] = cameraip
    body['cameraip'] = cameraip
    body['imageunix'] = float(datetime.now().timestamp()) * 1000000000
#    filename = "/" + str(index%2) + ".jpg"
    imagefile = path + '/' + filename
    body['file'] =  face + '/' + filename
    # print('send image.....' + imagefile)
    print('Send {}'.format(body))
    stream.writeQString(json.dumps(body))

    with open(imagefile, "rb") as img_file:
        bytes = img_file.read()
        stream.writeBytes(bytes)

    try:
        for sock in socks:
            sock.send(data, zmq.DONTWAIT)
        print('Send image succeed!')
    except:
        pass

#python3.9 mocksendimage.py /home/ubuntu/work/test/samples/0 5020,5030 192.168.10.20 0
#python3.9 mocksendimage.py path ports cameraip face
if __name__ == '__main__':
    context = zmq.Context(2)
    path = sys.argv[1]
    ports = sys.argv[2]
    cameraip = sys.argv[3]
    face = sys.argv[4]
    socks = []
    portlist = ports.split(',')
    for port in portlist:
        push_socket = context.socket(zmq.PUSH)
        push_socket.set_hwm(5)
        push_socket.bind("tcp://*:{}".format(port))
        print("Add port {}".format(port))
        socks.append(push_socket)
    while True:
        send_files(socks, path, cameraip)
#        send(socks, path, cameraip, face)
#        time.sleep(0.16)
