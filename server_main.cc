#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "messagehandler.h"
#include "database.h"
#include "memdb.h"
#include "diskdb.h"

#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

using namespace std;

void listNewsgroups (Database&, MessageHandler&);
void createNewsgroup(Database&, MessageHandler&);
void deleteNewsgroup(Database&, MessageHandler&);
void listArticles   (Database&, MessageHandler&);
void createArticle  (Database&, MessageHandler&);
void deleteArticle  (Database&, MessageHandler&);
void getArticle     (Database&, MessageHandler&);


int main(int argc, char* argv[]){
	if (argc != 2) {
		cerr << "Usage: myserver port-number" << endl;
		exit(1);
	}
	
	int port = -1;
	try {
		port = stoi(argv[1]);
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		exit(1);
	}
	
	Server server(port);
	if (!server.isReady()) {
		cerr << "Server initialization error." << endl;
		exit(1);
	}
    
    //MemDb db{};
    DiskDb db{};
	
	while (true) {
		auto conn = server.waitForActivity();
		if (conn != nullptr) {
            MessageHandler m{*conn.get()};
			try {
                auto code = m.recvCode();
                //cout << "Received code " << (int)code << endl;
                switch (code) {
                    case Protocol::COM_LIST_NG   : listNewsgroups (db, m); break;
                    case Protocol::COM_CREATE_NG : createNewsgroup(db, m); break;
                    case Protocol::COM_DELETE_NG : deleteNewsgroup(db, m); break;
                    case Protocol::COM_LIST_ART  : listArticles   (db, m); break;
                    case Protocol::COM_CREATE_ART: createArticle  (db, m); break;
                    case Protocol::COM_DELETE_ART: deleteArticle  (db, m); break;
                    case Protocol::COM_GET_ART   : getArticle     (db, m); break;
                    default: throw ConnectionClosedException(); // misbehaving client
                }
                m.sendCode(Protocol::ANS_END);
                if (Protocol::COM_END != m.recvCode()) {
                    throw ConnectionClosedException(); // misbehaving client
                }
                //cout << "Ended command" << endl;
			} catch (ConnectionClosedException&) {
				server.deregisterConnection(conn);
				cout << "Client closed connection" << endl;
			} catch (ProtocolViolationException&) {
                server.deregisterConnection(conn);
                cout << "Client violated protocol" << endl;
            }
		} else {
			conn = make_shared<Connection>();
			server.registerConnection(conn);
			cout << "New client connects" << endl;
		}
	}
}


bool ackIf(bool cond, MessageHandler& m) {
    m.sendCode(cond ? Protocol::ANS_ACK : Protocol::ANS_NAK);
    return cond;
}

void listNewsgroups(Database& d, MessageHandler& m) {
    m.sendCode(Protocol::ANS_LIST_NG);
    auto ngs = d.list();
    m.sendIntParameter(ngs.size());
    for (auto& ng : ngs) {
        m.sendIntParameter(ng.id);
        m.sendStringParameter(ng.name);
    }
}

void createNewsgroup(Database& d, MessageHandler& m) {
    auto title = m.recvStringParameter();
    m.sendCode(Protocol::ANS_CREATE_NG);
    Newsgroup ng;
    ng.name = title;
    if (!ackIf(d.create(ng), m)) {
        m.sendCode(Protocol::ERR_NG_ALREADY_EXISTS);
    }
}

void deleteNewsgroup(Database& d, MessageHandler& m) {
    auto id = m.recvIntParameter();
    m.sendCode(Protocol::ANS_DELETE_NG);
    if (!ackIf(d.remove(id), m)) {
        m.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    }
}

void listArticles(Database& d, MessageHandler& m) {
    auto id = m.recvIntParameter();
    m.sendCode(Protocol::ANS_LIST_ART);
    try {
        auto articles = d.list(id);
        m.sendCode(Protocol::ANS_ACK);
        m.sendIntParameter(articles.size());
        for (auto& article : articles) {
            m.sendIntParameter(article.id);
            m.sendStringParameter(article.title);
        }
    } catch (const NewsgroupNotFoundException& e) {
        m.sendCode(Protocol::ANS_NAK);
        m.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    }
}

void createArticle(Database& d, MessageHandler& m) {
    Article a{};
    auto id = m.recvIntParameter();
    a.title = m.recvStringParameter();
    a.author = m.recvStringParameter();
    a.text = m.recvStringParameter();
    
    m.sendCode(Protocol::ANS_CREATE_ART);
    try {
        d.createArticle(id, a);
        m.sendCode(Protocol::ANS_ACK);
    } catch (const NewsgroupNotFoundException& e) {
        m.sendCode(Protocol::ANS_NAK);
        m.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    }
}

void deleteArticle(Database& d, MessageHandler& m) {
    auto ngId = m.recvIntParameter();
    auto aId = m.recvIntParameter();
    
    m.sendCode(Protocol::ANS_DELETE_ART);
    try {
        d.removeArticle(ngId, aId);
        m.sendCode(Protocol::ANS_ACK);
    } catch (const NewsgroupNotFoundException& e) {
        m.sendCode(Protocol::ANS_NAK);
        m.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    } catch (const ArticleNotFoundException& e) {
        m.sendCode(Protocol::ANS_NAK);
        m.sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);
    }
}

void getArticle(Database& d, MessageHandler& m) {
    auto ngId = m.recvIntParameter();
    auto aId = m.recvIntParameter();
    
    m.sendCode(Protocol::ANS_GET_ART);
    try {
        auto article = d.read(ngId, aId);
        m.sendCode(Protocol::ANS_ACK);
        m.sendStringParameter(article.title);
        m.sendStringParameter(article.author);
        m.sendStringParameter(article.text);
    } catch (const NewsgroupNotFoundException& e) {
        m.sendCode(Protocol::ANS_NAK);
        m.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    } catch (const ArticleNotFoundException& e) {
        m.sendCode(Protocol::ANS_NAK);
        m.sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);
    }
}
