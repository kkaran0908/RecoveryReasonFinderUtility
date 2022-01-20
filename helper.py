from paramiko import SSHClient
from scp import SCPClient
import subprocess

def ssh_scp_files(ssh_host, ssh_user, ssh_password, ssh_port, source_volume, destination_volume,server_restart_time):
    print("Copying the file from prod-sim: "+ ssh_host +" Please wait!\n")
    ssh = SSHClient()
    ssh.load_system_host_keys()
    ssh.connect(ssh_host, username=ssh_user, password=ssh_password, look_for_keys=False)

    with SCPClient(ssh.get_transport()) as scp:
        scp.get(source_volume, destination_volume)

    print("Done with copying the file!")
    subprocess.call(["./adl_recovery_reason_finder",server_restart_time])


ssh_host = "10.192.2.176"  #IP of prod-sim machine
ssh_user ="*************" #user_name  
ssh_password = "**************"  #password
ssh_port = 22
destination_volume = "./"
source_volume = "/var/log/debesys/algojob-algoserver_exec-1.log" #address of the remote log file
server_restart_time = "2022-01-20 06:44:34" #server restart time (find from https://kibana-int.debesys.net/)

ssh_scp_files(ssh_host, ssh_user, ssh_password, ssh_port, source_volume, destination_volume,server_restart_time)