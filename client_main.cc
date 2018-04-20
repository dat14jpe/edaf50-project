#include "connection.h"
#include "connectionclosedexception.h"
#include "client.h"
#include "messagehandler.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <map>
#include <sstream>

using namespace std;

void exitCommand(MessageHandler&) {
    exit(0);
}


int main(int argc, char* argv[]) {
	if (argc != 3) {
		cerr << "Usage: myclient host-name port-number" << endl;
		exit(1);
	}
	
	int port = -1;
	try {
		port = stoi(argv[2]);
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		exit(1);
	}
	
	Connection conn(argv[1], port);
	if (!conn.isConnected()) {
		cerr << "Connection attempt failed" << endl;
		exit(1);
	}
	
    MessageHandler mh{conn};
    map<string, Command> commands;
    commands["list_groups"   ] = {listNewsgroupsCommand , "List newsgroups"};
    commands["create_group"  ] = {createNewsgroupCommand, "Create a newsgroup (input name)"};
    commands["delete_group"  ] = {deleteNewsgroupCommand, "Delete a newsgroup (input ID)"};
    commands["list_articles" ] = {listArticlesCommand   , "List articles in a newsgroup (input ID)"};
    commands["create_article"] = {createArticleCommand  , "Create an article (input title, author, and text)"};
    commands["delete_article"] = {deleteArticleCommand  , "Delete an article (input newsgroup ID and article ID)"};
    commands["get_article"   ] = {getArticleCommand     , "Get an article (input newsgroup ID and article ID)"};
    commands["exit"          ] = {exitCommand           , "Exit program"};
    
	cout << "To list available commands, type help" << endl << "Type a command: ";
	string line;
	while (cin.good()) { //getline(cin, line)) {
		try {
            /*stringstream ss{line};
            string command;
            ss >> command;*/
            string command;
            cin >> command;
            if ("help" == command) {
                cout << "Available commands: " << endl;
                for (auto& c : commands) {
                    cout << "  " << c.first << ":" << endl << "    " << c.second.comment << endl;
                }
            } else {
                auto cs = commands.find(command);
                if (commands.end() != cs) {
                    cs->second.func(mh);
                } else {
                    cout << "Unrecognized command" << endl;
                }
            }
		} catch (ConnectionClosedException&) {
			cout << " no reply from server. Exiting." << endl;
			exit(1);
		}
        cout << "Type a command: ";
	}
}

