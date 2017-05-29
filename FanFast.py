import re
import setFan
import json

WORDS = ["FAN", "FAST"]

file = open("hostnamesFans.json")
for line in file:
    content = json.loads(line)
    hostname = content["fans"]


def handle(text, mic, profile):
    setFan.setFan(hostname, 0, "FAST")
    mic.say("Okay, I am turning the fan to a fast frequency")


def isValid(text):
    return bool(re.search(r'\bFAN\b', text, re.IGNORECASE))
