#include "CommandParser.h"
#include <Arduino.h>
#include <safememcpy.h>

Commands::Commands() {
  index = 0;
  paramIndex = 0;
  callback = NULL;
  controlMark = false;
};
Commands::~Commands() {

};
void Commands::begin(Stream &stream) {
  this->stream = &stream;
}


void Commands::send(const char *reply, const char *p1, const char *p2, const unsigned char *data, const int length){
  send(reply, p1, p2, NULL, data, length);
}
void Commands::send(const char *reply, const char *p1, const unsigned char *data, const int length){
  send(reply, p1, NULL, NULL, data, length);
}
void Commands::send(const char *reply, const unsigned char *data, const int length){
  send(reply, NULL, NULL, NULL, data, length);
}

void Commands::send(const char *reply, const char *p1, const char *p2, const char *p3){
  send(reply, p1, p2, p3, NULL, NULL);
}
void Commands::send(const char *reply, const char *p1, const char *p2){
  send(reply, p1, p2, NULL, NULL, NULL);
}
void Commands::send(const char *reply, const char *p1){
  send(reply, p1, NULL, NULL, NULL, NULL);
}
void Commands::send(const char *reply){
  send(reply, NULL, NULL, NULL, NULL, NULL);
}



void Commands::send(const char *reply, const char *p1, const char *p2, const char *p3, const unsigned char *data, const int length) {
  stream->print(reply);

  if (p1!=NULL) {
    stream->print(" ");
    sendParameter(p1);
  }
  if (p2!=NULL) {
    stream->print(" ");
    sendParameter(p2);
  }
  if (p3!=NULL) {
    stream->print(" ");
    sendParameter(p3);
  }
  if (data!=NULL && length>0) {
    stream->print(" [");
    for (int i = 0; i < length; i++) {
      if (i != 0) {
        stream->print(",");
      }
      stream->print(data[i],HEX);
    }
    stream->print("]");
  }
  stream->print(";\n");
}

void Commands::handleInputCommands(void (cb)(const char* cmdName, const char*p1, const char*p2, const char*p3, const unsigned char*binary, int size)) {
  callback = cb;
  while (stream->available()) {
    parse(stream->read());
  }
}
  void Commands::sendParameter(const char*p) {
    for(int i=0;i<strlen(p);i++){
      if(p[i]==' '){
        stream->print("\\");
      }
      if(p[i]=='\\'){
        stream->print("\\");
      }
      stream->print(p[i]);
    }
  }
void Commands::parse(char a) {
  if(a=='\\' && controlMark==false) {
    controlMark = true;
    return;
  }

  buffer[index] = a;
  buffer[index + 1] = 0;

  if(controlMark) {
    index++;
    controlMark = false;
  } else {
    if(a=='#') {
      commentStarted = true;
      return;
    }

    if(commentStarted) {
      if (a == '\n' || a == '\r') {
        commentStarted=false;
      }
      return;
    }

    if (a == ' ' || a == '\t' || a == ';' && binaryIndex == 0) {
      buffer[index] = 0;
      if (paramIndex == 0) { //CMD NAME
        memcpyS(cmdName,sizeof(cmdName), buffer, sizeof(cmdName));
        cmdName[sizeof(cmdName) - 1] = 0;
        binaryIndex = 0;
        paramIndex++;
        param1[0] = 0;
        param2[0] = 0;
        param3[0] = 0;
      } else {
        //Handle params

        switch (paramIndex) {
          case 1: memcpyS(param1,sizeof(param1), buffer, sizeof(param1)); param1[sizeof(param1) - 1] = 0; break;
          case 2: memcpyS(param2,sizeof(param2), buffer, sizeof(param2)); param2[sizeof(param2) - 1] = 0; break;
          case 3: memcpyS(param3,sizeof(param3), buffer, sizeof(param3)); param3[sizeof(param3) - 1] = 0; break;
          default: break;
        }
        paramIndex++;
      }
      buffer[0];
      index = 0;
    } else if (a == '[') {
      index = 0;
      binaryIndex = 0;

    } else if (a == '\n' || a == '\r') {
    } else if (a == ']' || a == ',') {
      if (binaryIndex >= sizeof(binaryData)) {
        binaryIndex = sizeof(binaryData) - 1;
      }
      buffer[index] = 0;
      char *endptr;
      binaryData[binaryIndex] = (unsigned char)strtol(buffer, &endptr, 16);
      index = 0;
      binaryIndex++;
    } else {
      index++;
    }
    if (a == ';') {
      index = 0;
      callback(cmdName, param1, param2, param3, binaryIndex == 0 ? NULL : binaryData, binaryIndex);
      paramIndex = 0;
      binaryIndex = 0;
    }
  }
  if (index >= sizeof(buffer) - 1) {
    index = 0;
    binaryIndex = 0;
    param1[0] = 0;
    param2[0] = 0;
    param3[0] = 0;
  }
}
