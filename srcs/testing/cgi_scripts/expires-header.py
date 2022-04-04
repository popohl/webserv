#!/bin/python3
import time, datetime

datetimeInFiveSeconds = datetime.datetime.fromtimestamp(time.time() + 15)

formattedDate = datetimeInFiveSeconds.strftime("%a, %d %b %Y %X %z")
print('Expires: ' + formattedDate)
print('')
print("hello world!");
