import sys
import time
import group_controller_1 as gui
import socket
import threading
import queue

# QNX_IP = "169.254.158.67"
# WIN_IP = "169.254.106.144"

QNX_IP = "192.168.10.1"
WIN_IP = "192.168.10.2"

# QNX_IP = "127.0.0.1"
# WIN_IP = "127.0.0.1"

LOCAL_IP = WIN_IP
LOCAL_PORT = 5001

REMOTE_IP = QNX_IP
REMOTE_PORT = 5000

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

sock.bind((LOCAL_IP, LOCAL_PORT))

running = True
tx_queue = queue.Queue()

count = 0

global msg
msg = ""

global actualPosition
actualPosition = "0"

def receive_thread():
    global running

    while running:
        try:
            data, addr = sock.recvfrom(1024)
            global count
            count += 1
            if count == 10:
                count = 0
                # print("RX :", data.decode())
                global actualPosition
                actualPosition = data.decode()
                print("actualPosition: ", actualPosition)
                ui.label_actual_position.setText(actualPosition)
                # time.sleep(0.001)
        except:
            pass

def transmit_thread():

    global running

    while running:
        global msg
        # msg = input("TX : ")
        if msg == "exit":
            running = False
            break

        # sock.sendto(msg.encode(), (REMOTE_IP, REMOTE_PORT))

def pushButton_E_STOP_1_released():
    print("pushButton_E_STOP_1_released")
    global msg
    msg = "1 1"
    sock.sendto(msg.encode(), (REMOTE_IP, REMOTE_PORT))

def pushButton_CLEAR_E_STOP_1_released():
    print("pushButton_CLEAR_E_STOP_1_released")
    global msg
    msg = "2 1"
    sock.sendto(msg.encode(), (REMOTE_IP, REMOTE_PORT))

def pushButton_DISABLE_1_released():
    print("pushButton_DISABLE_1_released")
    global msg
    msg = "3 1"
    sock.sendto(msg.encode(), (REMOTE_IP, REMOTE_PORT))

def pushButton_CLEAR_FAULT_1_released():
    print("pushButton_CLEAR_FAULT_1_released")
    global msg
    msg = "4 1"
    sock.sendto(msg.encode(), (REMOTE_IP, REMOTE_PORT))

def pushButton_ENABLE_1_released():
    print("pushButton_ENABLE_1_released")
    global msg
    msg = "5 1"
    sock.sendto(msg.encode(), (REMOTE_IP, REMOTE_PORT))

def pushButton_READY_1_released():
    print("pushButton_READY_1_released")
    global msg
    msg = "6 1"
    sock.sendto(msg.encode(), (REMOTE_IP, REMOTE_PORT))

def pushButton_STATE_X_1_released():
    print("pushButton_STATE_X_1_released")
    global msg
    msg = "7 1"
    sock.sendto(msg.encode(), (REMOTE_IP, REMOTE_PORT))

def pushButton_STOP_1_released():
    print("pushButton_STOP_1_released")
    global msg
    msg = "8 1"
    sock.sendto(msg.encode(), (REMOTE_IP, REMOTE_PORT))

def pushButton_E_STOP_2_released():
    print("pushButton_E_STOP_2_released")
    global msg
    msg = "1 2"
    sock.sendto(msg.encode(), (REMOTE_IP, REMOTE_PORT))

def pushButton_CLEAR_E_STOP_2_released():
    print("pushButton_CLEAR_E_STOP_2_released")
    global msg
    msg = "2 2"
    sock.sendto(msg.encode(), (REMOTE_IP, REMOTE_PORT))

def pushButton_DISABLE_2_released():
    print("pushButton_DISABLE_2_released")
    global msg
    msg = "3 2"
    sock.sendto(msg.encode(), (REMOTE_IP, REMOTE_PORT))

def pushButton_CLEAR_FAULT_2_released():
    print("pushButton_CLEAR_FAULT_2_released")
    global msg
    msg = "4 2"
    sock.sendto(msg.encode(), (REMOTE_IP, REMOTE_PORT))

def pushButton_ENABLE_2_released():
    print("pushButton_ENABLE_2_released")
    global msg
    msg = "5 2"
    sock.sendto(msg.encode(), (REMOTE_IP, REMOTE_PORT))

def pushButton_READY_2_released():
    print("pushButton_READY_2_released")
    global msg
    msg = "6 2"
    sock.sendto(msg.encode(), (REMOTE_IP, REMOTE_PORT))

def pushButton_STATE_X_2_released():
    print("pushButton_STATE_X_2_released")
    global msg
    msg = "7 2"
    sock.sendto(msg.encode(), (REMOTE_IP, REMOTE_PORT))

def pushButton_STOP_2_released():
    print("pushButton_STOP_2_released")
    global msg
    msg = "8 2"
    sock.sendto(msg.encode(), (REMOTE_IP, REMOTE_PORT))

