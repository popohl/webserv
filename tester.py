import requests

def test_get(port, path, status):
    response = requests.get("http://localhost:" + str(port) + str(path))
    if (response.status_code == status):
        print("success for GET " + str(path))
    else:
        print("failed for " + path + ": expected : " + str(status) + " got " + str(response.status_code) + " instead")


#This test set is built based on the config.conf file

test_get(9000, "/doesnotexist", 404)
test_get(9000, "/images/huge", 404)
test_get(9000, "/", 200)
test_get(9000, "/index.html", 200)
test_get(9000, "/images/huge.png", 200)
test_get(9000, "/test/", 405);
#ish, logger shows redir ...
test_get(9000, "/arch/", 200);

