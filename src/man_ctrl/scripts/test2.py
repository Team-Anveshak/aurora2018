file= open("../config/drive_config.txt",'r+')
for l in file:
    print l
file.close()
file = open("../config/drive_config.txt",'w')
file.write("123")
