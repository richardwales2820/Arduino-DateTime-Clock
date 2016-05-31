"""
This python script references the calendar.py script, which contains code defining UI elements
created with Qt Designer, a program that allows easy, drag and drop GUI creation.

The UI elements are connected to python functions made here

Pyserial is used to communicate with the Arduino over serial
"""
import sys
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import *
from calendar import Ui_MainWindow
import serial
import serial.tools.list_ports
from time import sleep

"""
This class inherits from the QMainWindow class, A Qt framework class,
 and the Ui_MainWindow class, a class defined from the calendar.py script
"""
class App(QtWidgets.QMainWindow, Ui_MainWindow):

    #__init__ is called when an object of this class is instantiated (ignore the self and parent for now, its just python stuff)
    def __init__(self, parent=None):

        #Create a serial object from pyserial
        self.serial = serial.Serial()

        #Iterate the list of available ports and check if its tag matches |   [These tags, "Arduino" and "Generic" are just descriptions that 
        #the arduino's tag listed in Windows COM port (Arduino)           |    are given for the arduino in a windows and mac computer]
        #or Mac OSX /dev/ tree (Generic)                                  |
        for p in list(serial.tools.list_ports.comports()):
            if "Arduino" in p.description or "Generic" in p.description:
                self.serial.port = str(p.device)    #Set serial port to the Arduino port
                self.serial.timeout = 1             #timeout to 1 second, exits serial if no connection is made within this time
                self.serial.setDTR(False)           #DTR = false; ensures the Arduino does not reset upon making a connection
                self.serial.open()                  #Finally, opens the serial connection

        #Calls the inherited class' __init__ function to initialize UI, then
        #calls the setupUi function from it initialize more GUI things in calendar.py
        super(App, self).__init__(parent)
        self.setupUi(self)

        #Connects the button to a function defined below so it runs upon being clicked
        self.pushButton.clicked.connect(self.sendDate)

    def checkDate(self):
        date = self.calendarWidget.selectedDate()
        time = self.timeEdit.time()

        dayOfWeek = date.dayOfWeek()
        print(dayOfWeek)

    def sendDate(self):
        """
        This function takes in the date and time values from the calendar widget,
        takes each individual piece of data and converts it into a byte string
        encoded in latin-1 encoding to play nice with the Arduino (doesn't like unicode),
        then sends the bytes over serial
        """
        date = self.calendarWidget.selectedDate()
        time = self.timeEdit.time()

        dayOfWeek = date.dayOfWeek()
        print(dayOfWeek)
        
        secBytes = bytes(str(time.second()).encode(encoding="latin-1"))  
        minBytes = bytes(str(time.minute()).encode(encoding="latin-1"))
        hourBytes = bytes(str(time.hour()).encode(encoding="latin-1"))

        dayBytes = bytes(str(date.day()).encode(encoding="latin-1"))
        monthBytes = bytes(str(date.month()).encode(encoding="latin-1"))
        yearBytes = bytes(str(date.year()).encode(encoding="latin-1"))

        dayOfWeekBytes = bytes(str(dayOfWeek).encode(encoding="latin-1"))  

        """
        Before sending each string of bytes, a tag corresponding to a letter is sent.
        This allows the Arduino to search for a letter, then upon receiving that letter,
        prepare itself to receive a single string of bytes corresponding to an integer value.

        Keeps the data values separated from one another for successful parsing

        sleep(1) has the program wait 1 second between each send to ensure the Arduino finished parsing
        """        
        self.serial.write(str("A").encode(encoding="latin-1"))
        self.serial.write(secBytes)
        sleep(1)

        self.serial.write(str("B").encode(encoding="latin-1"))
        self.serial.write(minBytes)
        sleep(1)
        
        self.serial.write(str("C").encode(encoding="latin-1"))
        self.serial.write(hourBytes)
        sleep(1)

        self.serial.write(str("D").encode(encoding="latin-1"))
        self.serial.write(dayBytes)
        sleep(1)

        self.serial.write(str("E").encode(encoding="latin-1"))
        self.serial.write(monthBytes)
        sleep(1)
        
        self.serial.write(str("F").encode(encoding="latin-1"))
        self.serial.write(yearBytes)
        sleep(1)

        self.serial.write(str("G").encode(encoding="latin-1"))
        self.serial.write(dayOfWeekBytes)

def main():

    #Qt initialization stuff
    #Create an object of the class QApplication to host everything
    app = QtWidgets.QApplication(sys.argv)

    #form is an object of the class defined above
    form = App()

    #Show the UI elements
    form.show()

    #Exit the application when the Qt window closes
    sys.exit(app.exec_())

#run main() [python doesn't really have an int main() that always runs on start up]
main()