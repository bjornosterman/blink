from flask import Flask
from threading import Thread, Lock
from serial import Serial

web = Flask(__name__)

lock = Lock()
counter = 0


@web.route("/api")
def api():
    global counter
    local_counter = 0
    lock.acquire()
    local_counter = counter
    lock.release()
    return str(local_counter)


def runSerial():
    global counter
    port = Serial('COM6')
    print("Port" + str(port))
    # port.baudrate = 9600
    # Do one readline() to make sure that we don't try to read half a line
    port.readline()

    while True:

        line = port.readline()
        print(b"Line: " + line)
        if (line[:2] == b"//"):
            print(line)
        else:
            local_couner = int(line)
            lock.acquire()
            counter = local_couner
            lock.release()

thread = Thread(target=runSerial)
thread.start()

web.run()
