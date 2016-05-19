import sys
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import *
from calendar import Ui_MainWindow
import serial
import serial.tools.list_ports
from time import sleep

class App(QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__(self, parent=None):
        self.serial = serial.Serial()

        for p in list(serial.tools.list_ports.comports()):
            if "Arduino" in p.description:
                self.serial.port = str(p.device)
                self.serial.timeout = 1
                self.serial.setDTR(False)
                self.serial.open()


        super(App, self).__init__(parent)
        self.setupUi(self)

        self.pushButton.clicked.connect(self.sendDate)

    def sendDate(self):
        date = self.calendarWidget.selectedDate()
        time = self.timeEdit.time()

        #secBytes = bytes(time.second())
        secBytes = bytes(str(time.second()).encode(encoding="latin-1"))
        
        minBytes = bytes(str(time.minute()).encode(encoding="latin-1"))
        hourBytes = bytes(str(time.hour()).encode(encoding="latin-1"))

        dayBytes = bytes(str(date.day()).encode(encoding="latin-1"))
        monthBytes = bytes(str(date.month()).encode(encoding="latin-1"))
        yearBytes = bytes(str(date.year()).encode(encoding="latin-1"))
        """
        print(secBytes)
        self.serial.write(secBytes)
        
        """
        #print(len(secBytes))
        #print(secBytes)
        #print(bytes(30))
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
       

def main():
    app = QtWidgets.QApplication(sys.argv)
    form = App()
    form.show()
    sys.exit(app.exec_())

main()