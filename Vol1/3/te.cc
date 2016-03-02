#include <functional>
#include <algorithm>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <list>
using namespace std;

map<char,char> maptable;

typedef pair<string,string> strpair;
typedef pair<string,string::size_t> strint;

vector< vector<string> >* memo;

struct TrieNode{
	map<char,TrieNode*> children;
	string word;

	TrieNode::TrieNode():children(),word(){}
	static TrieNode* buildTree(list<string>& wordlist);
	void flush();
	TrieNode* getChild(char key){ return children[key];}
	void addChild(char key){if(children[key]==NULL)	children[key] = new TrieNode;}
};

TrieNode* TrieNode::buildTree(list<string>& wordlist)
{
	TrieNode* root = new TrieNode;
	TrieNode* cur;
	auto it = wordlist.begin();
	for(it;it != wordlist.end();it++){
		cur = root;
		auto itc = it->begin();
		for(itc;itc != it->end();itc++){
			char ch = maptable[*itc];
			cur->addChild(ch);
			cur = cur->getChild(ch);
		}
		cur->word = *it;
	}
	return root;
}

void TrieNode::flush()
{
	auto it = children.begin();
	for(it;it!=children.end();it++)
		it->flush();
	delete this;
}

// DP:
// Trie Tree:
// FindSeq recur to match the current call and find every possible words along the line.
// if a word exists, one possible seq appears.
// memo table is marked by the start and end of each sub seq.
// A shortest seq is seq[i:k]+seq[k+1,j] where two seq are both shortest, or a total new seq
//
string FindSeq(string& call, TrieNode* root, string::size_t i, string::size_t len)
{
	vector<strpair>::iterator it;
	vector<strpair>::size_t sz;
	if(lenstr==0) return "";
	return NULL;
}

int main()
{
	maptable['i'] = '1';maptable['j'] = '1';
	maptable['a'] = '2';maptable['b'] = '2';maptable['c'] = '2';
	maptable['d'] = '3';maptable['e'] = '3';maptable['d'] = '3';
	maptable['g'] = '4';maptable['h'] = '4';
	maptable['k'] = '5';maptable['l'] = '5';
	maptable['m'] = '6';maptable['n'] = '6';
	maptable['p'] = '7';maptable['r'] = '7';maptable['s'] = '7';
	maptable['t'] = '8';maptable['u'] = '8';maptable['v'] = '8';
	maptable['w'] = '9';maptable['x'] = '9';maptable['y'] = '9';
	maptable['o'] = '0';maptable['q'] = '0';maptable['z'] = '0';

	int num,call;
	string str;
	list<string> wordlist;
	while(true){
		cin >> call;
		if(call==-1) break;
		cin >> num;
		for(int i = 0;i<num;i++){
			cin >> str;
			wordlist.push_back(str);
		}
		TrieNode* root = TrieNode::buildTree(wordlist);
		memo = new string[call.size()][call.size()]();
		str = FindSeq(call,root);
		if(str == "") cout<<"No solution."<<endl;
		else cout<<str<<endl;
		wordlist.clear();
	}
	return 0;
}
