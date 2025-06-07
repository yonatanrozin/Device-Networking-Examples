# Device-Networking-Examples
A collection of simple code examples to help incorporate wireless technologies in interactive multimedia projects.

## Setup
### Dependencies
- [Node.js](https://nodejs.org/en)

### Download + Install
- Be sure dependencies above are all installed!
- ```git clone``` this repository into a folder of your choice if you've used GitHub, OR...
- Download & extract the [.zip file](https://github.com/yonatanrozin/Device-Networking-Examples/archive/refs/heads/main.zip)
- Navigate to the extracted/cloned folder inside a terminal window
  - Open a new terminal window (Terminal on Mac, Terminal or PowerShell on Windows)
  - Run ```cd <path/to/folder>```. You can type the path manually or drag the folder into the terminal from a finder/file explorer window to paste the file path automatically!
  - Run ```npm install``` to install Node.js dependencies

## Run
 - Navigate to project folder inside a terminal window (see installation steps above)
 - Run ```npm run start```
 - Open ```http://localhost:1880``` in a browser window of your choice

## Setup HTTPS
- In terminal window, from project folder:
  - Run ```npm run get-cert```

## Run HTTPS
- In terminal window, from project folder: 
  - Be sure setup HTTPS steps have been followed!
  - Run ```npm run start-secure```
