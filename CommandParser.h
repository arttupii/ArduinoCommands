#ifndef __COMMAND_PARSER_H_
#define __COMMAND_PARSER_H_

#define MAX_CMD_PARAM_LENGTH 10
#define MAX_BINARY_PARAM_LENGTH 240

#include <Stream.h>
class Commands {
  public:
    Commands();
    ~Commands();
    void begin(Stream &stream);
    void send(const char *reply, const char *p1, const char *p2, const char *p3, const unsigned char *data, const int length);
    void send(const char *reply, const char *p1, const char *p2, const unsigned char *data, const int length);
    void send(const char *reply, const char *p1, const unsigned char *data, const int length);
    void send(const char *reply, const unsigned char *data, const int length);

    void send(const char *reply, const char *p1, const char *p2, const char *p3);
    void send(const char *reply, const char *p1, const char *p2);
    void send(const char *reply, const char *p1);
    void send(const char *reply);

    void handleInputCommands(void (cb)(const char* cmdName, const char*p1, const char*p2, const char*p3, const unsigned char*binary, int size));
  private:
    void parse(char a);
    void sendParameter(const char*p);
  private:
    char buffer[MAX_CMD_PARAM_LENGTH];
    char paramIndex;

    char cmdName[MAX_CMD_PARAM_LENGTH];
    char param1[MAX_CMD_PARAM_LENGTH];
    char param2[MAX_CMD_PARAM_LENGTH];
    char param3[MAX_CMD_PARAM_LENGTH];
    unsigned char binaryData[MAX_BINARY_PARAM_LENGTH];

    int binaryIndex;
    bool commentStarted;
    bool controlMark;
    int index;
    Stream *stream;
    void (*callback)(const char* cmdName, const char*p1, const char*p2, const char*p3, const unsigned char*binary, int size);
};

#endif
