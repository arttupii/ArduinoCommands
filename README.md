Simple command parser for Arduino

#### Features:
 - Max length of the parameter: 10byte
 - Max length of the command name: 10byte
 - Max count of the parameters: 3 (+binary data)
 - Max binary data size: 240byte
 - Sender&Receiver
 - General stream support

#### Examples:
```
SET param1 param2 [1,2,3,4,5,6,7,ff,aa,12,1,34];
ACK param1 param2;
SET param1 param2 param3;
RESET;
SET param1;
SET [1,3,ff,ee];
#Comment line
SET Hi\ world; 
SET param\\1; 
```

## Test code
```c++
#include<CommandParser.h>

Commands cmd;

void setup() {
  Serial.begin(9600);

  cmd.begin(Serial);
}

void loop() {
  cmd.handleInputCommands([](const char* cmdName, const char*p1, const char*p2, const char*p3, const unsigned char*binary, int size) {

    if(strcmp(cmdName,"SET") == 0) { //Handle SET command
      Serial.println("#RECEIVED SET:");
      Serial.println(cmdName);
      Serial.println("Params:"); 
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
```
### Output

Send set command: "SET param1 param2 param3 [1,2,3,4,5,6,7];"
```
15:16:00.956 -> #RECEIVED SET:
15:16:00.956 -> SET
15:16:00.956 -> Params:
15:16:00.956 -> param1
15:16:00.956 -> param3
15:16:00.989 -> param3
15:16:00.989 -> Binary:
15:16:00.989 -> 1,2,3,4,5,6,7,
```
Send get command: "GET;"
```
15:20:39.505 -> #RECEIVED GET:
15:20:39.505 -> ACK [48,45,4C,4C,4F];
```
Send unkown command: "BLAA;"
```
15:22:33.488 -> NACK INVALID\ COMMAND;
```
