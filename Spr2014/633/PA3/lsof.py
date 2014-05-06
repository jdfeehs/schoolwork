#!/user/bin/env python
import sys
import struct
def main():
    #define all of the offsets
    kernel_offset = 0xc0000000
    pid_offset = 0xc8
    next_offset = 0x8c
    name_offset = 0x1cd
    file_ptr_offset = 0x484
    dentry_name_ofs = 36
    dentry_inode_ofs = 12


    infile = open(sys.argv[1],'rb')
    desired_pid = int(sys.argv[2])
    task_top = 0x003ed3a0
    cur_pid = 0
    #find the correct task_struct for the desired pid
    while not cur_pid == desired_pid:
        infile.seek(task_top + next_offset)
        next_addr = struct.unpack('<I',infile.read(4))[0] - kernel_offset
        task_top = next_addr - next_offset
        infile.seek(task_top + pid_offset)
        cur_pid = struct.unpack('<I',infile.read(4))[0]
        if(task_top == 0x003ed3a0):
            print "PID ",desired_pid," is not a valid PID for this memory image."
            sys.exit()

    #now we are in the correct task_struct
    #so, get the name (will need it later)
    infile.seek(task_top + name_offset)
    name = ""
    curByte = infile.read(1)
    while ord(curByte) != 0:
        name += curByte
        curByte = infile.read(1)

    #now, navigate into the file_struct
    infile.seek(task_top + file_ptr_offset)
    file_struct_addr = struct.unpack('<I',infile.read(4))[0] - kernel_offset
  
    # if it's a null pointer, just quit
    if(file_struct_addr < 0):
        print "The file_struct pointer was void"
        sys.exit()

    infile.seek(file_struct_addr + 4) #go to the address of the fd table pointer
    fdtable_ptr = struct.unpack('<I',infile.read(4))[0] - kernel_offset

    #Now, go to the fdtable
    infile.seek(fdtable_ptr + 12)
    #this will get the pointers needed from the fdtable
    open_fd_ptr = struct.unpack('<I',infile.read(4))[0] - kernel_offset
    #now get the pointer to the array of files
    infile.seek(fdtable_ptr + 4)
    files_array_ptr = struct.unpack('<I',infile.read(4))[0] - kernel_offset
    
    infile.seek(open_fd_ptr)
    open_fd_int = struct.unpack('<I',infile.read(4))[0]
    #now, do everything else on a bit by bit basis
    open_fd_bits = bin(open_fd_int)[2:].zfill(32)
    
    #print the headers for the output
    print '{0:<16}{1:<8}{2:<8}{3:<8}{4:<8}{5:<12}{6:<8}{7:<16}{8:<32}'.format("Process Name","PID","FD","Mode","Type","Device No","Size","Inode Number","File Name")
    print "----------------------------------------------------------------------------------------------------------------------"  
    fd_num = 0
    #go through the bits in the open fd table, get info, and print
    for i in open_fd_bits[::-1]:
        if i == "1": #this fd exists
            #go to the pointer in the fd array
            infile.seek(files_array_ptr + (fd_num *4)) 
            cur_file_pointer = struct.unpack('<I',infile.read(4)) [0] - kernel_offset
            if cur_file_pointer > 0:
                infile.seek(cur_file_pointer + 8) 
                #get the path and file name
                vfs_mnt = struct.unpack('<I',infile.read(4))[0] - kernel_offset
                infile.seek(cur_file_pointer + 12)
                main_dentry = struct.unpack('I',infile.read(4))[0] - kernel_offset
                infile.seek(vfs_mnt + 16) #go to where the dentry value is in the vfs_mnt
                vfs_mnt_dentry = struct.unpack('<I',infile.read(4))[0] - kernel_offset
                infile.seek(vfs_mnt_dentry + dentry_name_ofs)
                name_ptr = struct.unpack('<I',infile.read(4))[0] - kernel_offset
                infile.seek(name_ptr)
                file_name = ""
                curByte = infile.read(1)
                while ord(curByte) != 0:
                    file_name += curByte
                    curByte = infile.read(1)
                root_name = file_name
                file_name = ""
                #now, get the rest of the name
                infile.seek(main_dentry+dentry_name_ofs)
                name_ptr = struct.unpack('<I',infile.read(4))[0] - kernel_offset
                infile.seek(name_ptr)
                curByte = infile.read(1)
                while ord(curByte) != 0:
                    file_name += curByte
                    curByte = infile.read(1)
                #got the end of the name
                cur_dentry = main_dentry
                parent_name = ""
                parent_not_null = True
                while parent_name != root_name and parent_not_null:
                    parent_name = ""
                    #find the dentry's parent dentry
                    infile.seek(cur_dentry + 24)
                    parent_ptr = struct.unpack('<I',infile.read(4))[0] - kernel_offset
                    if(parent_ptr < 0):
                        parent_not_null = False #we're done
                    else:
                        infile.seek(parent_ptr + dentry_name_ofs)
                        name_ptr = struct.unpack('<I',infile.read(4))[0] - kernel_offset
                        if(name_ptr < 0): #found the top, it's a /
                            file_name = '/' + file_name
                            parent_not_null = False #we need to get out of the loop
                        else: #keep going
                            infile.seek(name_ptr)
                            curByte = infile.read(1)
                            if ord(curByte) != 0:
                                while ord(curByte) != 0:
                                    parent_name += curByte
                                    curByte = infile.read(1)
                                if file_name != "":
                                    if parent_name == "/":
                                        file_name = parent_name + file_name
                                    else:
                                        file_name = parent_name + '/' + file_name
                                else:
                                    file_name = parent_name
                            cur_dentry = parent_ptr + 24
               
                #Now i'm going after the mode
                infile.seek(cur_file_pointer + 28)
                f_mode = struct.unpack('<I',infile.read(4))[0]
                f_mode_str = ""
                if f_mode & 0x4:
                    f_mode_str += "r"
                if f_mode & 0x2:
                    f_mode_str += "w"
                if f_mode & 0x1:
                    f_mode_str += "x"
                if f_mode & 8:
                    f_mode_str += "+"
                if f_mode_str == "": 
                    f_mode_str = "none"
            
                #go to the inode to get the remaining fields
                infile.seek(main_dentry + 12)
                inode_addr = struct.unpack('<I',infile.read(4))[0] - kernel_offset

                #get the file size
                infile.seek(inode_addr + 60)
                file_size = struct.unpack('<I',infile.read(4))[0]

                #get inode_number
                infile.seek(inode_addr + 32)
                inode_no = struct.unpack('<I',infile.read(4))[0]
            
                #get type
                infile.seek(inode_addr + 106) #get i_mode (type)
                i_mode = struct.unpack('<H',infile.read(2))[0] >> 12
                i_mode_str = ""
                if i_mode == 0:
                    i_mode_str = "UNKNOWN"
                if i_mode == 1:
                    i_mode_str = "FIFO"
                if i_mode == 2:
                    i_mode_str = "CHR"
                if i_mode == 4:
                    i_mode_str = "DIR"
                if i_mode == 6:
                    i_mode_str = "BLK"
                if i_mode == 8:
                    i_mode_str = "REG"
                if i_mode == 10:
                    i_mode_str = "LNK"
                if i_mode == 12:
                    i_mode_str = "SOCK"
                if i_mode == 14:
                    i_mode_str = "WHT"

                #get the device number
                infile.seek(inode_addr + 52)
                dev_num = struct.unpack('<I',infile.read(4))[0] 

            print '{0:<16}{1:<8}{2:<8}{3:<8}{4:<8}{5:<12}{6:<8}{7:<16}{8:<32}'.format(name,desired_pid,fd_num,f_mode_str,i_mode_str,dev_num,file_size,inode_no,file_name)
        #increment fd_num for the output and loop
        fd_num = fd_num + 1


if __name__ == "__main__":
    main()
