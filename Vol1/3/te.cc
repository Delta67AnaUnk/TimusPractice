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

struct BestNode{
	int count;
	bool rollback;

	BestNode():count(-10),rollback(false){}
	bool valid() const{return count > -1;}
};

#ifndef DEBUG
int FindPart(string& call,string::size_type start,TrieNode* cur, vector<BestNode>& best)
{
	string::size_type i;
	TrieNode* tmpnode;
	for(i = start;i < call.size()+1;i++){
		char ch = 0;
		if(i < call.size()) ch = call[i];
		// 1. has child, has word, root also has child: two branches
		// 2. no child, has word: go back to root and try
		// 	2.1 root has child: continue
		// 	2.2 root has no child: dead branch, same as 3
		// 3. no child, no word: dead branch, block the entrance of this line in the previous node
		// 4. has child, no word: continue
		if(cur->hasWord()){
			// the cur pointer here still points to i-1
			if(i!=call.size() && best[i].valid()){
				return best[i].count+1;
			}else if(i==call.size()){
				return 1;
			}
			BestNode b1,b2;
			if(cur->hasKey(ch)){
				tmpnode = (*cur)[ch];
				b1.rollback = false;
				b1.count = FindPart(call,i+1,tmpnode,best);
			}
			if(cur->root->hasKey(ch)){
				tmpnode = (*(cur->root))[ch];
				b2.rollback = true;
				b2.count = FindPart(call,i+1,tmpnode,best)+1;
			}
			if(b1.valid() && !b2.valid()) best[i] = b1;
			else if(!b1.valid() && b2.valid()) best[i] = b2;
			else if(!b1.valid() && !b2.valid()) return -10;
			else{
				if(b1.count<b2.count){
					best[i] = b1;
				}else {
					best[i] = b2;
				}
			}
			return best[i].count;
		}else{
			if(i!=call.size() && cur->hasKey(ch)) cur = (*cur)[ch];
			else return -10;
		}
	}
	return -5;
}
#else
int FindPart(string& call,string::size_type start,TrieNode* cur, vector<BestNode>& best,int layer = 0)
{
	string::size_type i;
	cerr<<"Into layer "<<layer<<endl;
	for(i = start;i < call.size()+1;i++){
		char ch = call[0];
		if(i < call.size()) ch = call[i];
		try{
			if(ch<'0'&&ch>'9') throw ch;
		}catch(char){
			cerr<<"ERROR!!!!:bad ch:"<<ch<<endl;
		}
		if(cur->hasWord()){
			if(i!=call.size() && best[i].valid()){
				cerr<<"pre stored, return "<<best[i].count+1<<endl;
				return best[i].count+1;
			}else if(i==call.size()){
				cerr<<"reach end, last word: "<<cur->word<<endl;
				return 1;
			}
			BestNode b1,b2;
			if(cur->hasKey(ch)){
				cerr<<"Route 1 continue at "<<ch<<endl;
				b1.node = (*cur)[ch];
				b1.rollback = false;
				b1.count = FindPart(call,i+1,b1.node,best,layer+1);
				cerr<<"Route 1 done, b1 = "<<b1.count<<endl;
			}
			if(cur->root->hasKey(ch)){
				cerr<<"Route 2 start at "<<ch<<" word = "<<cur->word<<endl;
				b2.node = (*(cur->root))[ch];
				b2.rollback = true;
				b2.count = FindPart(call,i+1,b2.node,best,layer+1)+1;
				cerr<<"Route 2 done, b1 = "<<b2.count<<endl;
			}
			if(b1.valid() && !b2.valid()) best[i] = b1;
			else if(!b1.valid() && b2.valid()) best[i] = b2;
			else if(!b1.valid() && !b2.valid()){
				cerr<<"None valid branch, return -10"<<endl;
				return -10;
			}else{
				cerr<<"Comparing b1 and b2"<<endl;
				cerr<<"b1 has count: "<<b1.count<<endl;
				cerr<<"b2 has count: "<<b2.count<<endl;
				if(b1.count<b2.count){
					best[i] = b1;
					cerr<<"At "<<layer<<" best is continue, count = "<<best[i].count<<endl;
				}else {
					best[i] = b2;
					cerr<<"At "<<layer<<" best is end, count = "<<best[i].count<<endl;
				}
			}
			cerr<<"return "<<best[i].count<<endl;
			return best[i].count;
		}else{
			if(i!=call.size() && cur->hasKey(ch)){
				cerr<<"next: "<<ch<<endl;
				cur = (*cur)[ch];
			}else {
				cerr<<"Dead End, No word available"<<endl;
				return -10;
			}
		}
	}
	return -10;
}
#endif

string FindSeq(string& call,list<string>& wordlist)
{
	string ret;
	string::size_type i;
	vector<BestNode> best(call.size());
	TrieNode* root = TrieNode::buildTree(wordlist);
	int count = FindPart(call,0,root,best);
	#ifdef DEBUG
	cerr<<"Done, count = "<<count<<endl;
	#endif
	list<string> retls;
	if(count < 0) return "No solution.";
	else{
		int last = 0;
		TrieNode* lastptr;
		try{
			#ifdef DEBUG 
			cerr<<endl<<call<<endl<<"Parse done, output:"<<endl;
			#endif
			for(i=0;i<call.size();i++){
				lastptr = root;
				#ifdef DEBUG 
				cerr<<i<<": "<<call[i]<<endl;
				#endif
				if(root->hasWord()&&best[i].valid()){
					if(best[i].rollback){
						retls.push_back(root->word);
						root = (*(root->root))[call[i]];
					}else{
						root = (*root)[call[i]];
					}
				}else if(!root->hasWord()){
					root = (*root)[call[i]];
				}else{
					throw root;
				}
				if(root == nullptr) throw root;
			}
			retls.push_back(root->word);
		}catch(...){
			cerr<<"last "<<last<<endl;
			if(last==1){
				cerr<<"Error!"<<endl;
				cerr<<"validity: "<<best[i].valid()<<endl;
				cerr<<"word: "<<lastptr->word.empty()<<endl;
				if(lastptr->word.empty())
					cout<<"last word empty, show pointer: "<<lastptr->hasKey(call[i-1])<<endl;
				cerr<<"pointer: "<<lastptr<<endl;
			}
			cerr<<root<<endl;
			cerr<<best[i].rollback<<"  "<<best[i].valid()<<endl;
			cerr<<call<<": "<<i<<endl;
		}
	}
	for(auto it = retls.begin();it!=retls.end();it++){
		ret+=*it;
		ret+=' ';
	}
	ret = ret.substr(0,ret.size()-1);
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
