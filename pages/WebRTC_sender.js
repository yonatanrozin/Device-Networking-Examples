const peerConfiguration = {
    iceServers: [ { urls: [ 'stun:stun.l.google.com:19302', 'stun:stun1.l.google.com:19302' ] } ]
}
const peer = new RTCPeerConnection(peerConfiguration);
const peerSocket = new WebSocket("/RTC_sender");

peerSocket.addEventListener("message", async (msg) => {
    const [eventName, data] = JSON.parse(msg.data);

    if (eventName === "answer") {
        console.log("Answer received.");
        await peer.setRemoteDescription(new RTCSessionDescription(data));
    }
    else if (eventName === "iceCandidate") {
        await peer.addIceCandidate(data);
    }
});

peer.addEventListener("icecandidate", ({ candidate }) => {
    if (candidate) {
        console.log("Local ICE candidate fetched.");
        peerSocket.send(JSON.stringify(["iceCandidate", candidate]));
    }
});

async function RTCConnect() {
    console.log("Sending offer.");
    const offer = await peer.createOffer();
    await peer.setLocalDescription(new RTCSessionDescription(offer));
    peerSocket.send(JSON.stringify(["offer", offer]));
}

peer.onsignalingstatechange = (e) => console.log(e.target.signalingState);
peer.addEventListener("connectionstatechange", () => console.log(peer.connectionState));
