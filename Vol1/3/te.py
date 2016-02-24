#import time
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

minlen = 0

def FindSub(call,wordlist,numlist,curlen):
    flag = False
    global minlen
    curstr = call;
    tmplen = 999999
    for index, num in enumerate(numlist):
        if num in call:
            count = call.count(num)
            tmplen = curlen + count
            if tmplen >= minlen:
                # If already longer than current shortest sequence, no need to try
                continue
            newstr = call.replace(num,wordlist[index])
            subls = None
            if not any(i.isdigit() for i in newstr):
                # Get one answer and verify it is shorter than current minimum
                if tmplen < minlen:
                    minlen = tmplen
                    subls = newstr
            elif curlen < minlen-1:
                # If reaches the number of current minimum length, no need to test other cases
                subls = FindSub(newstr,wordlist,numlist,tmplen)

            if subls != None:
                curstr = subls
                flag = True
    if flag == False:
        return None
    else:
        return curstr

def Te():
    global minlen
    call = input()
    wordlist = []
    numlist = []
    while call != "-1":
        minlen = len(call)+1
        wordlist[:] = []
        numlist[:] = []
        num = int(input())
        for _ in range(num):
            wordlist.append(input())
        wordlist.sort(key=len,reverse=True)
        # Greedy try, longest word first
        for word in wordlist:
            numword=""
            for i in range(len(word)):
                numword += table[word[i]]
            numlist.append(numword)
        wordlist[:] = [' '+word for word in wordlist]
        string = FindSub(call,wordlist,numlist,0)
        if string == None:
            print("No solution.")
        else:
            print(string[1:])
        call = input()
    return 0

if __name__ == "__main__":
    #tm = time.time()
    Te()
    #print(time.time()-tm)
