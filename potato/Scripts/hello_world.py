import sys
import codecs
import cv2
import numpy as np
import traceback
import time

try:
    print("hello, world")

    with open("uhhh.txt", mode="w") as f:
        while True:
            raw = sys.stdin.readline().strip().encode('utf8')
            hx = bytearray(codecs.decode(raw, 'hex'))
            nparr = np.array(hx)
            frame = cv2.imdecode(nparr, 0)
            #with open("foo.png", mode="wb") as i:
            #    i.write(hx)
            cv2.imshow('gray', frame)
except Exception as e:
    with open('err.txt', mode='w') as f:
        f.write(str(e))
        tb = traceback.format_exc()
        f.write(tb)
        f.flush()
