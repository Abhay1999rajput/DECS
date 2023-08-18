import matplotlib.pyplot as plt
import numpy as np
  
data=np.genfromtxt("./load_data.csv", delimiter=',')
x=data[:,0:1]
y=data[:,1:2]
y1=data[:,2]
  
plt.plot(x, y1)  # Plot the chart
plt.xlabel("User count")
plt.ylabel("RTT")
plt.show() 
