from __future__ import division
import sys
import numpy as np

if len(sys.argv)!=3:
	print 'Invalid number of arguments. Please use:\n\tpython SNPtoSLC.py [HV.snp] [hvSettingsOutput.txt]'
	exit(-1)

with open(sys.argv[2],'w') as g:
	g.write("#Sector\tLayer\tComponent\tOrder\tHV\n")
	with open(str(sys.argv[1]),'rb') as a:
			for line in a:
				li = line.strip().split(":")
				if len(li)!=2: continue
				if 'vset' not in li[1]: continue
				name = str(li[0].split("HV_")[1])
				hv = float(li[1].split(" ")[2])

				# Parse layer
				layer = str(name[0])
				if layer == 'V': layer = 6

				# Parse sector, component
				comp = int(name[1:3])
				sector = -1
				if comp < 4: sector = 1
				elif comp < 11: sector = 2; comp-=3
				elif comp < 17 and 'B' not in name: sector = 3; comp-=10
				elif comp < 17 and 'A' not in name: sector = 4; comp-=10
				else: sector = 5; comp-=16

				# Parse order
				order = -1
				if 'L' in name: order = 0
				elif 'R' in name: order = 1
				elif 'V' in name and 'B' not in name: order = 0
				elif 'V' in name and 'A' not in name: order = 1
		
				g.write(str(sector)+"\t"+str(layer)+"\t"+str(comp)+"\t"+str(order)+"\t"+str(hv)+"\n")

