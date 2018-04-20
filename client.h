#ifndef CLIENT_H
#define CLIENT_H

#include <sstream>
#include "messagehandler.h"

// Client commands:
typedef void (*CommandFunc)(MessageHandler&, std::stringstream&);

void listNewsgroupsCommand  (MessageHandler&, std::stringstream&);
void createNewsgroupCommand (MessageHandler&, std::stringstream&);
void deleteNewsgroupCommand (MessageHandler&, std::stringstream&);
void listArticlesCommand    (MessageHandler&, std::stringstream&);
void createArticleCommand   (MessageHandler&, std::stringstream&);
void deleteArticleCommand   (MessageHandler&, std::stringstream&);
void getArticleCommand      (MessageHandler&, std::stringstream&);

struct Command {
    CommandFunc func;
    std::string comment;
};

#endif
