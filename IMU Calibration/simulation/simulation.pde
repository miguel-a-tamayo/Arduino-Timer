import processing.serial.*;

PShape shuttle; // initialize the PShape for space shuttle

Serial port; // initialize serial port to listen for Arduino
int port_num = 0; // change to correct port where arduino is connected to

String raw;
String[] split;
float rX = 0;
float rY = 0;
float rZ = 0;

void setup() {
  size(2000, 2000, P3D); // set up canvas size
  
  shuttle = loadShape("space-shuttle.obj"); // load space Shuttle object
  //println(Serial.list());
  
  String port_name = Serial.list()[port_num];
  port = new Serial(this, port_name, 9600);
}

void draw() {
  
  background(200);
  fixPos();
  rotateZ(PI - radians(rZ));
  rotateX(radians(rY));
  rotateY(PI + radians(rX));
  shape(shuttle);
  //println(rY);
}

void fixPos(){
  scale(2);
  translate(width/4, height/4, 745);
}

void serialEvent(Serial port) {
  raw = port.readStringUntil('\n');
  
  if (raw != null) {
    String[] split = raw.split(",", 0);
    println(float(split[0]) + " " + rX);
    rX = float(split[0]);
    rY = float(split[1]);
    rZ = float(split[2]);
  }
}
