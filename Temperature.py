import re
import random
import json

from subprocess import Popen, PIPE

WORDS = ["DEGREES"]


def handle(text, mic, profile):
    temperature_as_string = "42"
    p = Popen(["python3", "/home/pi/jasper-client/client/modules/client.py"], stdout=PIPE, bufsize=1)
    with p.stdout:
        for line in iter(p.stdout.readline, b''):
            temperature_as_string = str(line)[3:-3]
            print "Step 1, check"
            print 'Temperature_as_string: ' + temperature_as_string
    #p.wait() # wait for the subprocess to exit
    #temperature_as_string = client.client()
    #regex = r'.*(\d*).*'
    #matches = re.match(regex, str(temperature_as_string))
    #resultnumber = matches.group(0)
    #resultstring = '{"temperature": ' + str(resultnumber) + '}'
    #print resultstring
    print "loading json"
    temperature_as_json = json.loads(temperature_as_string)
    print "json loaded"
    temperature_as_int = temperature_as_json['temperature']
    temperature_rounded = round((temperature_as_int/100.0), 2)
    print temperature_rounded
    mic.say("This room has a Temperature of %s degrees" % temperature_rounded)


def isValid(text):
    return bool(re.search(r'\bdegrees\b', text, re.IGNORECASE))
