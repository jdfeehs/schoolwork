#/usr/bin/env python
import sys
import struct
def main():
    #CONSTANTS- used in the rest of the code
    kernel_offset = 0xc0000000
    swapper_addr = 0x003ed56d
    init_task = 0x003ed3a0
    name_offset = (swapper_addr - init_task)
    next_addr = 0x003ed42c
    first_next = next_addr
    next_offset = (next_addr - init_task)
    next_to_name = swapper_addr - next_addr
    next_offset = 0x8c
    name_offset = 0x1cd
    next_to_pid = 0x3c
    next_to_parent_addr = 0x44
    parent_offset = 0xd4 
    pid_offset = 0xc8
    state_offset = 0x0
    uid_offset = 0x18c
    gid_offset = 0x19c
    mm_struct_offset = 0xa4
    mm_hiwater_offset = 0x58
  

    #open the memory snapshot
    infile = open(sys.argv[1],'rb')
 
    #for testing purposes, I decided to find the values for
    # the swapper task outside of my main loop. 
    #seek to the next value, grab it
    infile.seek(init_task + next_offset)
    next_value = struct.unpack('<I', infile.read(4))[0]
    next_value = next_value - kernel_offset
    
    #now that I grabbed the next value, seek to grab the name
    infile.seek(init_task + next_offset + next_to_pid)
    pid = struct.unpack('<I',infile.read(4))[0]
    infile.seek(init_task + name_offset)
    name = ""
    curByte = infile.read(1)
    while ord(curByte) != 0:
        name += curByte
        curByte = infile.read(1)
 
    #get the parent address
    infile.seek(init_task + parent_offset)
    parent_addr = struct.unpack('<I',infile.read(4))[0] - kernel_offset  

    #go there, get the ppid
    infile.seek(parent_addr+pid_offset)
    parent_pid = struct.unpack('<I', infile.read(4))[0]
    #From earlier checking, I know the PID of swapper is 0


    #get the uid and gid
    infile.seek(init_task + uid_offset)
    uid = struct.unpack('<I',infile.read(4))[0]
    infile.seek(init_task + gid_offset)
    gid = struct.unpack('<I',infile.read(4))[0]

    #get the state
    infile.seek(init_task)
    state_num = struct.unpack('<I',infile.read(4))[0]
    if state_num == 0:
        state = "RU"
    elif state_num == 1:
        state = "IN"
    elif state_num == 130:
        state = "UN,WK"

    #go to mm_struct
    infile.seek(init_task + mm_struct_offset)
    mm_addr = struct.unpack('<I',infile.read(4))[0] - kernel_offset
    if not mm_addr == (kernel_offset * -1) or mm_addr == 0:
        infile.seek(mm_addr + mm_hiwater_offset)
        hiwater_vm = struct.unpack('<I',infile.read(4))[0]
        infile.seek(mm_addr + mm_hiwater_offset + 4)
        total_vm = struct.unpack('<I',infile.read(4))[0]
    else:
        hiwater_vm = -1
        total_vm = -1


    # setup the table output
    print '{0:<16}{1:<8}{2:<8}{3:<8}{4:<8}{5:<16}{6:<8}{7:<8}{8:<8}'.format("kernel-addr", "pid","ppid","uid","gid","name","state","rss","total-vm")
    print "-----------------------------------------------------------------------------------------------"

    #print the name, the address of the top, and the next value
    print '{0:<16}{1:<8}{2:<8}{3:<8}{4:<8}{5:<16}{6:<8}{7:<8}{8:<8}'.format(hex(init_task+kernel_offset),pid,parent_pid,uid,gid,name,state,hiwater_vm,total_vm)


    #now loop through all of the task_structs
    while(not (next_value == first_next)):
        infile.seek(next_value)
        #save this next field addr
        next_addr = next_value
        next_top = next_addr - next_offset 
        #find the next "next" field
        next_value = struct.unpack('<I', infile.read(4))[0] - kernel_offset
        #get the pid
        infile.seek(next_addr - next_offset + pid_offset)# next_to_pid)
        pid = struct.unpack('<I',infile.read(4))[0]
        #get the name
        infile.seek(next_addr -  next_offset + name_offset)
        name = ""
        curByte = infile.read(1)
        while ord(curByte) != 0:
            name += curByte
            curByte = infile.read(1)
        #get the parent address
        infile.seek(next_top + parent_offset)
        parent_addr = struct.unpack('<I',infile.read(4))[0] - kernel_offset
        #go there, get the ppid
        infile.seek(parent_addr+pid_offset)
        parent_pid = struct.unpack('<I',infile.read(4))[0]
     
        #get the state
        infile.seek(next_top)
        state_num = struct.unpack('<I',infile.read(4))[0]
        if(state_num == 0):
            state = "RU"
        elif state_num == 1:
            state = "IN"
        elif state_num == 130:
            state = "UN,WK"
        else:
            state = "FAIL"

        infile.seek(next_top + uid_offset)
        uid = struct.unpack('<I',infile.read(4))[0]
        infile.seek(next_top + gid_offset)
        gid = struct.unpack('<I',infile.read(4))[0]



        #go to mm_struct
        infile.seek(next_top + mm_struct_offset)
        mm_addr = struct.unpack('<I',infile.read(4))[0] - kernel_offset
        if not mm_addr + kernel_offset == 0:
            infile.seek(mm_addr + mm_hiwater_offset)
            hiwater_vm = struct.unpack('<I',infile.read(4))[0]
            infile.seek(mm_addr + mm_hiwater_offset + 8)
            total_vm = struct.unpack('<I',infile.read(4))[0]
        else:
            hiwater_vm = -1
            total_vm = -1
        print '{0:<16}{1:<8}{2:<8}{3:<8}{4:<8}{5:<16}{6:<8}{7:<8}{8:<8}'.format(hex(next_top+kernel_offset),pid,parent_pid,uid,gid,name,state,hiwater_vm,total_vm)


if __name__ == "__main__":
    main()
