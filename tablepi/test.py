
def toLeoCoords(xIn, yIn):
    """
    Our input is expected to be in the ranges:
      xIn: [0, 1920]
      yIn: [0, 1080]

    This function will scale these values to the system
    of the Arduino Leonardo in the ranges:
      x: [0, 32767]
      y: [0, 32767]

    returned as a tuple: (x, y)
    """
    newX = int(32767 * (xIn / 1920))
    newY = int(32767 * (yIn / 1080))
    return (newX, newY)

#while True:
#    kinectCoords = connection.readline()[:-1].split(' ')
    
print(toLeoCoords(0,0))
print(toLeoCoords(960,540))
print(toLeoCoords(1920,1080))


def toWriteOutString(leoCoords):
    numZerosX = 5 - len(str(leoCoords[0]))
    numZerosY = 5 - len(str(leoCoords[1]))
    return ('0'*numZerosX)+str(leoCoords[0])+('0'*numZerosX)+str(leoCoords[1])+'.'

print(toWriteOutString((0,0)))
print(toWriteOutString((32767,32767)))
