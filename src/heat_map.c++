#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <set>
#include <cmath>
#include <iomanip>
using namespace std;
struct scores_cos {
	string name;
	float score;
};
struct scores_match {
	string name;
	int score;
};
struct score_dice {
	string name;
	float score;
};
struct score_jac {
	string name;
	float score;
};
struct author {
	string name;
	vector<string> subjects;
};
vector<author> a;
vector<author> b;
bool compare(author &a, author &b) {
	return a.name < b.name;
}
bool compare3(score_dice a, score_dice b) {
	return a.score > b.score;
}
bool compare4(scores_match a, scores_match b) {
	return a.score > b.score;
}
bool compare5(score_jac a, score_jac b) {
	return a.score > b.score;
}
bool check(author a, author b) {
	return a.name < b.name;
}
void author_names(void) {
	ifstream myfile;
	myfile.open("data/author_uniq.txt");
	string c;
	while (getline(myfile, c)) {
		string q;
		int cumma = 0;
		for (int i = 0; c[i] != '\0'; i++) {
			if (c[i] == ',') {
				cumma++;
			}
			if (cumma == 2) {
				break;
			}
			q += c[i];
		}
		author d;
		//cout << q << endl;
		d.name = q;
		a.push_back(d);

	}
	sort(a.begin(), a.end(), compare);
}
bool compare2(scores_cos a, scores_cos b) {
	return a.score > b.score;
}
void read_catalog(void) {
	ifstream myfile2;
	myfile2.open("data/catalog.tab");
	for (int i = 0; i < 45291; i++) {
		string v;
		string r;
		string k;
		author q;
		int x = 0;
		myfile2 >> v;
		getline(myfile2, r);
		int t = 0;
		int h;
		for (int j = 0; r[j] != '\0'; j++) {
			if (r[j] == '	') {
				t++;
			}
			if (t == 2 && r[j] != '	') {
				h = j;
				k += r[j];
				x++;
			}

		}
		if (k.empty() == 0) {
			int cumma = 0;
			for (int j = 0; j < x; j++) {
				if (k[j] == ',') {
					cumma++;
				}
				if (cumma == 2) {
					break;
				}
				q.name += k[j];
			}
			//cout << q.name << endl;
			vector<author>::iterator p;
			p = lower_bound(a.begin(), a.end(), q, check);
			string e;
			author& ca = a[p - a.begin()];
			for (int j = h + 1; r[j] != '\0'; j++) {
				if (r[j] != '	' && r[j] != ':') {
					e += r[j];
				}
				if (r[j] == ':' && r[j + 1] == ':') {
					int f = 0;

					for (int i = 0; i != ca.subjects.size(); i++) {
						if (ca.subjects[i] == e) {
							f = 1;
						}
					}
					f = 0;
					if (f == 0 && ca.name == q.name) {
						ca.subjects.push_back(e);
					}
					e.clear();
				}
			}
			if (!e.empty() && ca.name == q.name) {
				ca.subjects.push_back(e);
			}

		}
	}
}
void read_top() {
	ifstream myfile3;
	myfile3.open("data/genre_authors.txt");
	string c;
	while (getline(myfile3, c)) {
		string q;
		int cumma = 0;
		for (int i = 0; c[i] != '\0'; i++) {
			if (c[i] == ',') {
				cumma++;
			}
			if (cumma == 2) {
				break;
			}
			q += c[i];
		}
		author d;
		//	cout << q << endl;
		d.name = q;
		b.push_back(d);

	}
	//sort(b.begin(), b.end(), compare);
}
int find(string check) {
	for (int i = 0; i != a.size(); i++) {
		if (a[i].name == check) {
			return i;
		}
	}
	return 0;
}
float cosine_score(int fir, int sec) {
	set<string> all_subjects;
	int sid = 0;
	for (int i = 0; i != a[fir].subjects.size(); i++) {
		all_subjects.insert(a[fir].subjects[i]);
	}
	for (int i = 0; i != a[sec].subjects.size(); i++) {

		all_subjects.insert(a[sec].subjects[i]);
		sid++;
	}
	int cn = all_subjects.size();
	int first[cn];
	int second[cn];
	for (int i = 0; i < cn; i++) {
		first[i] = 0;
		second[i] = 0;
	}
	int first_ind = 0;
	int second_ind = 0;
	for (set<string>::iterator it = all_subjects.begin();
			it != all_subjects.end(); it++) {
		for (int i = 0; i != a[fir].subjects.size(); i++) {
			if (*it == a[fir].subjects[i]) {
				first[first_ind]++;
			}
		}
		first_ind++;
	}
	for (set<string>::iterator it = all_subjects.begin();
			it != all_subjects.end(); it++) {
		for (int i = 0; i != a[sec].subjects.size(); i++) {
			if (*it == a[sec].subjects[i]) {
				second[second_ind]++;
			}
		}
		second_ind++;
	}
	float sum = 0;
	float deno1 = 0;
	float deno2 = 0;
	for (int i = 0; i < cn; i++) {
		sum += first[i] * second[i];
	}
	for (int i = 0; i < cn; i++) {
		deno1 += first[i] * first[i];
		deno2 += second[i] * second[i];
	}
	float final_deno = sqrt(deno1) * sqrt(deno2);
	float ansco = sum / final_deno;
	return ansco;
}
float jaccard(int l, int j) {
	set<string> uniq;
	if (a[j].name != a[l].name) {
		for (int i = 0; i != a[l].subjects.size(); i++) {
			for (int k = 0; k != a[j].subjects.size(); k++) {
				if (a[l].subjects[i] == a[j].subjects[k]) {
					uniq.insert(a[l].subjects[i]);
				}
			}
		}
	}
	float je = a[l].subjects.size();
	float av = a[j].subjects.size();
	float z = uniq.size() / (je + av - uniq.size());
	return z;
}
int matching(int l, int j) {
	int num = 0;
	if (a[l].name != a[j].name) {
		for (int k = 0; k != a[j].subjects.size(); k++) {
			for (int i = 0; i != a[l].subjects.size(); i++) {
				if (a[j].subjects[k] == a[l].subjects[i]) {
					num++;
				}
			}
		}
	}
	return num;
}
float dice(int l, int j) {
	float num = 0;
	if (a[l].name != a[j].name) {
		for (int k = 0; k != a[j].subjects.size(); k++) {
			for (int i = 0; i != a[l].subjects.size(); i++) {
				if (a[j].subjects[k] == a[l].subjects[i]) {
					num++;
				}
			}
		}
	}
	num *= 2;
	num /= (a[j].subjects.size() + a[l].subjects.size());
	return num;
}


