import requests
import json
import wget
import os

import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--package', dest='pkg', required=True)
parser.add_argument('--folder', dest='pwd', required=True)
args = parser.parse_args()

def install(package_name, pwd):
  url = "https://pypi.org/pypi/" + package_name + "/json"
  r = requests.get(url)
  pkg_url = r.json()['urls']
  url = pkg_url[len(pkg_url) - 1]['url']
  arq = url.split("/")[-1]
  wget.download(url, pwd + arq)
  folder = arq.split(".tar.gz")[0] 
  os.system("cd lib/ && tar -xvf " + pwd + arq)
  os.system("rm " + pwd + arq)
  os.system("cd lib/ && mv " + pwd + folder + "/"+ folder.split("-")[0] + " " + folder.split("-")[0])
  os.system("rm -rf " + pwd + folder)


if __name__== "__main__":
  print()
  install(args.pkg, args.pwd)