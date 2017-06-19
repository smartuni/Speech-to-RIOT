import re
import json

from subprocess import Popen, PIPE

WORDS = ["FAN", "MEDIUM"]


def handle(text, mic, profile):
    file = open("hostnamesFans.json")
    for line in file:
        content = json.loads(line)
        hostname = content["fans"]
    p = Popen(["python3", "/home/pi/jasper-client/client/modules/setFan.py", hostname, "0", "MEDIUM"], stdout=PIPE, bufsize=1)
    mic.say("Okay, I am turning the fan to a medium frequency")


def isValid(text):
    # textoutput for debugging
    print "---------------------------------printing text variable----------------------------------------"
    print text
    return bool(re.search(r'\bFAN MEDIUM\b', text, re.IGNORECASE))
