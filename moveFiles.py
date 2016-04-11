import os;
import shutil;
import sys;

projName = sys.argv[1];

srcInc = "./" + projName + "/";
srcLib = "./Debug/";
desInc = "./Dist/include/" + projName + "/";
desLib = "./Dist/lib/";

if not os.path.exists(desInc):
	os.makedirs(desInc);
	
if not os.path.exists(desLib):
	os.makedirs(desLib);

print "[INCLUDES]";
for file in os.listdir(srcInc):
	if file.endswith(".h") or file.endswith(".hpp"):
		print file;
		shutil.copyfile(srcInc + file, desInc + file);
		
print "\n[LIBRARIES]"
for file in os.listdir(srcLib):
	if file.endswith(".lib"):
		print file;
		shutil.copyfile(srcLib + file, desLib + file);