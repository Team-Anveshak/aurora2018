from math import *

lon1, lat1, lon2, lat2 = map(radians, [80.22903861,12.99057435,80.2319906, 12.99100858])
dlon = lon2 - lon1
dlat = lat2 - lat1
a = sin(dlat/2)**2 + cos(lat1) * cos(lat2) * sin(dlon/2)**2
c = 2 * atan2(sqrt(a), sqrt(1-a))
dist = 6371 * c*1000
print dist
