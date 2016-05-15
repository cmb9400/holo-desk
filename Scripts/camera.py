import four_point_transform as fpt
import numpy as np
import cv2
import sys
import codecs
import traceback
import itertools

# Select video input; 0 for default video
# cap = cv2.VideoCapture(1)

def smooth(x,window_len=11,window='hanning'):
    """smooth the data using a window with requested size.
    
    This method is based on the convolution of a scaled window with the signal.
    The signal is prepared by introducing reflected copies of the signal 
    (with the window size) in both ends so that transient parts are minimized
    in the begining and end part of the output signal.
    
    input:
        x: the input signal 
        window_len: the dimension of the smoothing window; should be an odd integer
        window: the type of window from 'flat', 'hanning', 'hamming', 'bartlett', 'blackman'
            flat window will produce a moving average smoothing.

    output:
        the smoothed signal
        
    example:

    t=linspace(-2,2,0.1)
    x=sin(t)+randn(len(t))*0.1
    y=smooth(x)
    
    see also: 
    
    numpy.hanning, numpy.hamming, numpy.bartlett, numpy.blackman, numpy.convolve
    scipy.signal.lfilter
 
    TODO: the window parameter could be the window itself if an array instead of a string
    NOTE: length(output) != length(input), to correct this: return y[(window_len/2-1):-(window_len/2)] instead of just y.
    """ 
     
    if x.ndim != 1:
        raise ValueError("smooth only accepts 1 dimension arrays.")

    if x.size < window_len:
        raise ValueError("Input vector needs to be bigger than window size.")
        
    if window_len<3:
        return x
    
    if not window in ['flat', 'hanning', 'hamming', 'bartlett', 'blackman']:
        raise ValueError("Window is on of 'flat', 'hanning', 'hamming', 'bartlett', 'blackman'")

    s=np.r_[x[window_len-1:0:-1],x,x[-1:-window_len:-1]]
    #print(len(s))
    if window == 'flat': #moving average
        w=np.ones(window_len,'d')
    else:
        w=eval('np.'+window+'(window_len)')
    
    y=np.convolve(w/w.sum(),s,mode='valid')
    return y

TOP = 0; RIGHT = 1; BOTTOM = 2; LEFT = 3;
def checkEdge(image, edge, rows, threshold=True):
    """
    checkEdge is a function that takes in an image and an edge to check,
    and returns if there are values in that edge.

    image is the image to check for edges.

    edge is an integer corresponding to the following values:
    TOP = 0; RIGHT = 1; BOTTOM = 2; LEFT = 3;

    rows is an integer that is the number of rows to be checked from the edge.

    If threshold is an integer, then return true or false the
    summed rows are greater than the threshold.

    Otherwise, return an integer that is the value of the summed rows.
    """

    # rotate the image so the top row is the interesting one
    rotImg = np.rot90(image, edge)

    # crop the image so we can do a dumb sum
    cropImg = rotImg[:rows, :]

    edgeSum = np.sum(cropImg)

    if (threshold == True):
        return edgeSum
    return edgeSum > threshold

def findPoint(image, brokenEdge):
    """
    findPoint finds the coordinate opposite from the direction we broke

    image is the image we're scanning

    brokenEdge is the edge we broke with our arm
    """

    # determine the side to approach from,
    # depending on the side we broke
    pointEdge = (brokenEdge + 2) % 4

    # rotate the image so the top row is the interesting one
    rotImg = np.rot90(image, pointEdge)

    # sum across the rows
    sumRows = np.sum(rotImg, axis=1)

    # get the first index where there is a value
    nonZeroRows = np.nonzero(sumRows)[0]
    targetRow = nonZeroRows[0]

    # get the first index of that row from the rotated image
    targetColumn = np.nonzero(rotImg[targetRow])[0][0]

    # depending on the rotation, mutate the coords
    if ((pointEdge % 2) == 1):
        targetRow, targetColumn = targetColumn, targetRow
    if ((pointEdge == 1) or (pointEdge == 2)):
        targetColumn = np.shape(image)[1] - targetColumn
    if ((pointEdge == 2) or (pointEdge == 3)):
        targetRow = np.shape(image)[0] - targetRow

    #print("SHAPE: {0}".format(np.shape(image)))
    #print("POINT EDGE: {0}".format(pointEdge))

    return [targetRow, targetColumn]

def scaleCoord(coordinate, cameraWidth, cameraHeight, displayWidth, displayHeight):
    """
    transform coordinate to be appropriate for the camera size and display size
    returns new coordinates
    """
    newX = int(displayWidth * (coordinate[0]/cameraWidth))
    newY = int(displayHeight * (coordinate[1]/cameraHeight))

    return [newX, newY]

