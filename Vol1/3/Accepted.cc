#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <list>

//#define DEBUG

using namespace std;

map<char,char> maptable;

struct TrieNode{
	map<char,TrieNode*> children;
	TrieNode* root;
	string word;

	TrieNode(TrieNode* root):children(),root(root),word(){}
	static TrieNode* buildTree(list<string>& wordlist);
	void flush();
	TrieNode* getChild(char key){ return children[key];}
	TrieNode* operator[](const char key){return children[key];}
	bool hasKey(char key){return children.find(key)!=children.end();}
	bool hasWord() const{return !word.empty();}
	void addChild(char key){if(!hasKey(key)) children[key] = new TrieNode(this->root);}
	string getSub(int a, int b, const string& str);
};

TrieNode* TrieNode::buildTree(list<string>& wordlist)
{
	TrieNode* root;
	TrieNode* cur;
	root = new TrieNode(root);
	root->root = root;
	auto it = wordlist.begin();
	for(it;it != wordlist.end();it++){
		cur = root;
		auto itc = it->begin();
		for(itc;itc != it->end();itc++){
			char ch = maptable[*itc];
			cur->addChild(ch);
			cur = (*cur)[ch];
		}
		cur->word = *it;
	}
	return root;
}

void TrieNode::flush()
{
	if(this==nullptr) return;
	auto it = children.begin();
	for(it;it!=children.end();it++)
		it->second->flush();
	delete this;
}

string TrieNode::getSub(int a, int b, const string& str)
{
	TrieNode* cur = this;
	for(int i = a; i <= b; i++){
		if(!cur->hasKey(str[i])) return "";
		cur = (*cur)[str[i]];
	}
	if(cur->hasWord()) return cur->word;
	return "";
}

struct BestNode
{
	int len;//,mid;

	BestNode():len(-1){}
};

string FindSeq(const string& call,list<string>& wordlist)
{
	int callsz = call.size();
	int best[callsz][callsz];
	for(int i = 0;i<callsz;i++) for(int j =0;j<callsz;j++)best[i][j] = -1;
	TrieNode* root = TrieNode::buildTree(wordlist);
	string str[callsz][callsz];
	for(int length = 0; length < callsz; length++){
		for(int start = 0; start+length<callsz; start++){
			if(!root->getSub(start,start+length,call).empty()){
				best[start][start+length] = 1;
				str[start][start+length] = root->getSub(start,start+length,call);
			}else{
				int curbest = 9999,curmid = 0,flag = 0;
				for(int mid = start+1; mid < start+length; mid++){
					if(best[start][mid]!=-1&&best[mid+1][start+length]!=-1){
						flag = 1;
						if(best[start][mid]+best[mid+1][start+length]<curbest){
							curbest = best[start][mid]+best[mid+1][start+length];
							curmid = mid;
						}
					}
				}
				if(flag==1){
					best[start][start+length]= curbest;
					str[start][start+length] = str[start][curmid]+' '+str[curmid+1][start+length];
				}
			}
		}
	}
	string ret;
	if(best[0][callsz-1]==-1) {
		ret = string("No solution.");
	}else{
		ret = str[0][callsz-1];
	}
	root = root->root;
	root->flush();
	return ret;
}

int main()
{
	maptable['i'] = '1';maptable['j'] = '1';
	maptable['a'] = '2';maptable['b'] = '2';maptable['c'] = '2';
	maptable['d'] = '3';maptable['e'] = '3';maptable['f'] = '3';
	maptable['g'] = '4';maptable['h'] = '4';
	maptable['k'] = '5';maptable['l'] = '5';
	maptable['m'] = '6';maptable['n'] = '6';
	maptable['p'] = '7';maptable['r'] = '7';maptable['s'] = '7';
	maptable['t'] = '8';maptable['u'] = '8';maptable['v'] = '8';
	maptable['w'] = '9';maptable['x'] = '9';maptable['y'] = '9';
	maptable['o'] = '0';maptable['q'] = '0';maptable['z'] = '0';

	int num;
	string str,call;
	list<string> wordlist;
	while(true){
		cin >> call;
		if(call=="-1") break;
		#ifdef DEBUG
		cerr<<endl<<"New Call: "<<call<<endl;
		#endif
		cin >> num;
		for(int i = 0;i<num;i++){
			cin >> str;
			wordlist.push_back(str);
		}
		cout<< FindSeq(call,wordlist) << endl;
		wordlist.clear();
	}
	return 0;
}
