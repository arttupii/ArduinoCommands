#include<CommandParser.h>

Commands cmd;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  cmd.begin(Serial);
}

void loop() {
  cmd.handleInputCommands([](const char* cmdName, const char*p1, const char*p2, const char*p3, const unsigned char*binary, int size) {

    if(strcmp(cmdName,"SET") == 0) { //Handle SET command
      Serial.println("#RECEIVED SET:");
      Serial.println(cmdName);
      Serial.println(p1); //Print params
      Serial.println(p2);
      Serial.println(p3);
      Serial.println("Binary:"); //Print binary data
      for (int i = 0; i < size; i++) {
        Serial.print(binary[i], HEX);
        Serial.print(",");
      }
      //Send ack
      cmd.send("ACK");
    }
    else if(strcmp(cmdName,"GET") == 0) { //Handle GET command
      Serial.println("#RECEIVED GET:");
      
      unsigned char binary[] = {'H','E', 'L', 'L', 'O'};
      cmd.send("ACK", binary , sizeof(binary)); //SEND binary as reply
    } else {
      cmd.send("NACK", "INVALID COMMAND"); //Handle invalid command
    }
   });
}
