#!/Python27/python.exe

# we need comprehensive debugging
import cgitb
cgitb.enable()

# let's use this stuff
import os
import os.path
import mysql.connector
import datetime
import sys

# we need these global variables for adequate web interaction
_get = {}
_post = {}
_cookie = {}
output = ""
response_headers = []

def parseQueryTypeString(theStr):
    rawsplit = []
    ret = {}

    if len(theStr) > 0:
        rawsplit = theStr.split("&")

        for x in rawsplit:
            tmpSplit = x.split("=")
            ret[tmpSplit[0]] = tmpSplit[1]

    return ret

def parseGet():
    global _get
    # parse the php _GET[] equivalents
    _get = parseQueryTypeString(os.environ['QUERY_STRING'])


def parsePost():
    global _post
    if os.environ['REQUEST_METHOD'] == 'POST':
        # read in raw form data as recieved via POST
        _post = parseQueryTypeString(sys.stdin.read())
    else:
        _post = {}

    fnd = ["+", "%3D", "%21", "%40", "%23", "%24", "%25", "%5E", "%26", "%28", "%29", "%2F"]
    rpl = [" ", "=", "!", "@", "#", "$", "%", "^", "&", "(", ")", "/"]

    for x in _post.keys():
        for i in range(len(fnd)):
            _post[x] = _post[x].replace(fnd[i], rpl[i])


def parseCookies():
    global _cookie
    if "HTTP_COOKIE" in os.environ:
        c = os.environ['HTTP_COOKIE'].split("; ")
        for x in c:
            cs = x.split("=")
            _cookie[cs[0]] = cs[1]

def set_cookie(key, value, days_expire = 7):
    global response_headers
    if days_expire is None:
        max_age = 365 * 24 * 60 * 60  #one year
    else:
        max_age = days_expire * 24 * 60 * 60 
    expires = datetime.datetime.strftime(datetime.datetime.utcnow() + datetime.timedelta(seconds=max_age), "%a, %d-%b-%Y %H:%M:%S GMT")
    response_headers.append(('Set-Cookie', key + "=" + value + "; Expires=" + expires + "; Path=/"))


def force_download_of_page(filename):
    global response_headers
    response_headers.append(('Content-Disposition', 'attachment; filename="' + filename + '"'))

def getd3d(cnx):
    cur1 = cnx.cursor()
    cur1.execute("select count(*) as cnt from data")
    res = cur1.fetchall()
    ret = res[0][0]
    cur1.close()
    return ret

def test():
    cnx = mysql.connector.connect(user='root', password='', host='127.0.0.1', database='d3d')

    oldd3d = getd3d(cnx)
    f3d(cnx)
    newd3d = getd3d(cnx)

    cnx.commit()
    cnx.close()

    if (newd3d != oldd3d):
        return "worked! old: {0}, new: {1}".format(oldd3d, newd3d)
    else:
        return "failed!"

def print_linebreak():
    global output
    output += "<br /><hr><br />"

def print_list_items(list_):
    global output
    for x,c in list_.items():
        output += x + '=' + c + '<br />'

def print_env_vars():
    print("<font size=+1>Environment</font><br />")
    for param in os.environ.keys():
        print("<b>%20s</b>: %s<br />" % (param, os.environ[param]))

def print_folder_items():
    for files in os.listdir("."):
        print(files + "<br />")

####################
## TEST STUFF ##
####################

# yay trash output
# print_list_items(_get)
# print_linebreak()
# print_list_items(_post)
# print_linebreak()
# print_list_items(_cookie)
# print_linebreak()
# output += test()
# print_linebreak()

# output += '<form method="post"><input name="dero" type="text"><input type="submit"></form>'

#############################
# start page execution here #
#############################

# necessary header
response_headers.append(('Content-type', 'text/html'))

# parse all input variables
parseGet()
parsePost()
parseCookies()

##############################
#  page content starts here  #
##############################

corelocalpath = "C:/xampp/htdocs/x"

if "newcat_name" in _post:
    try:
        os.mkdir(corelocalpath + _post["newcat_requestsource"] + "/" + _post["newcat_name"])
    except WindowsError as e:
        output += "<div style='background-color: red;'>" + e.strerror + "</div><br />"

# work out the target path
# note this has absolutely NO error checking (invalid paths are still processed)
if not "path" in _get:
    _get["path"] = "/"

localpath = corelocalpath + _get["path"]
onlydirectories = [ f for f in os.listdir(localpath) if os.path.isdir(os.path.join(localpath,f)) ]

output += "<table width=500px border=1px><tr><td>Subcats</td></tr>"

for name in onlydirectories:
    output += "<tr><td><a href='?path=" + _get["path"] + name + "/'>" + name + "</a></td></tr>"

output += "<tr><td><form style='border: 0px; padding: 0px; margin: 0px;' method='post'><input name='newcat_requestsource' type='hidden' value='" + _get["path"] + "'><input name='newcat_name' type='text'><input name='newcat_submit' type='submit' value='Create Category'></form></td></tr>"

output += "</table>"

##############################
#   page content ends here   #
##############################

# output everything the page wants
for x in response_headers:
    print(x[0] + ": " + x[1] + "\r\n")
print(output)