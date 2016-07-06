import os;
import shutil;
import sys;

srcPath_inc = "./GLT/";
desPath_inc = "./!RELEASE/include/GLT/";

srcPath_lib = "./Bin/";
desPath_lib = "./!RELEASE/lib/";


print "[CLEANING]"
for file in os.listdir(desPath_inc):
	filePath = os.path.join(desPath_inc, file);
	if (os.path.isfile(filePath)):
		os.unlink(filePath);
		print filePath;

print "[INCLUDES]";
for file in os.listdir(srcPath_inc):
	if file.endswith(".h") or file.endswith(".hpp"):
		fileSrc = os.path.join(srcPath_inc, file);
		fileDes = os.path.join(desPath_inc, file);

		shutil.copyfile(fileSrc, fileDes);
		print fileSrc + " --> " + fileDes;

print "[LIBS]"
def copyLibrary( suffix ):
	for file in os.listdir(srcPath_lib + suffix):
		if file.endswith(".lib"):
			fileSrc = os.path.join(srcPath_lib + suffix, file);
			fileDes = os.path.join(desPath_lib, file);

			print fileSrc + " --> " + fileDes;
			shutil.copyfile(fileSrc, fileDes);

#Debug libraries
copyLibrary("Debug/");
copyLibrary("Release/");