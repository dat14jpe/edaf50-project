#include "messagehandler.h"

MessageHandler::MessageHandler(Connection& conn)
    : conn(conn) {}

void MessageHandler::sendByte(unsigned char byte) {
    conn.write(byte);
}
    
void MessageHandler::sendCode(Protocol code) {
    sendByte(static_cast<unsigned char>(code));
}

void MessageHandler::sendInt(int value) {
    for (auto i = 3; i >= 0; --i) {
        sendByte((value >> (8 * i)) & 0xFF);
        // - to do: log?
    }
}

void MessageHandler::sendIntParameter(int param) {
    sendCode(Protocol::PAR_NUM);
    sendInt(param);
}

void MessageHandler::sendStringParameter(const std::string& param) {
    sendCode(Protocol::PAR_STRING);
    sendInt(param.size());
    for (char c : param) {
        sendByte(c);
    }
}

unsigned char MessageHandler::recvByte() {
    return conn.read();
}
    
Protocol MessageHandler::recvCode() {
    Protocol code = static_cast<Protocol>(recvByte());
    // - to do: log?
    return code;
}

int MessageHandler::recvInt() {
    int value = 0;
    for (auto i = 3; i >= 0; --i) {
        value |= recvByte() << (8 * i);
    }
    return value;
}

int MessageHandler::recvIntParameter() {
    auto code = recvCode();
    if (Protocol::PAR_NUM != code) {
        throw ProtocolViolationException{};
    }
    return recvInt();
}

std::string MessageHandler::recvStringParameter() {
    auto code = recvCode();
    if (Protocol::PAR_STRING != code) {
        throw ProtocolViolationException{};
    }
    int n = recvInt();
    if (n < 0) {
        throw ProtocolViolationException{};
    }
    std::string result;
    result.resize(n);
    for (auto& c : result) {
        c = static_cast<char>(conn.read());
    }
    return result;
}
