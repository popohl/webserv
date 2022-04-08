import os
import cgi, cgitb 
form = cgi.FieldStorage()

print('conTENt-TyPe: text/html\b\n\b\n')
print('<font size=+1>Hello there')
print(' ', form.getvalue('name'))
print(', here are your pictures:</font></br>')

path = os.environ['PATH_TRANSLATED']
print('<h3>displaying folder: ', path, '</h3>')

filesinpath = os.listdir(path)

print("<ul>")

for file in filesinpath:
    print("<li>", file, "</li>")
    print("<li><img src=\"../../../images/" + file + "\" alt=\"Girl in a jacket\" width=\"500\" height=\"600\"></li>")

print("</ul>")
