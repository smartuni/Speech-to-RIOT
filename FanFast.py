import re
import json

from subprocess import Popen, PIPE

WORDS = ["FAN", "FAST"]


def handle(text, mic, profile):
    file = open("hostnamesFans.json")
    for line in file:
        content = json.loads(line)
        hostname = content["fans"]
    p = Popen(["python3", "/home/pi/jasper-client/client/modules/setFan.py", hostname, "0", "FAST"], stdout=PIPE, bufsize=1)
    mic.say("Okay, I am turning the fan to a fast frequency")


def isValid(text):
    return bool(re.search(r'\bFAN FAST\b', text, re.IGNORECASE))
