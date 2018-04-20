#include "client.h"
#include "protocol.h"
#include <iostream>
#include "article.h"
#include "newsgroup.h"

using namespace std;

static void testCode(MessageHandler& mh, Protocol c) {
    if (static_cast<Protocol>(mh.recvByte()) != c) {
        throw ProtocolViolationException();
    }
}

static string getLine() {
    cin.ignore();
    string s;
    getline(cin, s);
    return s;
}

void listNewsgroupsCommand(MessageHandler& mh) {
    mh.sendCode(Protocol::COM_LIST_NG);
    mh.sendCode(Protocol::COM_END);
    
    testCode(mh, Protocol::ANS_LIST_NG);
    auto num = mh.recvIntParameter();
    switch (num) {
        case 0: cout << "No newsgroups" << endl; break;
        case 1: cout << "1 newsgroup:" << endl; break;
        default: cout << num << " newsgroups:" << endl;
    }
    for (auto i = 0; i < num; ++i) {
        auto id = mh.recvIntParameter();
        auto name = mh.recvStringParameter();
        cout << "  " << id << ": " << name << endl;
    }
    testCode(mh, Protocol::ANS_END);
}

void createNewsgroupCommand(MessageHandler& mh) {
    string name;
    cout << "Enter newsgroup title: "; name = getLine();
    cout << name << endl;
    
    mh.sendCode(Protocol::COM_CREATE_NG);
    mh.sendStringParameter(name);
    mh.sendCode(Protocol::COM_END);
    
    testCode(mh, Protocol::ANS_CREATE_NG);
    auto code = static_cast<Protocol>(mh.recvByte());
    if (Protocol::ANS_ACK == code) {
        cout << "Newsgroup created" << endl;
    } else {
        cout << "A newsgroup with this title already exists" << endl;
        testCode(mh, Protocol::ERR_NG_ALREADY_EXISTS);
    }
    testCode(mh, Protocol::ANS_END);
}

void deleteNewsgroupCommand(MessageHandler& mh) {
    Newsgroup::Id ngId;
    cout << "Enter newsgroup ID: "; cin >> ngId;
    mh.sendCode(Protocol::COM_DELETE_NG);
    mh.sendIntParameter(ngId);
    mh.sendCode(Protocol::COM_END);
    
    testCode(mh, Protocol::ANS_DELETE_NG);
    auto code = static_cast<Protocol>(mh.recvByte());
    if (Protocol::ANS_ACK == code) {
        cout << "Newsgroup deleted" << endl;
    } else {
        cout << "Newsgroup not found" << endl;
        testCode(mh, Protocol::ERR_NG_DOES_NOT_EXIST);
    }
    testCode(mh, Protocol::ANS_END);
}

void listArticlesCommand(MessageHandler& mh) {
    Newsgroup::Id ngId;
    cout << "Enter newsgroup ID: "; cin >> ngId;
    mh.sendCode(Protocol::COM_LIST_ART);
    mh.sendIntParameter(ngId);
    mh.sendCode(Protocol::COM_END);
    
    testCode(mh, Protocol::ANS_LIST_ART);
    auto code = static_cast<Protocol>(mh.recvByte());
    if (Protocol::ANS_ACK == code) {
        auto num = mh.recvIntParameter();
        switch (num) {
            case 0: cout << "No articles" << endl; break;
            case 1: cout << "1 article:" << endl; break;
            default: cout << num << " articles:" << endl;
        }
        for (auto i = 0; i < num; ++i) {
            auto id = mh.recvIntParameter();
            auto title = mh.recvStringParameter();
            cout << "  " << id << ": " << title << endl;
        }
    } else {
        cout << "Newsgroup not found" << endl;
        testCode(mh, Protocol::ERR_NG_DOES_NOT_EXIST);
    }
    testCode(mh, Protocol::ANS_END);
}

void createArticleCommand(MessageHandler& mh) {
    Newsgroup::Id ngId;
    string title, author, text;
    cout << "Enter newsgroup ID: "; cin >> ngId;
    cout << "Enter article title: "; title = getLine();
    cout << "Enter article author: "; author = getLine();
    cout << "Enter article text: "; text = getLine();
    
    mh.sendCode(Protocol::COM_CREATE_ART);
    mh.sendIntParameter(ngId);
    mh.sendStringParameter(title);
    mh.sendStringParameter(author);
    mh.sendStringParameter(text);
    mh.sendCode(Protocol::COM_END);
    
    testCode(mh, Protocol::ANS_CREATE_ART);
    auto code = static_cast<Protocol>(mh.recvByte());
    if (Protocol::ANS_ACK == code) {
        cout << "Article created" << endl;
    } else {
        testCode(mh, Protocol::ERR_NG_DOES_NOT_EXIST);
        cout << "Newsgroup not found" << endl;
    }
    testCode(mh, Protocol::ANS_END);
}

void deleteArticleCommand(MessageHandler& mh) {
    Newsgroup::Id ngId;
    Article::Id aId;
    cout << "Enter newsgroup ID: "; cin >> ngId;
    cout << "Enter article ID: "; cin >> aId;
    
    mh.sendCode(Protocol::COM_DELETE_ART);
    mh.sendIntParameter(ngId);
    mh.sendIntParameter(aId);
    mh.sendCode(Protocol::COM_END);
    
    testCode(mh, Protocol::ANS_DELETE_ART);
    auto code = static_cast<Protocol>(mh.recvByte());
    if (Protocol::ANS_ACK == code) {
        cout << "Article deleted" << endl;
    } else {
        code = static_cast<Protocol>(mh.recvByte());
        if (Protocol::ERR_NG_DOES_NOT_EXIST == code) {
            cout << "Newsgroup not found" << endl;
        } else {
            cout << "Article not found" << endl;
        }
    }
    testCode(mh, Protocol::ANS_END);
}

void getArticleCommand(MessageHandler& mh) {
    Newsgroup::Id ngId;
    Article::Id aId;
    cout << "Enter newsgroup ID: "; cin >> ngId;
    cout << "Enter article ID: "; cin >> aId;
    
    mh.sendCode(Protocol::COM_GET_ART);
    mh.sendIntParameter(ngId);
    mh.sendIntParameter(aId);
    mh.sendCode(Protocol::COM_END);
    
    testCode(mh, Protocol::ANS_GET_ART);
    auto code = static_cast<Protocol>(mh.recvByte());
    if (Protocol::ANS_ACK == code) {
        string title, author, text;
        title = mh.recvStringParameter();
        author = mh.recvStringParameter();
        text = mh.recvStringParameter();
        cout << "Title: " << title << endl << "Author: " << author << endl
             << "Text: " << text << endl << endl;
    } else {
        code = static_cast<Protocol>(mh.recvByte());
        if (Protocol::ERR_NG_DOES_NOT_EXIST == code) {
            cout << "Newsgroup not found" << endl;
        } else {
            cout << "Article not found" << endl;
        }
    }
    testCode(mh, Protocol::ANS_END);
}

