from flask import Flask
from threading import Thread, Lock
from serial import Serial

web = Flask(__name__)

lock = Lock()
port = Serial('COM3')
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
    Serial.baudrate = 9600
    # Do one readline() to make sure that we don't try to read half a line
    Serial.readline()

    while True:

        line = Serial.readline()
        if (line[:2] == "//"):
            print(line)
        else:
            local_couner = int(line)
            lock.acquire()
            counter = local_couner
            lock.release()

thread = Thread(target=runSerial)
thread.run()

web.run()
