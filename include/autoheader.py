################################################################################
# Copyright (c) 2018 Yirain Suen
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to use,
# copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
# Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
# PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
################################################################################

# Extract from internal/*.h and generate the API header files automatically.
# Usage: python3 autoheader.py
# Requirement: Python3.6.6 / Linux operating system.
# If the internal header files were updated, it will regenerate the API header file, else do nothing.
import os
external = "bitcointk"
internal = "internal"

# The folder's name is the module's name, the rest header files under 'include/internal' will be ignored.
def getModuleStructure():
	# Get module names.
	modules = []
	for item in os.walk(internal):
		modules.append(item[1])
	
	# Get header names
	dic = {}
	for name in modules[0]:
		for item in os.walk(internal+"/"+name):
			dic[name]=item[2]
	return dic

def generate(structure):
	for mName, hFiles in structure.items():
		with open(external+'/'+mName+'.h', 'w') as mfile:
			allLines = []
			for hfile in hFiles:
				with open(internal+'/'+mName+'/'+hfile) as h:
					allLines.append('\r')
					for line in h.readlines():
						allLines.append(line)
			mfile.writelines(allLines)
d = getModuleStructure()
print(d)
generate(d)