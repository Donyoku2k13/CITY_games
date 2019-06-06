#pragma once
#include "CitiesModel.h"


void SetColor(int text, int bg) {
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((bg << 4) | text));
}

template<int txt = 7, int bg = 0>
ostream& color(ostream &text) {
	SetColor(txt, bg);
	return text;
}


class CitiesGame {
	CitiesModel game;
	string userName;
	int pts;
public:
	CitiesGame(string dir)
		:game(dir) {
		int r = 10 + rand() % 5;
		int x = 0;
		for (size_t i = 0; i < r; i++)
		{
			cout << "Äîáðî ïîæàëîâàòü â èãðó!\n";
			cout << "Çàãðóçêà";
			for (size_t i = 0; i < x; i++)
			{
				cout << ".";
			}
			Sleep(300);
			x++;
			if (x == 4) x = 0;
			system("cls");
		}
		fstream users(dir + "\\users.csv", ios::in);

		int counter = 0;
		string str;
		getline(users, str);
		while (!users.eof()) {
			getline(users, str);
			if (str.length() != 0)
				counter++;
		}
		cout << "Â èãðå ó÷àñòâóåò " << counter << " èãðîêîâ!";
		cout << "\n\nÂâåäèòå ñâîå èìÿ äëÿ íà÷àëà èãðû!\n";
		string name;
		getline(cin, name);
		users.close();
		users.open(dir + "\\users.csv", ios::out | ios::app);
		pts = 0;
		if (counter == 0)
			users << name << ";" << pts << ";1\n";
		else
			users << name << ";" << pts << ";0\n";
		userName = name;

		users.close();
	}
	void start() {
		bool exit = false;
		while (1) {
			vector<pair<string, pair<char, int>>> players;
			int flag = 0;
			while (flag != 1) {
				fstream users(game.getDir() + "\\users.csv");
				players.clear();
				while (!users.eof()) {
					string tmp;
					getline(users, tmp);
					if (tmp.size() > 0) {
						//ïåòÿ;0
						string name = tmp.substr(0, tmp.find(";"));
						int points = stoi(tmp.substr(tmp.find(";") + 1, tmp.rfind(";")));
						char c = tmp[tmp.size() - 1];
						if (c == '1'&&userName == name)
							flag = 1;
						if (c == '1')
							cout << "Õîäèò " << name << endl;
						players.push_back(make_pair(name, make_pair(c, points)));
					}
				}
				users.close();
				Sleep(1000);
				system("cls");
			}
			//Çíà÷èò ìû õîäèì
			int choice = 0;
			while (choice != 1) {
				cout << "Ó âàñ " << pts << " î÷êîâ!" << endl;
				string winner;
				int max = 0;
				for (size_t i = 0; i < players.size(); i++)
				{
					if (players[i].second.second > max)
					{
						max = players[i].second.second;
						winner = players[i].first;
					}
				}

				if (getLastCity().size() == 0)
					cout << "Ââåäèòå íàçâàíèå ëþáîãî ãîðîäà: ";
				else {
					if (winner == userName)
					{
						cout << "Âû ëèäèðóåòå!" << endl;
					}
					else cout << "Ëèäèðóåò " << winner << ": " << max << " î÷êîâ" << endl;
					cout << "Ïîñëåäíèé ãîðîä: " << color<2, 0> << getLastCity() << color<7, 0> << endl;
				}
				if (getLastLetter() != '0') {
					cout << "Ââåäèòå ãîðîä íà áóêâó " << color<2, 0> << getLastLetter() << color<7, 0> << endl;
					cout << color<4, 0> << "(äëÿ çàâåðøåíèÿ èãðû ââåäèòå 0): " << color<7, 0>;
				}

				string answer;
				getline(cin, answer);
				if (answer == "0")
				{
					exit = true;
					cout << "Ñïàñèáî çà èãðó!\n";
					cout << "Ïîáåäèòåëü: " << winner << endl
						<< "Ñ êîëè÷åñòâîì î÷êîâ: " << max << endl;
					break;
				}
				try {
					game.checkCity(answer);
					choice = 1;
					pts += 5;
				}
				catch (exception&e) {
					cout << color<4, 0> << e.what() << color<7, 0>;
					choice = 1;
					system("pause");
					system("cls");
				}
			}

			//ñ÷èòûâàåì òåêóùèõ èãðîêîâ
			fstream users(game.getDir() + "\\users.csv");
			players.clear();
			int pos;
			while (!users.eof()) {
				string tmp;
				getline(users, tmp);
				if (tmp.length() > 0) {
					//ïåòÿ;0
					string name = tmp.substr(0, tmp.find(";"));
					int points = stoi(tmp.substr(tmp.find(";") + 1, tmp.rfind(";")));
					char c = tmp[tmp.size() - 1];

					if (c == '1')
						pos = players.size();
					players.push_back(make_pair(name, make_pair(c, points)));
				}
			}
			users.close();
			players[pos].second.first = '0';
			players[pos].second.second = pts;
			pos++;
			if (pos == players.size())
				pos = 0;
			players[pos].second.first = '1';

			ofstream user(game.getDir() + "\\users.csv");
			if (exit)
			{
				user << "";
				ofstream usedC(game.getDir() + "\\usedCities.csv");
				usedC << "";
				user.close();
				usedC.close();
				break;
			}
			for (auto&i : players) {
				user << i.first << ";" << i.second.second << ";" << i.second.first << endl;
			}
			user.close();
			system("cls");
		}
		return;
	}
private:
	char getLastLetter() {
		fstream file(game.getDir() + "\\usedCities.csv");
		string str;
		while (!file.eof()) {
			string s;
			getline(file, s);
			//cout << s;
			if (s.size() > 0)
				str += s;
		}
		file.close();
		if (str.size() == 0) return '0';

		int posLet = str.size() - 1;
		bool invalid = false;
		do
		{
			if (str[posLet] == 'ü'
				|| str[posLet] == 'û'
				|| str[posLet] == 'é' ||
				str[posLet] == 'ö')
			{
				posLet--;
				invalid = true;
			}
			else invalid = false;
		} while (invalid);
		return str[posLet] - 32;
	}
	string getLastCity() {
		fstream file(game.getDir() + "\\usedCities.csv");
		string str;
		while (!file.eof()) {
			string s;
			getline(file, s);
			//cout << s;
			if (s.size() > 0)
				str = s;
		}
		file.close();

		return str;
	}
};
