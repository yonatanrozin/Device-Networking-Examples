const data = {};
const socket = new WebSocket("/messages");
socket.addEventListener("open", () => setInterval(() => {
    socket.send(JSON.stringify(getOSCMsgs(data)));
}, 1000/60));

function getOSCMsgs(obj, path = "") {
    const msgs = {};
    for (const key in obj) {
        const keyPath = `${path}/${key}`;
        if (Array.isArray(obj[key])) msgs[keyPath] = obj[key];
        else if (typeof obj[key] === "object") Object.assign(msgs, getOSCMsgs(obj[key], keyPath));
        else msgs[keyPath] = [obj[key]];
    }
    return msgs;
}