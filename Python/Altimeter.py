import os
import time
pids = [pid for pid in os.listdir('/proc') if pid.isdigit()]
processList = []
warning = 0
warningid = 0

# test with > dd if=/dev/zero of=/dev/null

while 1:
    # get list of pids
    pids = [pid for pid in os.listdir('/proc') if pid.isdigit()]
    processList = []
    for pid in pids:
        try:
            # /proc/pid/stat contains information cpu_usage calculation
            stats = open(os.path.join('/proc', pid, 'stat'), 'rb').read().split() # rb read only & binary mode
            uptime = open(os.path.join('/proc', 'uptime'), 'rb').read().split()
            utime = int(stats[13])
            stime = int(stats[14])
            cutime = int(stats[15])
            cstime = int(stats[16])
            starttime = int(stats[21])
            clocktck = int(os.popen("getconf CLK_TCK").read()) # clock tick

            total_time = (utime + stime + cutime + cstime)
            seconds = float(uptime[0]) - (starttime / clocktck)
            cpu_usage = 100 * ((total_time / clocktck)/seconds)
            processList.append([stats[0] ,stats[1], cpu_usage])
        except IOError: # proc has already terminated
            continue

    # highest cpu usage at position 0 by reverse sorting
    processList = sorted(processList,key=lambda x: x[2], reverse = 1)

    # when process has more than x percent cpu usage 2 times in a row, show warning
    if processList[0][2] > 90:
        if warning == 1 and warningid == processList[0][0]:
            print("Warning Process '" + processList[0][1][1:-1] + "' is using more than 90'%' of CPU ressources")
        #remember
        warning = 1
        warningid = processList[0][0]
    else:
        #reset
        warning = 0
        warningid = 0

    # print sorted list with info
    print("Process Name:       PID:     CPU%:")
    print("")
    for i in range(0,18):
        n = str(processList[i][1][1:-1])
        p = str(processList[i][0])
        # add spaces for formating
        if len(n) != 15:
            n = n + (15 - len(n)) * " "
        if len(p) != 6:
            p = p + (6 - len(p)) * " "

        print(n + "     " + p + "   %.3f" % processList[i][2] +" %")

    print("===========================================")
    time.sleep(5)