def horizontalScrollBar_1_sliderMoved(my_ui):
    print("horizontalScrollBar_1_sliderMoved: ", ui.horizontalScrollBar_1.sliderPosition())
    ui.label_horizontal_scroll_bar.setText(str(round(float(ui.horizontalScrollBar_1.sliderPosition()) / 10.0, 2)))
    global msg
    msg = "start," + str(ui.horizontalScrollBar_1.sliderPosition()) + ",end"
    sock.sendto(msg.encode(), (REMOTE_IP, REMOTE_PORT))

def pushButton_left_released():
    global msg
    d_button = 0.0
    try:
        d_button = 10.0 * int(ui.lineEdit_step_size.text())
    except:
        d_button = 0.0
    
    d_button = int(d_button)
    next_command = int(ui.horizontalScrollBar_1.sliderPosition() - d_button)
    if(next_command < ui.horizontalScrollBar_1.minimum()):
        next_command = ui.horizontalScrollBar_1.minimum()
    if(next_command > ui.horizontalScrollBar_1.maximum()):
            next_command = ui.horizontalScrollBar_1.maximum()

    print("pushButton_left_released, commanded: ", next_command)
    ui.label_horizontal_scroll_bar.setText(str(round(float(ui.horizontalScrollBar_1.sliderPosition()) / 10.0, 2)))
    ui.horizontalScrollBar_1.setSliderPosition(next_command)

    msg = "start," + str(next_command) + ",end"
    sock.sendto(msg.encode(), (REMOTE_IP, REMOTE_PORT))

def pushButton_right_released():
    global msg
    d_button = 0.0
    try:
        d_button = 10.0 * int(ui.lineEdit_step_size.text())
    except:
        d_button = 0.0
        
    d_button = int(d_button)
    next_command = int(ui.horizontalScrollBar_1.sliderPosition() + d_button)
    if(next_command < ui.horizontalScrollBar_1.minimum()):
        next_command = ui.horizontalScrollBar_1.minimum()
    if(next_command > ui.horizontalScrollBar_1.maximum()):
            next_command = ui.horizontalScrollBar_1.maximum()

    print("pushButton_left_released, commanded: ", next_command)
    ui.label_horizontal_scroll_bar.setText(str(round(float(ui.horizontalScrollBar_1.sliderPosition()) / 10.0, 2)))
    ui.horizontalScrollBar_1.setSliderPosition(next_command)

    msg = "start," + str(next_command) + ",end"
    sock.sendto(msg.encode(), (REMOTE_IP, REMOTE_PORT))

if __name__ == "__main__":
    rx = threading.Thread(target=receive_thread)
    tx = threading.Thread(target=transmit_thread)

    app = gui.QtWidgets.QApplication(sys.argv)
    MainWindow = gui.QtWidgets.QMainWindow()

    ui = gui.Ui_MainWindow()
    ui.setupUi(MainWindow)

    ui.pushButton_E_STOP_1.released.connect(pushButton_E_STOP_1_released)
    ui.pushButton_CLEAR_E_STOP_1.released.connect(pushButton_CLEAR_E_STOP_1_released)
    ui.pushButton_DISABLE_1.released.connect(pushButton_DISABLE_1_released)
    ui.pushButton_CLEAR_FAULT_1.released.connect(pushButton_CLEAR_FAULT_1_released)
    ui.pushButton_ENABLE_1.released.connect(pushButton_ENABLE_1_released)
    ui.pushButton_READY_1.released.connect(pushButton_READY_1_released)
    ui.pushButton_STOP_1.released.connect(pushButton_STOP_1_released)
    ui.pushButton_STATE_X_1.released.connect(pushButton_STATE_X_1_released)

    ui.pushButton_E_STOP_2.released.connect(pushButton_E_STOP_2_released)
    ui.pushButton_CLEAR_E_STOP_2.released.connect(pushButton_CLEAR_E_STOP_2_released)
    ui.pushButton_DISABLE_2.released.connect(pushButton_DISABLE_2_released)
    ui.pushButton_CLEAR_FAULT_2.released.connect(pushButton_CLEAR_FAULT_2_released)
    ui.pushButton_ENABLE_2.released.connect(pushButton_ENABLE_2_released)
    ui.pushButton_READY_2.released.connect(pushButton_READY_2_released)
    ui.pushButton_STOP_2.released.connect(pushButton_STOP_2_released)
    ui.pushButton_STATE_X_2.released.connect(pushButton_STATE_X_2_released)

    ui.horizontalScrollBar_1.sliderMoved.connect(horizontalScrollBar_1_sliderMoved)

    ui.pushButton_left.released.connect(pushButton_left_released)
    ui.pushButton_right.released.connect(pushButton_right_released)

    rx.start()
    tx.start()

    MainWindow.show()
    
    ret = app.exec_()
    running = False
    tx_queue.put("exit")

    rx.join()
    tx.join()
    sock.close()
    sys.exit(ret)
