#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <string>
#include "connection.h"
#include "protocol.h"

struct ProtocolViolationException {}; // - to do: add message (and values)

class MessageHandler {
    Connection& conn;
    
public:
    MessageHandler(Connection&);
    
    // All of the below functions may throw ConnectionClosedException.
    // recv*Parameter functions may throw ProtocolViolationException.
    void sendByte(unsigned char);
    void sendCode(Protocol);
    void sendInt(int);
    void sendIntParameter(int);
    void sendStringParameter(const std::string&);
    unsigned char recvByte();
    Protocol recvCode();
    int recvInt();
    int recvIntParameter();
    std::string recvStringParameter();
};

#endif
