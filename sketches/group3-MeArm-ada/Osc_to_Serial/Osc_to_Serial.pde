import oscP5.*;
import netP5.*;
import processing.serial.*;

OscP5 oscP5;
Serial myPort;

void setup() {
  // Start OSC listener on port 12000
  oscP5 = new OscP5(this, 12002);
  
  // Initialize serial communication
  String[] portList = Serial.list();
  if (portList.length > 0) {
    myPort = new Serial(this, portList[0], 9600);
    println("Serial port opened: " + portList[0]);
  } else {
    println("No serial port available.");
  }
}

void draw() {
  // No active draw loop needed
  
  println("in draw");
}

void oscEvent(OscMessage theOscMessage) {
  if (theOscMessage.checkAddrPattern("/wek/inputs") == true) {
    println("soemthing is happening!!!!");
    // Expecting three float values from OSC
    if (theOscMessage.checkTypetag("fff")) { 
      float value1 = theOscMessage.get(0).floatValue();
      float value2 = theOscMessage.get(1).floatValue();
      float value3 = theOscMessage.get(2).floatValue();
      float value4 = random(80,120);
      
      // Formatting our message for the Arduino and sending it over serial
      String message = value1 + "," + value2 + "," + value3;
      myPort.write(message + '\n'); 
      println("Sent to Arduino: " + message);
    } else {
      println("Unexpected message format: " + theOscMessage.typetag());
    }
  }
}