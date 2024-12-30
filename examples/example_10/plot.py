"""
Utility script for plotting with matplotlib
"""
import matplotlib.pyplot as plt
import csv
import math
import numpy as np

def main(filename):
	
	with open(filename, 'r', newline='') as csvfile:
		csv_file_reader = csv.reader(csvfile, delimiter=",")
		
		time = []
		x = []
		y = []
		z = []
		u = []
		v = []
		w = []
		p = []
		q = []
		r = []
		phi = []
		theta = []
		psi = []
		
		for row in csv_file_reader:
			if not row[0].startswith('#'):
				try:

					assert len(row) == 13

					time.append(float(row[0]))
					x.append(float(row[1]))
					y.append(float(row[2]))
					z.append(float(row[3]))
					
					u.append(float(row[4]))
					v.append(float(row[5]))
					w.append(float(row[6]))
					
					p.append(float(row[7]))
					q.append(float(row[8]))
					r.append(float(row[9]))

					phi.append(float(row[10]))
					theta.append(float(row[11]))
					psi.append(float(row[12]))
				except Exception as e:
					print(e)
					continue
		
		assert len(time) == len(x), "Invalid length"
		plt.plot(time, x, label='X^G', linewidth=2)
		plt.plot(time, y, label='Y^G', linewidth=2)
		plt.plot(time, z, label='Z^G', linewidth=2)
		
		plt.xlabel('time (secs)')
		plt.ylabel('position')
		plt.title('Quadrotor position in NED')
		plt.grid()
		plt.legend()
		plt.show()


		assert len(time) == len(phi), "Invalid length"

		plt.plot(time, phi, label='$\phi$', linewidth=2)
		plt.plot(time, theta, label='$\theta$', linewidth=2)
		plt.plot(time, psi, label='$\psi$', linewidth=2)
		plt.xlabel('time (secs)')
		plt.ylabel('Angle')
		plt.title('Euler angles (deg)')
		plt.grid()
		plt.legend()
		plt.show()
		
		
if __name__ == '__main__':
  main("/home/alex/qi3/rlenvs_from_cpp/build/examples/example_10/state.csv")
	
	


