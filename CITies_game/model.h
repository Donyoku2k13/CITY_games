#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <Windows.h>
using namespace std;

class CitiesModel {
	map<string, string> cities;
	string dir;
public:
	CitiesModel(string citiesDir) {
		this->dir = citiesDir;
		loadCities();
	}
	string getDir()const { return dir; }
	void checkCity(string answerCity) {
		if (cities.find(answerCity) == end(cities))
			throw exception("Ãîðîä íå íàéäåí!");
		vector<string> usedCities;
		fstream file(dir + "\\usedCities.csv", ios::in);
		while (!file.eof()) {
			string c;
			getline(file, c);
			if (c.size() > 0)
				usedCities.push_back(c);
		}
		file.close();
		if (usedCities.size() == 0) {
			file.open(dir + "\\usedCities.csv", ios::out);
			file << answerCity;
			file.close();
			return;
		}
		//answerCity;
		//usedCities;
		if (find(begin(usedCities), end(usedCities), answerCity)
			!= end(usedCities))
			throw exception("Ãîðîä óæå èñïîëüçîâàëñÿ!");
		//Àëìàòû Òàìáîâ
		char c = *((usedCities.end() - 1)->end() - 1);
		if (c == 'ü' || c == 'û' || c == 'é' ||
			c == 'ö')
			c = *((usedCities.end() - 1)->end() - 2);

		c -= 32;
		if (answerCity[0] != c) {
			string res = "Áóêâû íå ñîâïàäàþò, äîëæíà áûòü áóêâà ";
			res.push_back(c);
			throw exception(res.c_str());
		}
		usedCities.push_back(answerCity);
		file.open(dir + "\\usedCities.csv", ios::out);
		for (auto&i : usedCities)
			file << i << endl;
		file.close();

	}

private:
	void loadCities() {
		ifstream file(dir + "\\city.csv");
		if (!file)
			throw exception("Íå óäàåòñÿ îòêðûòü ôàéë!");
		string city;
		getline(file, city);
		while (!file.eof()) {
			getline(file, city);
			//4315;3159;4312;Àïðåëåâêà
			city = city.substr(city.rfind(';') + 1);
			cities.emplace(city, "");
		}
		file.close();
	}
};
