<h1 align="center">NanoBrain - CLI</h1>

<h3 align="center">
	<a  style="text-decoration: none"  href="#requirements">Requirements</a>
	<span> · </span>
	<a  style="text-decoration: none"  href="#install">Install</a>
	<span> · </span>
	<a  style="text-decoration: none"  href="#resources">Resources</a>
	<span> · </span>
	<a  style="text-decoration: none"  href="#usage">Usage</a>
</h3>

Crie seu sistema embarcado com aparência profissional, de maneira prática e rápida. **NanoBrain** compila uma imagem linux pequena e totalmente customizável. Para aplicações em python o pygame é a base para a interface gráfica do seu sistema, mas nada impede de você criar da sua maneira.

## Requirements
### Host systems
* Mac OS 10+
* Linux

### Prerequisities

* git
* Docker
* Python2
	* pip
	* requests
	* wget
* Netpbm

## Install

When starting we must prepare the development environment.

First of all NanoBrain works using containers so its installation of docker is necessary, you can install it by clicking [here!](https://www.docker.com/get-started)

The other dependencies you can install using the terminal, like this:

### Mac-os X:
Install Python2:

``` 
$ brew install python@2 git
```
### Linux (Debian)

Prerequsities:

```
$ sudo apt-get update
$ sudo apt-get -y install python python2 git netpbm curl build-essential
```
### All systems
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
## Resources

## Usage

### New Project

### Install Packages

### Change bootup image

### Save my own container

### Burn image




