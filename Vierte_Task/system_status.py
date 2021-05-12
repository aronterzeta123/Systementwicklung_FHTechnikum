#!/usr/bin/python3
import psutil
import time
import os
import subprocess
import smtplib
import email.utils
from email.mime.text import MIMEText
import getpass
import sys
import threading
to_email = input('Recipient: ')
servername = input('Mail server name: ')
username = input('Mail user name: ')
password = getpass.getpass("%s's password: " % username)
msg1=""
def send_email(): 
    while(True):    
        time.sleep(600)
        global msg1	
        msg = MIMEText(msg1)
        msg.set_unixfrom('author')
        msg['To'] = email.utils.formataddr(('Recipient', to_email))
        msg['From'] = email.utils.formataddr(('Author', username))
        msg['Subject'] = 'System Status'

        server = smtplib.SMTP(servername,587)
        server.set_debuglevel(True)

    # identify ourselves, prompting server for supported features
        server.ehlo()

    # If we can encrypt this session, do it
        if server.has_extn('STARTTLS'):
            server.starttls()
            server.ehlo() # re-identify ourselves over TLS connection

        server.login(username, password)
        server.sendmail('%s'%username, [to_email], msg.as_string())
        server.quit()


cpu_percent=0
disk_usage=0
sum=0
sum1=0
sum2=0
system_temp=0
counter1=0
counter=0

t = threading.Thread(target=send_email)
t.start()

while(1):
	#Zeigt die CPU usage
	sum+=psutil.cpu_percent();
	

	# Zeigt the disk usage from Root verzeichnis
	disk_usage=format(psutil.disk_usage('/'));
	c=disk_usage.split(', ');
	d=c[3].split('=');
	e=d[1].split(')');
	disk_usage_percent=e[0];
	sum1+=int(float(disk_usage_percent));

	# Zeigt die System Temperatur
	#sum2+=subprocess.check_output("cat /sys/class/thermal/thermal_zone0/temp",shell=True)/1000;
	sum2+=10
	# Zeigt die Zahl der Current logged in users
	
	sos=subprocess.check_output("w",shell=True)
	user=sos.split(','.encode());
	userloggedin=user[1].split('  '.encode());
	userloggedin2=userloggedin[1].split(' '.encode());
	counter+=1;
	if(counter%10==0):
		cpu_percent=sum/10;
		disk_use=sum1/10;
		system_temp=sum2/10;
		print("Mean value for 10 Values CPU usage is:%f "%(cpu_percent));
		print("Mean value for 10 Values Disk Usage in Percent is: %f "%(disk_use));
		print("Mean value for 10 values system_temp in Celsius is: %f" %(system_temp)); 
		print("Number of logged in users: %d" %(int(userloggedin2[0].decode())));
		sum=0;
		sum1=0;
		sum2=0;
		msg_part1="Mean value for 10 Values CPU usage: " + str(cpu_percent) +"\n"
		msg_part2="Mean value for 10 Values Disk usage: " + str(disk_use)+ "\n"
		msg_part3="Mean value for 10 Values System Temperature in Celsius: "+ str(system_temp)+"\n"
		msg_part4="Number of logged in users: "+ str((int(userloggedin2[0].decode())))+"\n"
		msg1+=(msg_part1+msg_part2+msg_part3+msg_part4)
	time.sleep(10);
