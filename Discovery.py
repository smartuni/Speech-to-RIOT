import re

from subprocess import Popen, PIPE

WORDS = ["SCAN", "FOR", "DEVICES"]


def handle(text, mic, profile):
    p = Popen(["python3", "/home/pi/jasper-client/client/modules/Scanner.py"], stdout=PIPE, bufsize=1)
    p.wait()
    mic.say("Okay, I have updated the devicelist.")


def isValid(text):
    return bool(re.search(r'\bSCAN FOR DEVICES\b', text, re.IGNORECASE))

