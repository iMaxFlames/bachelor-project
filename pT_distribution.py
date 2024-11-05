from numpy import *
from os import path
home = path.expanduser("~")

import matplotlib.pyplot as plt

# change the following line to your working folder
working_path = path.join(home, "jetscape-docker", "X-SCAPE", "build")

data_filename = "PythiaGun_MATTER_MARTINI_Hydro.dat"

# Define the headers
hard_process_header = "# HardProcess Parton List: PythiaGun"

hadron_header_start = "# Final State Hadrons"
hadron_header_stop = "Event"

# Initialize a list to store the lines between the headers
hard_process_list = []

event_list = [] # List  of all events
event_i = [] # List of all final state hadrons in event i

# Open the file and read it line by line
with open(path.join(working_path, data_filename), 'r') as file:
    hard_process_capture = False
    hadron_capture = False
    for line in file:
        # Extracting the hard scattering process for each event (just the first line)
        if hard_process_header in line:
            hard_process_capture = True
            continue  # Skip the start header line
        
        if hard_process_capture:
            hard_process_list.append(line.strip())
            hard_process_capture = False
            continue


        # Extracting all the final state hadrons from all events
        if hadron_header_start in line:
            hadron_capture = True
            continue

        if hadron_header_stop in line:
            hadron_capture = False
            if len(event_i) > 0:
                event_list.append(event_i) 
            event_i = []
            continue 

        if hadron_capture:
            event_i.append(line.strip())

# Because there is not stop header for the hadrons at the end 
event_list.append(event_i)

def plot_all_events():
    plt.figure()

    for event in hard_process_list:
        x = float(event.split()[-4])
        y = float(event.split()[-3])
        plt.scatter(x, y)

    plt.xlim(-10, 10)
    plt.ylim(-10, 10)
    plt.xlabel("x")
    plt.ylabel("y")
    plt.savefig("Figures/xy_for_all_events.pdf")
    plt.show()


def plot_high_pT_events(high_pT_threshold=30.0):
    plt.figure()

    for i in range(len(event_list)):
        for hadron in event_list[i]:
            pT = float(hadron.split()[5])
            if pT >= high_pT_threshold:
                x = float(hard_process_list[i].split()[-4])
                y = float(hard_process_list[i].split()[-3])
                plt.scatter(x, y)
                break


    
    plt.xlim(-10, 10)
    plt.ylim(-10, 10)
    plt.xlabel("x")
    plt.ylabel("y")
    plt.savefig("Figures/xy_high_pT_events.pdf")
    plt.show()

plot_high_pT_events()
# plot_all_events()

