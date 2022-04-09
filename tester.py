import requests
import time
#import base64
import colorama
from colorama import Fore, Style

def log(expected, result, path):
    if (expected == result):
        print(Fore.GREEN + "success " + Style.RESET_ALL + " for " + str(path))
    else:
        print(Fore.RED + "failed " + Style.RESET_ALL + " for " + path + ": expected : " + str(expected) + " got " + str(result) + " instead")

def test_get(port, path, status):
    response = requests.get("http://localhost:" + str(port) + str(path))
    log(status, response.status_code, path)
    time.sleep(0.5)


def test_post(port, path, postedFilePath, status):
    response = requests.post("http://localhost:" + str(port) + str(path), open(postedFilePath, 'rb'))
    log(status, response.status_code, path)
    time.sleep(0.5)

def test_delete(port, path, status):
    response = requests.delete("http://localhost:" + str(port) + str(path))
    log(status, response.status_code, path)
    time.sleep(0.5)

        
#This test set is built based on the config.conf file
print("GET :")
test_get(9000, "/doesnotexist", 404)
test_get(9000, "/images/huge", 404)
test_get(9000, "/", 200)
test_get(9000, "/index.html", 200)
test_get(9000, "/images/huge.png", 200)
test_get(9000, "/test/", 405)
#ish, logger shows redir ...
test_get(9000, "/arch/", 200)

print("DELETE :")
test_delete(9000, "/", 405)
test_delete(9000, "/upload/doesnotexist", 200) #delete can remove a folder !

print("POST")
test_post(9000, "/", "./srcs/main.cpp", 405)
test_post(9000, "/upload", "./srcs/main.cpp", 405)

print("valid POST then DELETE")
test_post(9000, "/upload/test", "./srcs/main.cpp", 201)
test_delete(9000, "/upload/test", 204)
test_post(9000, "/upload/SmallImage", "./www/favicon.ico", 201)
test_delete(9000, "/upload/SmallImage", 204)
#test_post(9000, "/upload/BigInvalidImage", "./www/img/huge.png", 413)

print("test with long request URI")
invalid_uri = '/' + ('x' * 2050)
test_get(9000, invalid_uri, 413)

#test error 414
