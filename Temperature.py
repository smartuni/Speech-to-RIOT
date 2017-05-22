import re
import random
import client
import json

WORDS = ["DEGREES"]


def handle(text, mic, profile):
    temperature_as_string = client.client()
    temperature_as_json = json.loads(temperature_as_string)
    temperature_as_int = temperature_as_json['temperature']
    temperature_rounded = round((temperature_as_int/100.0), 2)
    print(temperature_rounded)
    mic.say("This room has a Temperature of %s degrees" % temperature_rounded)


def isValid(text):
    return bool(re.search(r'\bdegrees\b', text, re.IGNORECASE))
