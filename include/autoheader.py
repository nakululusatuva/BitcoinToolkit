################################################################################
# Copyright (c) 2018 Yirain Suen                                               #
#                                                                              #
# Permission is hereby granted, free of charge, to any person obtaining a copy #
# of this software and associated documentation files (the "Software"), to     #
# deal in the Software without restriction, including without limitation the   #
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or  #
# sell copies of the Software, and to permit persons to whom the Software is   #
# furnished to do so, subject to the following conditions:                     #
#                                                                              #
# The above copyright notice and this permission notice shall be included in   #
# all copies or substantial portions of the Software.                          #
#                                                                              #
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR   #
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,     #
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE  #
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER       #
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING      #
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS #
# IN THE SOFTWARE.                                                             #
################################################################################

################################################################################
# Extract all the subheader files(internal/MODULE/*.h or internal/MODULE.h)    #
# under the module and recombine to a single API header file.                  #
# Usage: python3 autoheader.py                                                 #
# Requirement: Python3.6+ / Linux operating system.                            #
################################################################################

import os
external = "bitcointk"
internal = "internal"

copyright = \
'/** \n\
*  MIT LICENSE\n\
*  Copyright (c) 2018 Yirain Suen\n\
*  Permission is hereby granted, free of charge, to any person obtaining a copy\n\
*  of this software and associated documentation files (the "Software"), to\n\
*  deal in the Software without restriction, including without limitation the\n\
*  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or\n\
*  sell copies of the Software, and to permit persons to whom the Software is\n\
*  furnished to do so, subject to the following conditions:\n\
*  \n\
*  The above copyright notice and this permission notice shall be included in\n\
*  all copies or substantial portions of the Software.\n\
*  \n\
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n\
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n\
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n\
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n\
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING\n\
*  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS\n\
*  IN THE SOFTWARE.\n\
**/\n'

description = \
'/** \n\
*  AUTO-GENERATED CODE, DO NOT EDIT EXCEPT EXPERIMENTALLY\n\
*  Maintainers: Do not include this header internal to this library.\n\
**/\n'

# One-dimension list.
# The lines which the leading charater matched will be deleted.
# example: leadingPattern = ['*', '/']
leadingPattern = []

# One-dimension list.
# The whole line that matched will be deleted.
# example: Pattern = ['\n']
Pattern = []

# Two-dimension list (m by 2).
# The lines between the 'BEGIN TAG' and 'END TAG' will be deleted.
paragraphPattern = [
	['/** AUTOHEADER TAG: DELETE BEGIN **/\n', '/** AUTOHEADER TAG: DELETE END **/\n']
]

# One-dimension list
# Lines in the list will be added in the front of the file.
constantSpices = ['#include "common.h"\n']

# Two-dimension list (m by 2).
quoteSpices = [
	['#ifdef __cpluscplus\nextern "C" {\n#endif\n', '#ifdef __cpluscplus\n}\n#endif\n']
]

includeDependency = {
	'machine': 'container'
}

def getModulesStructure():
	# Get module names.
	modules_dir = []
	modules_file = []
	for item in os.walk(internal):
		# folders under internal/
		modules_dir += item[1]
		# files under internal/
		for file in item[2]:
			modules_file.append(file.rstrip('.h'))
		break

	# Generate module's structure
	structure_dir = {}
	structure_file = {}
	# Get header files under the module folder.
	for module in modules_dir:
		for item in os.walk(internal+"/"+module):
			structure_dir[module] = item[2]
	# Single header file as a module.
	for module in modules_file:
		for item in modules_file:
			structure_file[module] = [module + '.h']

	return structure_dir, structure_file

def extractInternalHeader(structure_dir, structure_file):
	extracted = {}
	# module_dir loop.
	for module, headers in structure_dir.items():
		allLines = []
		for header in headers:
			allLines.append('\n')
			with open(internal+'/'+module+'/'+header) as h:
				allLines.extend(h.readlines())
				allLines[-1] += '\n'
		extracted[module] = allLines

	# module_file loop.
	for module, header in structure_file.items():
		allLines = []
		with open(internal+'/'+header[0]) as h:
			allLines.extend(h.readlines())
			allLines[-1] += '\n'
		extracted[module] = allLines

	return extracted

def removeGut(extracted):
	gutted = {}

	# paragraph pattern
	for tags in paragraphPattern: # paragraph pattern tags loop
		for module, lines in extracted.items(): # modules loop
			flag = True
			keep = []
			for line in lines: # lines loop
				if line == tags[0]:
					flag = False
				elif line == tags[1]:
					flag = True
					continue
				if flag:
					keep.append(line)
			gutted[module] = keep

	# allMatched pattern
	for item in gutted.items():
		for line in item[1][:]:
			if line in Pattern:
				item[1].remove(line)

	# leading pattern
	for item in gutted.items():
		for line in item[1][:]:
			if line[0] in leadingPattern:
				item[1].remove(line)

	return gutted

def addSpices(gutted, constSpiceExceptions, quoteSpiceExceptions):
	spiced = gutted

	# Re-add include dependency.
	for module_name, dependence in includeDependency.items():
		for module, lines in spiced.items():
			if module == module_name:
				lines.insert(0, '#include "'+dependence+'.h'+'"')

	# Add constant spices.
	for spice in constantSpices:
		for module, lines in spiced.items():
			if module in constSpiceExceptions:
				continue
			lines.insert(0, spice)
	
	# Add quote spices.
	for spice in quoteSpices:
		for module, lines in spiced.items():
			if module in quoteSpiceExceptions:
				continue
			lines.insert(0, spice[0])
			lines.append(spice[1])

	# Add description and copyright.
	for module, lines in spiced.items():
		lines.insert(0, description)
	for module, lines in spiced.items():
		lines.insert(0, copyright)
	
	return spiced

def cook(processed):
	for module, lines in processed.items():
		with open(external+'/'+module+'.h', 'w') as f:
			f.writelines(lines)

s = getModulesStructure()
e = extractInternalHeader(s[0], s[1])
g = removeGut(e)
a = addSpices(g, ['common'], [])
cook(a)