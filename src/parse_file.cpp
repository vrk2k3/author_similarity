#include <iostream>
#include <list>
#include <fstream>
using namespace std;
string line;
string entry;

string subject;
string author;

ofstream catalog_out;
ofstream subject_out;
ofstream author_out;


string extract(string s1, string s2) {
	int s = line.find(s1) + s1.length();
	int e = line.find(s2, s+1);
	return line.substr(s, e-s);
}

string extract_between_tags() {
	int s = line.find(">");
	int e = line.find("<", s+1);
	return line.substr(s+1, e-s-1);
}

void print_subject() {
	if (subject.length() > 0) {
		int p = 0;
		int c = 0;
		while((p=subject.find("::", c))!=string::npos) {
			subject_out << subject.substr(c, p-c) << endl;
			c = p +2;
		}
	}
}

void print_author() {
	if (author.length() > 0) {
		author_out << author << endl;
	}
}

void print () {
	entry = entry + '\t' + subject;
	catalog_out << entry <<endl;

	print_subject();
	print_author();

}

int main() {
	catalog_out.open ("data/extract/catalog.tab");
	subject_out.open ("data/extract/subject.txt");
	author_out.open ("data/extract/author.txt");
	ifstream input;
	input.open ("data/source/catalog.rdf");

	int count = 0;
	while(getline(input, line)) {
		if (line.find("<pgterms:etext")!=string::npos) {
			if (entry.length() > 0) {
				count++;
				print();
				entry = "";
				subject = "";
				author = "";
			}
			if (count % 100 == 0) {
				cout << count << endl;
			}
			string id = extract("\"", "\"");
			entry = id;
		}

		if (line.find("<dc:title")!=string::npos) {
			if (line.find("</dc:title")==string::npos) { // title span multiple line
				string s;
				getline(input, s);
				while(s.find("</dc:title")==string::npos) {
					line+=s;
					getline(input, s);
				}
				//line = line.substr(0, line.length()-1)+"</dc:title";
				line+=s;
			}
			int found = line.find_first_of("\t\r\n");
			while(found!=string::npos) {
				line[found] = ' ';
				found= line.find_first_of("\t\r\n", found +1);
			}
			entry = entry + '\t' + extract_between_tags() ;
		}

		if (line.find("<dc:creator")!=string::npos) {
			author = extract_between_tags();
			entry = entry + '\t' + author ;
		}

		if (line.find("<rdf:li><dcterms:LCSH><rdf:value>")!=string::npos) {
			if (subject.length() == 0) {
				subject = extract("<rdf:value>", "</rdf:value>");
			} else {
				subject = subject + "::" + extract("<rdf:value>", "</rdf:value>");
			}
		}

		if (line.find("<dc:subject><dcterms:LCSH><rdf:value>")!=string::npos) {
			if (subject.length() == 0) {
				subject = extract("<rdf:value>", "</rdf:value>");
			} else {
				subject = subject + "::" + extract("<rdf:value>", "</rdf:value>");
			}
		}

		//<dc:subject><dcterms:LCSH><rdf:value>

	}
	print();


	catalog_out.close();
	subject_out.close();
	author_out.close();
}


