const data = {};

let socket;
function socketConnect() {
    socket = new WebSocket("/messages");
    socket.addEventListener("open", () => setInterval(() => {
        const OSCMsgs = getOSCMsgs(data);
        console.log(OSCMsgs)
        socket.send(JSON.stringify({packets: OSCMsgs}));
    }, 1000/60));
    socket.addEventListener("close", () => {
        setTimeout(socketConnect, 500);
    });
}
socketConnect();

function getOSCArgs(args) {
    if (!Array.isArray(args)) args = [args];
    return args.map((value) => {
        const type = {"boolean": "i", "number": "f", "string": "s"}[typeof value];
        return {type, value};
    });
}

function getOSCMsgs(obj, path = "") {
    const msgs = [];
    for (const key in obj) {
        const keyPath = `${path}/${key}`;
        if (Array.isArray(obj[key])) msgs.push({address: keyPath, args: getOSCArgs(obj[key])});
        else if (typeof obj[key] === "object") msgs.push(...getOSCMsgs(obj[key], keyPath));
        else msgs.push({address: keyPath, args: getOSCArgs(obj[key])});
    }
    return msgs;
}