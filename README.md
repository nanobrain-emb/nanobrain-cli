<div align="center"><img src="https://avatars1.githubusercontent.com/u/62779874?v=4" width="150px" /></div>
</p>
<h3 align="center">
	<a  style="text-decoration: none"  href="#requirements">Requirements</a>
	<span> · </span>
	<a  style="text-decoration: none"  href="#install">Install</a>
	<span> · </span>
	<a  style="text-decoration: none"  href="#usage">Usage</a>
</h3>

Create your professional-looking embedded system in a practical and fast way. **NanoBrain** compiles a small, fully customizable linux image. For python applications pygame is the basis for your system's graphical interface, but nothing prevents you from creating it your way.

## Requirements
### Host systems
* Mac OS 10+
* Linux

#### Prerequisities

* git
* Docker
* Python2
	* pip
	* requests
	* wget
* Netpbm

#### Install

When starting we must prepare the development environment.

First of all NanoBrain works using containers so its installation of docker is necessary, you can install it by clicking [here!](https://www.docker.com/get-started)

The other dependencies you can install using the terminal, like this:

##### Mac-os X:
Install Python2:

``` 
$ brew install python@2 git
```
##### Linux (Debian)

Prerequsities:

```
$ sudo apt-get update
$ sudo apt-get -y install python python2 git netpbm curl build-essential
```
##### All systems
Install python dependencies:

```
$ curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
$ python get-pip.py
```
```
$ pip install requests wget
```

#### Install nanorabrain-cli: 

```
$ git clone https://github.com/nanobrain-emb/nanobrain-cli.git
```
```
$ cd nanobrain-cli && make
```
### Target
Raspberry pi 3.
![](https://miro.medium.com/max/1400/1*A2gpUDLyOx903dVUStHFTA.jpeg)

## Usage

### Create Project

Define the enviroment variable

```
~$ export NANOBRAIN_BOARD=rpi3
~$ export NANOBRAIN_LANG=python
```

Create a new python-embedded projet: `nanobrain-cli new [name]`

```
~$ nanobrain-cli new HelloWorld
~$ cd HelloWorld
```

### Install Packages

You can include libraries in your embedded project `nanobrain-cli deps install [lib-name ...]`

```
~/HelloWorld $ nanobrain-cli deps install qrcode matplot
```

### Burn image
To generate an image and burn to the sdcard, simply execute: `nanobrain-cli -fb [driver]`

Example:
```
~/HelloWorld $ nanobrain-cli -fb /dev/disk2
```




