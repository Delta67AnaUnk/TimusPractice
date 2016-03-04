/*
 * Algorithm:
 * Trie Tree: save the wordlist using a trie tree with the related number as key
 * a visit on root at any time cause the count to increase
 *
 * DP:
 * When reaches a node containing word and is not a leaf node, two branches are 
 * generated, the one that goes back to root increases the count number. DFT, and,
 * when reach the end, save the whole path and, for every char in the array, save 
 * the current best solution
 * 
 */
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <list>

using namespace std;

map<char,char> maptable;

struct TrieNode{
	map<char,TrieNode*> children;
	TrieNode* root;
	string word;

	TrieNode(TrieNode* root):children(),root(root),word(){}
	static TrieNode* buildTree(list<string>& wordlist);
	static TrieNode* buildRevTree(list<string>& wordlist);
	void flush();
	TrieNode* getChild(char key){ return children[key];}
	void addChild(char key){if(children[key]==NULL)	children[key] = new TrieNode(this->root);}
	TrieNode* operator[](const char key){return children[key];}
	bool isLeaf(){return children.size()==0;}
	bool hasKey(char ch){return children.find(ch)!=children.end();}
};

TrieNode* TrieNode::buildTree(list<string>& wordlist)
{
	TrieNode* root;
	TrieNode* cur;
	root = new TrieNode(root);
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

TrieNode* TrieNode::buildRevTree(list<string>& wordlist)
{
	TrieNode* root;
	TrieNode* cur;
	root = new TrieNode(root);
	auto it = wordlist.begin();
	for(it;it != wordlist.end();it++){
		cur = root;
		auto itc = it->rbegin();
		for(itc;itc != it->rend();itc++){
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

struct Memo{
	list<string> str;
	TrieNode* ptr,*rptr;

	Memo(TrieNode* ptr, TrieNode* rptr):str(),ptr(ptr),rptr(rptr){}
	Memo():str(),ptr(nullptr),rptr(nullptr){}
	Memo(const Memo& other):str(other.str),ptr(other.ptr),rptr(other.ptr){}
	Memo* next(const char ch);
	Memo* rnext(const char ch);
	void deact(){str.clear();ptr = nullptr; rptr = nullptr;}
	bool operator <(const Memo& other){return this->str.size() < other.str.size() && this->str.size() > 0;}
};

Memo* Memo::next(const char ch)
{
	// pointer always points on the current node (thus root is never reached)
	if(ptr==nullptr) return nullptr;
	// cases:
	// 1. ptr has string, no child 
	// 	1.1 root has child -> save current string and push new tail
	// 	1.2 root has no child -> dead branch
	// 2. ptr has no string, no child -> dead branch
	// 3. ptr has child -> update tail
	Memo* ret = nullptr;
	if( ptr->hasKey(ch) ){ // child exists
		ret = new Memo(*this);
		ret->ptr = (*ptr)[ch];
		ret->str.back() = ptr->word;
	} else {
		if(str.back().empty()) // no string, dead
			return nullptr;
		else{
			if( ptr->root->hasKey(ch) ){
				ret = new Memo(*this);
				ret.ptr = (*(ptr->root))[ch];
				ret.str.push_back(ptr->word);
			}
			else
				return nullptr;
		}
	}
	return ret;
}

Memo* Memo::rnext(const char ch)
{
	if(rptr==nullptr) return nullptr;
	Memo* ret = nullptr;
	if( rptr->hasKey(ch) ){
		ret = new Memo(*this);
		ret->rptr = (*rptr)[ch];
		ret->str.front() = rptr->word;
	} else {
		if(str.front().empty()) 
			return nullptr;
		else{
			if( rptr->root->hasKey(ch) ){
				ret = new Memo(*this);
				ret->rptr = (*(rptr->root))[ch];
				ret->str.push_front(rptr->word);
			}
			else
				return nullptr;
		}
	}
	return ret;
}
// DP:
// Trie Tree and Reverse Trie Tree
// if a word exists, one possible seq appears.
// memo table is marked by the start and end of each sub seq.
// A shortest seq is seq[i:k]+seq[k+1,j] where two seq are both shortest, or a total new seq
// use the two trie trees, search forward and backword for valid words
string FindSeq(string& call,list<string>& wordlist)
{
	string ret;
	string::size_type i,j,x,y;

	Memo*** memo = new Memo** [call.size()];
	memo[0] = new Memo*[call.size() * call.size()]();
	for(int k = 1;k < call.size();k++)
		memo[k] = memo[k-1]+call.size();
	TrieNode* root = TrieNode::buildTree(wordlist);
	TrieNode* revroot = TrieNode::buildRevTree(wordlist);
	for(i = 0; i < call.size(); i++){
		memo[i][i] = Memo(root,nullptr).next(call[i]);
		if(revroot->hasKey(call[i])) memo[i][i].rptr = revroot[call[i]];
	}
	for(x = 1; x < call.size(); x++){ // start of substring
		for(y = 0; y+x < call.size(); y++){ //end of substring
			i = x+y;
			j = y;
			Memo* m1 = memo[i-1][j]->next(call[j]);
			Memo* m2 = memo[i][j+1]->rnext(call[i]);
			// nullptr is error state, which means the whole branch fails and should be discarded.
			// empty string means this branch has not get the end value, it does not implies the branch is invalid.
			// but if a branch has return a string, it is sure that the branch is still working.
			if(m1 && m2)
				if(memo[i-1][j] < memo[i][j-1]){
					memo[i][j].str.push_back(s1);
				}else{
					memo[i][j].str.push_front(s2);
				}
			}else if(!m1 && m2){
				memo[i][j].str.push_front(s2);
			}else if(s2.empty() && !s1.empty()){
				memo[i][j].str.push_back(s1);
			}
		}
	}
	if(memo[call.size()-1][0].ptr==root || memo[call.size()-1][0].rptr == root){
		auto it = memo[call.size()-1][0].str.begin();
		for(it;it!=memo[call.size()-1][0].str.end();it++)
			ret += (*it + " ");
		if(!ret.empty()) ret = ret.substr(0,ret.size()-1);
	} else {
		ret = "No solution.";
	}
	root->flush();
	revroot->flush();
	for(auto ita = memo.begin();ita != memo.end();ita++){
		for(auto itb = ita->begin(); itb != ita->end(); itb++){
			if( *itb != nullptr) delete *itb;
		}
	}
	delete [] memo[0];
	delete [] memo;
	return ret;
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

	int num;
	string str,call;
	list<string> wordlist;
	while(true){
		cin >> call;
		if(call=="-1") break;
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
