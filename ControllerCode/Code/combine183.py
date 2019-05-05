from datetime import datetime
import RPi.GPIO as GPIO
import serial
import time
import pyrebase

# initialize GPIO
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.cleanup()


#firebase Setup
config = {
    "apiKey" : "AIzaSyCmn7MGManWVv4vQGtQ6kZEW-CO-dB4sI0",
    "authDomain" : "gardener-42d8e.firebaseapp.com",
    "databaseURL" : "https://gardener-42d8e.firebaseio.com",
    "projectId" : "gardener-42d8e",
    "storageBucket" : "gardener-42d8e.appspot.com",
    "messagingSenderId" : "756594341925"
  };

firebase = pyrebase.initialize_app(config);

db = firebase.database()

#global veriable
securityP = "0"
cameraP = "0"
dcheckP = "0"
flag1 = 0
flag2 = 0
dum = 0;
#Serial
ser1 = serial.Serial('/dev/ttyUSB0', 9600)


#Time veriable
checkSecHour1 = 89
checkSecMinute1 = 89

checkSecHour2 = 89
checkSecMinute2 = 89

checkSecHour3 = 89
checkSecMinute3 = 89

checkDailyHour1 = 89
checkDaileMinute1 = 89

checkDailyHour2 = 89
checkDaileMinute2 = 89

def timeset():
    global checkSecHour1
    global checkSecMinute1

    global checkSecHour2
    global checkSecMinute2

    global checkSecHour3
    global checkSecMinute3

    global checkDailyHour1
    global checkDaileMinute1

    global checkDailyHour2
    global checkDaileMinute2
    
    timeadd = db.child("TimeTable").child("DCMorning").get()
    time = timeadd.val()
    d = time.split(":")
    checkSecHour1 = d[0]
    checkSecMinute1 = d[1]

    timeadd = db.child("TimeTable").child("DCNoon").get()
    time = timeadd.val()
    d = time.split(":")
    checkSecHour2 = d[0]
    checkSecMinute2 = d[1]

    timeadd = db.child("TimeTable").child("DCAfternoon").get()
    time = timeadd.val()
    d = time.split(":")
    checkSecHour3 = d[0]
    checkSecMinute3 = d[1]

    timeadd = db.child("TimeTable").child("SecFirst").get()
    time = timeadd.val()
    d = time.split(":")
    checkDailyHour1 = d[0]
    checkDaileMinute1 = d[1]

    timeadd = db.child("TimeTable").child("SecSecond").get()
    time = timeadd.val()
    d = time.split(":")
    checkDailyHour2 = d[0]
    checkDaileMinute2 = d[1]
    print(checkDailyHour1+checkDaileMinute1)
    
def isSecTime():
    now = datetime.now()

    currentHour = now.hour
    currentMinute = now.minute
    currentSecond = now.second
    if (((str(currentHour) == str(checkSecHour1)) and (str(currentMinute) == str(checkSecMinute1))) or
            ((str(currentHour) == str(checkSecHour2)) and (str(currentMinute) == str(checkSecMinute2))) or
            ((currentHour == checkSecHour3) and (currentMinute == checkSecMinute3))):
        print("Ok")
        return True
    else:
        print(" Not Ok")
        return False
    
def isDCheckTime():
    now = datetime.now()
    currentHour = now.hour
    currentMinute = now.minute
    currentSecond = now.second
    print(str(currentHour)+str(checkDailyHour1)+str(currentMinute)+str(checkDaileMinute1))
    if (((str(currentHour) == str(checkDailyHour1)) & (str(currentMinute) == str(checkDaileMinute1))) |
            ((str(currentHour) == str(checkDailyHour2)) & (str(currentMinute) == str(checkDaileMinute2)))):
        
        return True
    else:
        print(" Not Ok")
        return False


while True:
    now = datetime.now()
    currentHour = str(now.hour)
    currentMinute = str(now.minute)
    currentSecond = str(now.second)
    print(currentHour+':'+currentMinute+':'+currentSecond)
   
    data = ser1.readline()
    print(data)
    d = data.split('/t')
    if(flag1==0):
        if(d[0]=='Tag'):
            cardAdd = db.child("cardNumber").get()
            rfCard = str(cardAdd.val())
            tagTree = db.child("Tree").get()
            for tTag in tagTree.each():
                tag = tTag.key()
                rfCard += str(tag)+"*"
            print(rfCard);
            ser1.write(rfCard)
            rfCard =""
            flag1=1;
            time.sleep(1)
        
    
    if(d[0]=="TreeTag"):
        treeTag = d[1];
        hum = d[2]
        ph = d[3]
        gas = d[4]
        wrDate = d[5]
        tree = db.child("Tree").get()
        for treeKey in tree.each():
            tKey = treeKey.key()
            if(tKey == treeTag):
                db.child("Tree").child(tKey).child("Hum").set(hum)
                db.child("Tree").child(tKey).child("ph").set(ph)
                db.child("Tree").child(tKey).child("gas").set(gas)
                db.child("Tree").child(tKey).child("wrDate").set(wrDate)
                print("Updated")
        
    if(d[0]=='set'):
        timeset()
        time.sleep(1)
        
        if isDCheckTime():
            print("isDCheckTime")
            dcheckP = "1"
            sigsend = cameraP + securityP + dcheckP
            ser1.write(sigsend)
            time.sleep(3)
            dcheckP = "0"
            isDCheckTime = False
        
        camera = db.child("camera").get()
        cameraP = str(camera.val())
        print(cameraP)
        if(cameraP == "1"):
            sigsend = cameraP + securityP + dcheckP
            print("Camera ON")
            ser1.write(sigsend)
            time.sleep(6)
                
        
        if isSecTime():
            print("isSecTime")
            securityP = "1"
            sigsend = cameraP + securityP + dcheckP
            ser1.write(sigsend)
            time.sleep(3)
            securityP = "0"
            isSecTime = True
        print("Wait")
        ser1.write("Wait")
