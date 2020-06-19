from gtts import gTTS
import time
from playsound import playsound

# import os 
fin = open("workout.txt", 'r')

for line in fin:
    duration = line.split()[-1]
    message = line
    language = 'en'
    myobj = gTTS(text = message, lang = language, slow = False)

    myobj.save("workout.mp3")
    playsound("workout.mp3")
    time.sleep(int(duration))
