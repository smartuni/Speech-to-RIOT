import re
import random
import client

WORDS = ["DEGREES"]


def handle(text, mic, profile):
    temperature_as_string = client.client()
    temperature_as_int = int(temperature_as_string)
    temperature_rounded = round((temperature_as_int/100.0), 2)
    mic.say("This room has a Temperature of %s degrees" % temperature_rounded)


def isValid(text):
    return bool(re.search(r'\bdegrees\b', text, re.IGNORECASE))
