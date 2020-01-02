#!/usr/bin/python

import argparse, string, random, sys
from optparse import OptionParser

class shuf:
    def __init__(self, filename="sys.stdin", irange="None", count=None, 
                 repeat=False):
        if (irange == "None"):
            if (filename != "sys.stdin"):
                try:
                    f = open(filename, 'r')
                    tlines = f.readlines()
                    f.close()
                except IOError as e:
                    errno, strerror = e.args
                    parser.error("I/O error({0}): {1}".
                                 format(error, strerror))
               
                self.lines = [line.strip("\n") for line in tlines]
                        
            else:
                self.lines = []
                unorderedlines = list(sys.stdin.read())
                temp = ""
                for i in range(0, len(unorderedlines)):
                    if unorderedlines[i] != "\n":
                        temp += unorderedlines[i]
                    else:
                        self.lines.append(temp);
                        temp = ""
                if temp != "":
                    self.lines.append(temp)
                    
        else:
            try:
                dash = irange.index("-")
            except ValueError as e:
                parser.error("invalid RANGE {0}".
                             format(irange))

            firstnum, lastnum = irange.split("-")
            
            first = int(firstnum)
            last = int(lastnum)

            self.lines = []

            for i in range(first, last + 1):
                self.lines.append(str(i))
            
            
        self.count = count
        self.repeat = repeat
        

        random.shuffle(self.lines)
    
    
        if (isinstance(self.count, int) and self.count > len(self.lines)
            and repeat == False):
            self.count = len(self.lines)

    def shuffle(self):
        
        if self.repeat:
           if(self.count !=  None):
               for i in range(0, self.count):
                   print(random.choice(self.lines))
           else:
               while True:
                   print(random.choice(self.lines))
        else:
            if(self.count !=  None):
               for i in range(0, self.count):
                   print(self.lines[i])
            else:
               for i in range(0, len(self.lines)):
                   print(self.lines[i])
        
def main():

    version_msg = "%prog 2.0"
    usage_msg = """%prog {OPTION]... FILE

Output randomly selected lines from FILE."""

    parser = OptionParser(version=version_msg, usage=usage_msg)
 
    parser.add_option("-i", "--input-range",
                      action="store", dest="irange",
                      help="treat each number LO through HI as an input file")

    parser.add_option("-n", "--head-count",
                      action="store", dest="count",
                      help="output at most COUNT lines")

    parser.add_option("-r", "--repeat",
                      action="store_true", dest="repeat",
                      help="output lines can be repeated")
    
    options, args = parser.parse_args(sys.argv[1:])

    repeat = bool(options.repeat)
     
    if (options.count != None):
        try:
            count = int(options.count)
        except ValueError as e:
            parser.error("invalid COUNT {0}".
                     format(options.count))
        if (count < 0):
            parser.error("negative COUNT {0}".
                     format(count))
    else:
        count = None

    filename = "sys.stdin"
    irange = str(options.irange)
    
    if irange == "None":
        if len(args) > 1:
            parser.error("extra operand {0}".
                         format(args))
    
        if len(args) == 1:
            filename = args[0]
            if (filename == "-"):
                filename = "sys.stdin"
        else:
            filename = "sys.stdin"

    else:
        if len(args) > 0:
            parser.error("extra operand {0}".
                         format(args))
        try:
            dash = irange.index("-")
        except ValueError as e:
            parser.error("invalid RANGE {0}".
                         format(irange))

        if dash == 0:
            parser.error("invalid RANGE {0}".
                         format(irange))
        
        first, last = irange.split("-")
       
        try:
            firstnum = int(first)
        except ValueError as e:
            parser.error("invalid RANGE {0}".
                         format(irange))
        try:
            lastnum = int(last)
        except ValueError as e:
            parser.error("invalid RANGE {0}".
                         format(irange))

        if firstnum > lastnum:
            parser.error("invalid RANGE {0}".
                         format(irange))

    s = shuf(filename, irange, count, repeat)
    s.shuffle();
    
if __name__ == "__main__":
    main()
