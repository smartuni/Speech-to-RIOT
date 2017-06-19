import re
import json

from subprocess import Popen, PIPE

WORDS = ["FAN", "OFF"]


def handle(text, mic, profile):
    file = open("hostnamesFans.json")
    for line in file:
        content = json.loads(line)
        hostname = content["fans"]

    p = Popen(["python3", "/home/pi/jasper-client/client/modules/setFan.py", hostname, "0", "OFF"], stdout=PIPE, bufsize=1)
    mic.say("Okay, I am turning the fan off")


def isValid(text):
    return bool(re.search(r'\bFAN OFF\b', text, re.IGNORECASE))