def my_print(s):
    with open('print_result.txt', mode='a') as f:
        f.write(str(s))

def absoluteTrack():
    while(True):
        # Capture frame-by-frame
        raw = sys.stdin.readline().strip().encode('utf8')
        hx = bytearray(codecs.decode(raw, 'hex'))
        nparr = np.array(hx)
        frame = cv2.imdecode(nparr, 0)
        # my_print(np.shape(frame))

        # Load in a test image
        #frame = cv2.imread('./test/kinect.png')

        # transform the image based on hardcoded numbers
        # because kinect has a fisheye lens
        # also the coordinates are unflipped at this point
        # (tl, tr, br, bl) = rect
        tl = (75, 49)
        tr = (475, 45)
        br = (515, 266)
        bl = (53, 278)
        transImg = fpt.four_point_transform(frame, np.array([tl, tr, br, bl]))
        clean = transImg
        # transform the image read into a grayscale image
        # gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

        # blur the image to reduce noise
        # TODO tweak blur
        transImg = cv2.medianBlur(transImg, 5)
        transImg = cv2.blur(transImg, (5,5))
        
        # flip image vertically
        transImg = cv2.flip(transImg, 0)

        # do our edge detection here
        # we do canny in this example, but this SHOULD BE TWEAKED based on your
        # application and real-world settings.
        # http://docs.opencv.org/2.4/modules/imgproc/doc/feature_detection.html
        cannyEdges = cv2.Canny(transImg, 50, 50)

        edges = cannyEdges
        debug = edges

        # determine what edge we are coming from
        # TODO: not pass a threshold value, and pick the largest one
        # NB: LEFT and RIGHT have significantly different values then TOP and BOTTOM
        # TODO: increase the threshold to prevent noise from triggering a detection
        coord = []
        targetEdge = -1
        threshold = 1000
        rowsToCheck = 15

        if ( checkEdge( edges, BOTTOM, rowsToCheck, threshold ) ):
            targetEdge = BOTTOM


        # if any edge was broken with some confidence, find a coordinate
        if (targetEdge > -1):
            coord = findPoint( edges, targetEdge )

            # Scale coordinate to be appropriate for the display
            scaledCoord = scaleCoord(coord, np.shape(transImg)[0], np.shape(transImg)[1], 1080, 1920)
            
            # Send the value through the chain, with swap
            yield (scaledCoord[1], scaledCoord[0]);

            # Display the resulting frame
            debug = cv2.cvtColor(edges,cv2.COLOR_GRAY2RGB)
            debug[coord[0]-5:coord[0]+5, coord[1]-5:coord[1]+5, 0] = 255
            brokenEdge = np.rot90(np.zeros(np.shape(debug)[:2]), targetEdge)
            brokenEdge[0:20, :] = 255
            brokenEdge = np.rot90(brokenEdge, 4 - targetEdge)
            debug[:, :, 1] = brokenEdge

        cv2.imshow('transform', transImg)
        cv2.imshow('cannyEdges', debug)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # When everything done, release the capture
    cap.release()
    cv2.destroyAllWindows()

def smoothTuple(points):
    """
    Smooths out a list of tupple xy valuess
    """
    s = 'hanning'
    x = [x for (x, y) in points]
    y = [y for (x, y) in points]
    xs = smooth(np.array(x), len(x), s)
    ys = smooth(np.array(y), len(y), s)
    smoothed = []
    for i in range(0, len(points)):
        smoothed.append((int(xs[i]), int(ys[i])))
    return smoothed

try: 
    backlog = 4
    dataQueue = []
    for point in absoluteTrack():
        # Keep a buffer of previous point data to smooth on
        dataQueue.append(point)
        if (len(dataQueue) < backlog):
            continue
        smoothedBacklog = smoothTuple(dataQueue)
        cleaned = smoothedBacklog.pop(0)
        # pop the value so that smoothing works accurately 
        dataQueue.pop(0)
        # subtract pixels to hover the cursor in front of the hand
        # also, because individual fingers sometimes get lost, this is a good approximate
        print(cleaned[0], cleaned[1]-75)
        sys.stdout.flush()

        # Show a scaled down version of the final transmitted positions
        blank_image = np.ones((1080//2 ,1920//2,3), np.uint8)
        cv2.circle(blank_image, (cleaned[0]//2,cleaned[1]//2), 5, (0,255,0), -1)
        cv2.imshow('output', blank_image)
        
except Exception as e:
    with open('err.txt', mode='w') as f:
        f.write(str(e))
        tb = traceback.format_exc()
        f.write(tb)
        f.flush()