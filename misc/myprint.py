import commands;
import sys;
import getopt;

print 'Number of arguments:', len(sys.argv), 'arguments.'
print 'Argument List:', str(sys.argv)

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
