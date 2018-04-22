#ifndef CLIENT_H
#define CLIENT_H

#include "messagehandler.h"

// Client commands:
typedef void (*CommandFunc)(MessageHandler&);

void listNewsgroupsCommand  (MessageHandler&);
void createNewsgroupCommand (MessageHandler&);
void deleteNewsgroupCommand (MessageHandler&);
void listArticlesCommand    (MessageHandler&);
void createArticleCommand   (MessageHandler&);
void deleteArticleCommand   (MessageHandler&);
void getArticleCommand      (MessageHandler&);

struct Command {
    CommandFunc func;
    std::string comment;
};

#endif
