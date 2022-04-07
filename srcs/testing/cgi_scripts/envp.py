import os
import sys
import cgi, cgitb 

print('conTENt-TyPe: text/html\b\n\b\n')
print("hello world!");
print("\b\n\b\n");
print('<font size=+1>Environment</font></br>')
for param in os.environ.keys():
   print('<b>%20s</b>: %s</br>' % (param, os.environ[param]))

print('</br>')

form = cgi.FieldStorage()
print(form)
print(form.getvalue('fname'));

query_string = sys.stdin.read()
print(query_string)
