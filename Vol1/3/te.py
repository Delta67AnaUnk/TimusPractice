import re
table = {
        'a':'2','b':'2','c':'2',
        'd':'3','e':'3','f':'3',
        'g':'4','h':'4','i':'1',
        'j':'1','k':'5','l':'5',
        'm':'6','n':'6','o':'0',
        'p':'7','q':'0','r':'7',
        's':'7','t':'8','u':'8',
        'v':'8','w':'9','x':'9',
        'y':'9','z':'0'
        }

def FindSub(call,wordlist,numlist):
    flag = False
    curlen = 99999999;
    curstr = call;
    leftpart = len(re.findall(r'\d+',call))
    wordcount = len(re.findall(r'[a-z]+',call))
    for index, num in enumerate(numlist):
        if num in call:
            newstr = call.replace(num,' '+wordlist[index])
            if not any(i.isdigit() for i in newstr):
                subls = newstr
            else:
                subls = FindSub(newstr,wordlist,numlist)
            if subls == None:
                continue
            elif len(subls.split())-wordcount == leftpart:
                return subls
            elif len(subls) < curlen:
                curlen = len(subls)
                curstr = subls
                flag = True
    if flag == False:
        return None
    else:
        return curstr

def Te():
    call = input()
    wordlist = []
    numlist = []
    while call != "-1":
        wordlist[:] = []
        numlist[:] = []
        num = int(input())
        for _ in range(num):
            wordlist.append(input())
        for word in wordlist:
            numword=""
            for i in range(len(word)):
                numword += table[word[i]]
            numlist.append(numword)
        string = FindSub(call,wordlist,numlist)
        if string == None:
            print("No solution.")
        else:
            print(string[1:])
        call = input()
    return 0

Te()
