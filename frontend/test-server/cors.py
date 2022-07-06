#!/usr/bin/env python3
from http.server import HTTPServer, SimpleHTTPRequestHandler, test
import sys

import json
import requests

#url = 'https://lvx53rzzu7.execute-api.us-east-1.amazonaws.com/default/Quantum_Single_Full'
url = 'http://44.195.46.250:23333'

class CORSRequestHandler (SimpleHTTPRequestHandler):
    def end_headers (self):
      self.send_header('Access-Control-Allow-Origin', '*')
      self.send_header('Access-Control-Allow-Headers', 'content-type')
      SimpleHTTPRequestHandler.end_headers(self)

    def do_OPTIONS (self):
      foo = "{0}"
      
      self.send_response(200)
      self.send_header('Content-Type', 'application/json')
      self.end_headers()
      self.wfile.write(foo.encode('utf-8'))

    def do_POST (self):
      content_len = int(self.headers.get('content-length', 0))
      payload_body = self.rfile.read(content_len)

      # Send HTTP post request and get the response.
      response = requests.post(url, json=json.loads(payload_body))
      amplitudes = response.text
      
      self.send_response(200)
      self.send_header('Content-Type', 'application/json')
      self.end_headers()
      self.wfile.write(amplitudes.encode('utf-8'))

if __name__ == '__main__':
  test(CORSRequestHandler, HTTPServer, port=int(sys.argv[1]) if len(sys.argv) > 1 else 8000)

# initial src: https://stackoverflow.com/a/21957017/11039508
