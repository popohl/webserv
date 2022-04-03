import os
import cgi, cgitb 

print('Content-type: text/html\b\n\b\n')
print('<font size=+1>Environment</font></br>')
for param in os.environ.keys():
   print('<b>%20s</b>: %s</br>' % (param, os.environ[param]))

print('</br>')

form = cgi.FieldStorage()
print(form)
# print(form["fname"].value);
