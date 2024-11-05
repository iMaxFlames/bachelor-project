from numpy import *
from os import path
home = path.expanduser("~")

import matplotlib.pyplot as plt

# change the following line to your working folder
working_path = path.join(home, "jetscape-docker", "X-SCAPE", "build")

data_filename = "PythiaGun_MATTER_MARTINI_Hydro.dat"

# Define the headers
start_header = "# HardProcess Parton List: PythiaGun"
end_header = "# Energy loss Shower Initating Parton: JetEnergyLoss"

# Initialize a list to store the lines between the headers
event_list = []
event_i = []

# Open the file and read it line by line
with open(path.join(working_path, data_filename), 'r') as file:
    capture = False
    for line in file:
        # Check if we've reached the start header
        if start_header in line:
            capture = True
            continue  # Skip the start header line
        
        # Check if we've reached the end header
        if end_header in line:
            capture = False
            if len(event_i) > 0:
                event_list.append(event_i) 
            event_i = []
            continue 

        # If we're between headers, capture the line
        if capture:
            event_i.append(line.strip())

# Display the captured lines
# print("\n".join(lines_between_headers))

# print(event_list[0][0])
# line_split = event_list[0][0].split()
# print(line_split)

# print(line_split[-4], line_split[-3])
# print(type(line_split[-4]))


def plot_all_events():
    plt.figure()

    for event in event_list:
        particle = event[0]
        x = float(particle.split()[-4])
        y = float(particle.split()[-3])
        plt.scatter(x, y)

    plt.xlim(-10, 10)
    plt.ylim(-10, 10)
    plt.xlabel("x")
    plt.ylabel("y")
    plt.savefig("xy_for_all_events.pdf")
    plt.show()


def plot_high_pT_events():
    plt.figure()

    for event in event_list:
        for particle in event:
            if float(particle.split()[3]) > 80.0:
                x = float(particle.split()[-4])
                y = float(particle.split()[-3])
                plt.scatter(x, y)
                break
    
    plt.xlim(-10, 10)
    plt.ylim(-10, 10)
    plt.xlabel("x")
    plt.ylabel("y")
    plt.savefig("xy_high_pT_events.pdf")
    plt.show()

plot_high_pT_events()
# plot_all_events()