# Final_semester_BE--IoT-Project
Remote Patient Monitoring with telemedicine based on IoT


The proposed system collects and transmits data from various sensors connected to the body of a patient that measure vital parameters of human body to a central network capable of comprehending, analyzing and processing the collected data. It comprises of a Microcontroller, Temperature sensor, Heart rate sensor GSM MODEM, Piezo-Electric buzzer, and Regulated Power Supply. 

A heart rate sensor which measures the blood volume in tissues will be used and temperature sensor will be utilized to read the body temperature of the patient. ECG sensor will also be incorporated to determine the electrical activity of the heart. The Microcontroller collects the data from the sensors and sends the data to the cloud. 

The data of vital parameters from the sensors is displayed on a web page and also on the LCD. The user interface html webpage is designed to automatically refresh for every fifteen seconds thereby ensuring constant monitoring of patient by the doctor. The information stored on the cloud can be accessed anytime by the authorized doctor using any browser from end devices such as laptop/mobile. The doctors can login to the webpage using a unique IP address and password assigned to them. The doctors can also suggest medications to the patient which will be sent to the patient’s phone achieving both remote patient monitoring and Tele-medicine integrated into a single system . 

When the vital parameters cross the safe threshold, to alert the doctor a message is sent to the doctor’s cell phone through GSM modem and simultaneously the buzzer turns on to alert the caretaker. If there is no response reception from the prescribed doctor within a stipulated time frame, the message is sent to the mobile of another doctor, making
the system more reliable.
