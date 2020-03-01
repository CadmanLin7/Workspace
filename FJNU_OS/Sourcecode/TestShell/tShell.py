import os, signal

com1 = "pexit"
com2 = "pecho"
com3 = "pkill"

while True:
    str = raw_input('$')
    spl_str = str.split()
    if spl_str[0] == com1 :
        quit()

    elif spl_str[0] == com2 :
        print spl_str[1]

    elif spl_str[0] == com3 :
        try:
            os.kill(int(spl_str[1]), 9)
            print 'Process %s is dead.' %(spl_str[1])
        except:
            print 'No such process.'
    else :
        print "No such command."
