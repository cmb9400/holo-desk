
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

def toWriteOutString(leoCoords):
    """
    Convert a tuple of two integers to a string to send
    to the Arduino Leonardo in the format:

      xxxxxyyyyy.

    where xxxxx is a right-aligned x coordinate precceded by 0's,
	  yyyyy is a right-aligned y coordinate precceded by 0's,
    and a '.' terminates the string.
    """
    numZerosX = 5 - len(str(leoCoords[0]))
    numZerosY = 5 - len(str(leoCoords[1]))
    print(" Coords from Kinect are ", leoCoords, "numZerosX is ", numZerosX, "numZerosY is ", numZerosY, end="\t")
    return ('0'*numZerosX)+str(leoCoords[0])+('0'*numZerosY)+str(leoCoords[1])+'.'

print( toWriteOutString( (0,0) ) )
print( toWriteOutString( (1,1) ) )
print( toWriteOutString( (11,11) ) )
print( toWriteOutString( (111,111) ) )
print( toWriteOutString( (1111,1111) ) )
print( toWriteOutString( (11111,11111) ) )

print( toWriteOutString( (0,0) ) )
print( toWriteOutString( (0,1) ) )
print( toWriteOutString( (0,11) ) )
print( toWriteOutString( (0,111) ) )
print( toWriteOutString( (0,1111) ) )

print( toWriteOutString( (0,0) ) )
print( toWriteOutString( (1,0) ) )
print( toWriteOutString( (11,0) ) )
print( toWriteOutString( (111,0) ) )
print( toWriteOutString( (1111,0) ) )
print( toWriteOutString( (11111,0) ) )