int main(void){
	author_names();
	read_catalog();
	read_top();
	ofstream res;
	res.open("reports/heat_map_dice.csv");
	for(int i = 0; i != b.size(); i++){
		int firstIndex = find(b[i].name);

		for(int j = 0; j != b.size(); j++){
			int secondIndex = find(b[j].name);
			if(i == j){
				res << 0 << ' ';
			}else{
				//float b = cosine_score(i,j);
				res << dice(firstIndex,secondIndex) << ' ';
//				cout  << b[i].name << " " << a[firstIndex].subjects.size() << " " <<
//						b[j].name << " "   << " " << a[secondIndex].subjects.size() <<  " " << dice(firstIndex,secondIndex) << endl;
			}
		}
		res << endl;
	}
	res.close();
	res.open("reports/heat_map_cosine.csv");
	for(int i = 0; i != b.size(); i++){
		int firstIndex = find(b[i].name);
		for(int j = 0; j != b.size(); j++){
			int secondIndex = find(b[j].name);
			if(i == j){
				res << 0 << ' ';
			}else{
				//float b = cosine_score(i,j);
				res << cosine_score(firstIndex,secondIndex) << ' ';
			}
		}
		res << endl;
	}
	res.close();
	res.open("reports/heat_map_jaccard.csv");
	for(int i = 0; i != b.size(); i++){
		int firstIndex = find(b[i].name);
		for(int j = 0; j != b.size(); j++){
			int secondIndex = find(b[j].name);
			if(i == j){
				res << 0 << ' ';
			}else{
				//float b = cosine_score(i,j);
				res << jaccard(firstIndex,secondIndex) << ' ';
			}
		}
		res << endl;
	}
	res.close();
	res.open("reports/heat_map_match.csv");
	for(int i = 0; i != b.size(); i++){
		int firstIndex = find(b[i].name);
		for(int j = 0; j != b.size(); j++){
			int secondIndex = find(b[j].name);
			if(i == j){
				res << 0 << ' ';
			}else{
				//float b = cosine_score(i,j);
				res << matching(firstIndex,secondIndex) << ' ';
			}
		}
		res << endl;
	}

}
