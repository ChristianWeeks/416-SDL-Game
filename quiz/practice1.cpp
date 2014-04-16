#include <iostream>
#include <cstring>

class string {
	public:
		string();				//default constructor
		string(const char*);	//
		string(const string&);
//		string operator+(const string&, const string&);
//		string operator+(const char*, const string&);
//		string operator+(const string&, const char*);		
		string& operator=(const string&);

		const char* getBuf() const;
	private:
		char* buf;

};

string::string() : buf(new char[1]){
	std::cout << "Default" << std::endl;
	buf[0] = '\0';
}

string::string(const char* s){
	std::cout << "Convert" << std::endl;
	buf = new char[strlen(s)+1];
	strcpy(buf, s);
}

string::string(const string& s){
	std::cout << "Copy" << std::endl;
	buf = new char[strlen(s.getBuf())+1];
	strcpy(buf, s.getBuf());
}

string& string::operator=(const string&s){
	delete buf;
	buf = new char[strlen(s.getBuf())+1];
	strcpy(buf, s.getBuf());

}

const char* string::getBuf() const{
	return buf;
}

int main(){

	string s;
	string t("dog");
	string f(t);
	return 0;
}
