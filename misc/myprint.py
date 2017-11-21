import commands;
import sys;
import getopt;

print '- Number of arguments:', len(sys.argv), 'arguments.'
print '- Argument List:', str(sys.argv)

def main(argv) :
    inputfile  = 'test.txt'
    key1 = '1234'
    key2 = 'ab'
    try:
        opts, args = getopt.getopt(argv,"hi:o:k:p:",["ifile=","key1=","key2="])
    except getopt.GetoptError:
        print '- option error: test.py -i <logfile> -pattern1 <pattern1> -pattern2 <pattern2>'
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print '- help message: test.py -i <logfile> -pattern1 <pattern1> -pattern2 <pattern2>'
            sys.exit()
        elif opt in ("-i", "--ifile"):
            inputfile = arg
        elif opt in ("-k", "--key1"):
            key1 = arg
        elif opt in ("-p", "--key2"):
            key2 = arg
    print '- Input file :', inputfile
    print '- Key1 :', key1
    print '- Key2 :', key2
    logfile  = 'test.txt'
    pattern1 = '1234'
    pattern2 = 'ab'
    cmd = 'grep -n -i "' + pattern1 + '" ' +  logfile + '  | cut -f1 -d:'
    #print cmd;
    
    status, output = commands.getstatusoutput(cmd);
    #print status,output;
    if status != 0 :
        print '- commands error:', cmd;
        exit(1);
    result =  output.split('\n');
    #print result;
    n = len(result);
    #print n, '\n';
    i = 0; 
    while i < n:
        #print result[i];
        line1 = '0'
        if i == 0 :
            line1 = '1';
        else :
            line1 = result[i-1] 
        line2 = result[i]
        cmd1 = "sed -n '" + line1 + "," + line2 + "{/" + pattern2 + "/p}' test.txt"
        cmd2 = "sed -n '" + line2 +" p' test.txt"
        #print cmd1;
        #print cmd2;
        status, output = commands.getstatusoutput(cmd1);
        if status != 0 :
            print '- commands error:', cmd1;
            exit(2);
        print output
        status, output = commands.getstatusoutput(cmd2);
        if status != 0 :
            print '- commands error:', cmd2;
            exit(3);
        print output
        i = i + 1;

if __name__ == "__main__" :
    main(sys.argv[1:])
