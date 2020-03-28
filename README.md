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


## Requirements
* Docker
* Python2
	* pip
	* requests
* Netpbm

## Install

Ao começar devemos preparar o ambiente de desenvolvimento.

Antes de tudo **NanoBrain** funciona usando containers por isso é necessário sua instalação, você pode instala-lo clicando [here!](https://www.docker.com/get-started)

As outras dependências você pode instalar usando o terminal, assim:
### Mac-os X:
Install Python2:

``` 
$ brew install python@2 git
```
### Linux (Debian)

Prerequsities:

```
$ sudo apt-get update
```
```
$ sudo apt-get -y install python python2 git netbpm build-essential
```
### All systems
Install pip and requests:

```
$ curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
$ python get-pip.py
```
```
$ pip install requests
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




